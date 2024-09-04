// Copyright 2024 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cmath>
#include <random>
#include <functional>

namespace gb {

    /**
     * Random number function creator.
     *
     * @tparam TEngine Random number engine. (https://en.cppreference.com/w/cpp/numeric/random)
     */
    template<typename TEngine>
    class Random {
    private:
        std::random_device device;
        TEngine engine { device() };

    public:
        /**
         * Creates a random number generator function that generates floating point numbers [0, 1).
         *
         * @tparam T Floating point type.
         * @return A function that generates random numbers in the range.
         */
        template<std::floating_point T>
        std::function<T()> createCanonicalGenerator() noexcept {
            return [=, this]() noexcept {
                return std::generate_canonical<T, std::numeric_limits<T>::digits>(engine);
            };
        }

        /**
         * Creates a random number generator function that generates integer numbers [low, high].
         *
         * @tparam T Integer type.
         * @param low Range low boundary, inclusive.
         * @param high Range high boundary, inclusive.
         * @return A function that generates random numbers in the range.
         */
        template<std::integral T>
        std::function<T()> createIntGenerator(T const low, T const high) noexcept {
            return [=, this]() noexcept {
                std::uniform_int_distribution<T> distribution { low, high };
                return distribution(engine);
            };
        }

        /**
         * Creates a random number generator function that generates floating point numbers [low, high).
         *
         * @tparam T Floating point type.
         * @param low Range low boundary, inclusive.
         * @param high Range high boundary, exclusive.
         * @return A function that generates random numbers in the range.
         */
        template<std::floating_point T>
        std::function<T()> createFloatGenerator(T const low, T const high) noexcept {
            return [=, this]() noexcept {
                std::uniform_real_distribution<T> distribution { low, high };
                return distribution(engine);
            };
        }

        /**
         * This is a convenience method that creates a random number generator function that
         * generates an integer in the range of the container indexes.
         *
         * <p>If container is empty, behavior is undefined. Don't call with an empty container!
         *
         * @tparam TContainer Container type.
         * @param container The container from which to get the index range.
         * @return A function that generates random numbers in the range.
         */
        template<HasSize TContainer>
        std::function<size_t()> createContainerIndexGenerator(TContainer const& container) noexcept {
            return createIntGenerator<size_t>(0, container.size() - 1);
        }

        /**
         * This is a convenience method to pick a random item from a container.
         *
         * <p>This template is for non-const default iterator.
         *
         * <p>If container is empty, behavior is undefined. Don't call with an empty container!
         *
         * @tparam TContainer Container type.
         * @param container The container from which to pick an item.
         * @return A reference to the randomly picked item.
         */
        template<class TContainer>
        requires IterableContainer<TContainer> && HasNonConstValueTypeIterator<TContainer>
        TContainer::value_type& pickFromContainer(TContainer& container) noexcept {
            size_t const index { createContainerIndexGenerator(container)() };
            auto it { container.begin() };
            std::advance(it, index);
            return *it;
        }

        /**
         * This is a convenience method to pick a random item from a container.
         *
         * <p>This template is for const default iterator.
         *
         * <p>If container is empty, behavior is undefined. Don't call with an empty container!
         *
         * @tparam TContainer Container type.
         * @param container The container from which to pick an item.
         * @return A reference to the randomly picked item.
         */
        template<class TContainer>
        requires IterableContainer<TContainer> && HasConstValueTypeIterator<TContainer>
        TContainer::value_type const& pickFromContainer(TContainer& container) noexcept {
            size_t const index { createContainerIndexGenerator(container)() };
            auto it { container.begin() };
            std::advance(it, index);
            return *it;
        }

        /**
         * This is a convenience method to pick a random item from a container.
         *
         * <p>This is a vector specialization.
         *
         * <p>If container is empty, behavior is undefined. Don't call with an empty container!
         *
         * @tparam TItem Item type.
         * @param container The container from which to pick an item.
         * @return A reference to the randomly picked item.
         */
        template<typename TItem>
        TItem& pickFromContainer(std::vector<TItem>& container) noexcept {
            size_t const index { createContainerIndexGenerator(container)() };
            return container[index];
        }
    };

    /**
     * Random number function creator specialization with Mersenne Twister (32 bits) engine.
     */
    class RandomMT : public Random<std::mt19937> {};

    /**
     * Random number function creator specialization with Mersenne Twister (64 bits) engine.
     */
    class RandomMT64 : public Random<std::mt19937_64> {};
}
