// Copyright 2023-2024 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "constants.hpp"
#include <csignal>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <functional>

namespace gb {

    /**
     * Utility to monitor SIGINT and SIGTERM for proper application shutdown.
     */
    class ShutdownMonitor {
    private:
        static inline std::atomic<bool> generalShutdownInitiated { false };
        static inline std::mutex generalShutdownLock;
        static inline std::vector<std::shared_ptr<ShutdownMonitor>> shutdownMonitors;

        static inline void triggerShutdown([[maybe_unused]] int const signal) noexcept {
            if (!generalShutdownInitiated.compare_exchange_strong(_false, true)) {
                return;
            }
            std::lock_guard<std::mutex> lock { generalShutdownLock };
            for (auto const& monitor: shutdownMonitors) {
                monitor->shutdown();
            }
            shutdownMonitors.clear();
        }

    public:
        /**
         * Creates a monitor that will get notified when it's time for an orderly shutdown.
         *
         * @return A shutdown monitor.
         */
        [[nodiscard]]
        static inline std::shared_ptr<ShutdownMonitor> create() noexcept {
            static bool initialized { false };
            if (!initialized) {
                std::signal(SIGINT, triggerShutdown);
                std::signal(SIGTERM, triggerShutdown);
                initialized = true;
            }
            std::lock_guard<std::mutex> lock { generalShutdownLock };
            bool const isShuttingDown { generalShutdownInitiated };
            std::shared_ptr<ShutdownMonitor> const monitor { new ShutdownMonitor(isShuttingDown) };
            if (!isShuttingDown) {
                shutdownMonitors.push_back(monitor);
            }
            return monitor;
        }

    private:
        std::atomic<bool> isShuttingDown { false };
        std::mutex shutdownLock;
        std::condition_variable shuttingDown;

        explicit ShutdownMonitor(bool const shuttingDown) noexcept : isShuttingDown(shuttingDown) {}

    public:
        /**
         * Returns true when an orderly shutdown should occur.
         *
         * @return True when an orderly shutdown should occur.
         */
        [[nodiscard]]
        bool shouldShutdown() const noexcept {
            return isShuttingDown;
        }

        /**
         * Manually triggers an orderly shutdown.
         */
        void shutdown() noexcept {
            isShuttingDown = true;
            std::lock_guard<std::mutex> lock { shutdownLock };
            shuttingDown.notify_all();
        }

        /**
         * Awaits for a shutdown or expiration of the given timeout.
         * <p>
         * If a shutdown has been triggered, the method will exit fast.
         *
         * @param timeout Time to wait for shutdown.
         */
        void awaitShutdown(std::chrono::milliseconds const& timeout) noexcept {
            if (shouldShutdown()) {
                return;
            }
            std::unique_lock<std::mutex> lock { shutdownLock };
            std::chrono::time_point<std::chrono::steady_clock> limit { std::chrono::steady_clock::now() + timeout };
            shuttingDown.wait_until(lock, limit, [&] { return isShuttingDown.load(); });
        }

        /**
         * Awaits for a shutdown.
         * <p>
         * If a shutdown has been triggered, the method will exit fast.
         */
        void awaitShutdown() noexcept {
            if (shouldShutdown()) {
                return;
            }
            std::unique_lock<std::mutex> lock { shutdownLock };
            shuttingDown.wait(lock, [&] { return isShuttingDown.load(); });
        }

        /**
         * Convenience method to execute an action periodically at the given cadence,
         * until a shutdown is triggered.
         *
         * @param cadence Cadence at which to execute the action.
         * @param action Action to execute.
         */
        void whileLive(std::chrono::milliseconds const& cadence, std::function<void()> const& action) {
            while (!shouldShutdown()) {
                action();
                awaitShutdown(cadence);
            }
        }
    };
}
