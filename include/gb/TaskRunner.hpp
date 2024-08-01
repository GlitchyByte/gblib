// Copyright 2024 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "Task.hpp"
#include <vector>
#include <set>
#include <mutex>
#include <condition_variable>
#include <thread>

namespace gb {

    /**
     * Task runner facility to run tasks in their own threads.
     *
     * <p>Note that start method will block until the task signals it has started. This is by design
     * to ensure a task is ready to accept input, for example.
     */
    class TaskRunner {
    private:
        static inline bool taskComparator(std::shared_ptr<Task> const& lhs, std::shared_ptr<Task> const& rhs) noexcept {
            return lhs->taskId < rhs->taskId;
        }

    private:
        std::atomic<bool> _isActive { true };
        std::set<std::shared_ptr<Task>, decltype(&taskComparator)> tasks { taskComparator };
        std::mutex tasksLock;
        std::condition_variable isEmptySignal;
        std::thread threadController;
        std::mutex threadControllerLock;
        bool threadControllerShouldExit { false };
        std::vector<std::shared_ptr<Task>> finishingTasks;
        std::condition_variable threadControllerWakeUpSignal;

    public:
        /**
         * Creates a task runner.
         */
        TaskRunner() noexcept {
            threadController = std::thread {
                // Waits for threads to join and removes them from the set.
                [this]{
                    std::unique_lock<std::mutex> lock { threadControllerLock };
                    while (true) {
                        threadControllerWakeUpSignal.wait(lock, [this]{ return threadControllerShouldExit || !finishingTasks.empty(); });
                        if (threadControllerShouldExit) {
                            break;
                        }
                        for (auto& task: finishingTasks) {
                            task->thread.join();
                            removeTask(task);
                        }
                        finishingTasks.clear();
                    }
                }
            };
        }

        /**
         * Destroys the task runner.
         *
         * <p>It shuts down the runner as per shutdown().
         */
        ~TaskRunner() noexcept {
            shutdown();
        }

        /**
         * Returns true if the runner is active and able to accept tasks.
         *
         * @return True if the runner is active and able to accept tasks.
         */
        [[nodiscard]]
        bool isActive() const noexcept {
            return _isActive.load();
        }

        /**
         * Shuts down the runner.
         *
         * <p>Cancels all tasks and awaits on all tasks to stop.
         */
        void shutdown() noexcept {
            std::unique_lock<std::mutex> lock { tasksLock };
            if (!_isActive.compare_exchange_strong(_true, false)) {
                return;
            }
            for (auto const& task: tasks) {
                task->cancel();
            }
            isEmptySignal.wait(lock, [this]{ return tasks.empty(); });
            lock.unlock();
            std::unique_lock<std::mutex> controllerLock { threadControllerLock };
            threadControllerShouldExit = true;
            threadControllerWakeUpSignal.notify_one();
            controllerLock.unlock();
            threadController.join();
        }

        /**
         * Starts a task.
         *
         * <p>This method will block until the task signals it has started.
         *
         * @param task Task to start.
         * @return True if the task was started, false if the runner is not active.
         */
        bool start(std::shared_ptr<Task> const& task) noexcept {
            std::lock_guard<std::mutex> lock { tasksLock };
            if (!isActive()) {
                return false;
            }
            auto const& [ _, success] = tasks.insert(task);
            if (!success) {
                return false;
            }
            task->setTaskRunner(this);
            task->thread = std::thread {
                [this, task]{
                    task->action();
                    task->finished();
                    std::lock_guard<std::mutex> lock { threadControllerLock };
                    finishingTasks.push_back(task);
                    threadControllerWakeUpSignal.notify_one();
                }
            };
            task->awaitStart();
            return true;
        }

        /**
         * Cancels all tasks.
         */
        void cancelAll() noexcept {
            std::lock_guard<std::mutex> lock { tasksLock };
            for (auto const& task: tasks) {
                task->cancel();
            }
        }

        /**
         * Awaits for all tasks to finish.
         */
        void awaitAll() noexcept {
            std::unique_lock<std::mutex> lock { tasksLock };
            isEmptySignal.wait(lock, [this]{ return tasks.empty(); });
        }

    private:
        void removeTask(std::shared_ptr<Task> const& task) noexcept {
            std::lock_guard<std::mutex> lock { tasksLock };
            tasks.erase(task);
            if (tasks.empty()) {
                isEmptySignal.notify_one();
            }
        }
    };
}
