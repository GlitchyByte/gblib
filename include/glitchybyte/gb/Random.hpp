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
         * generates an integer in the range of the vector indexes.
         *
         * <p>If vector is empty, behavior is undefined. Don't call with an empty vector!
         *
         * @tparam TItem The item type.
         * @param vector The vector from which to get the index range.
         * @return A function that generates random numbers in the range.
         */
        template<typename TItem>
        std::function<size_t()> createVectorIndexGenerator(std::vector<TItem> const& vector) noexcept {
            return createIntGenerator<size_t>(0, vector.size() - 1);
        }

        /**
         * This is a convenience method to pick a random item from a vector.
         *
         * <p>If vector is empty, behavior is undefined. Don't call with an empty vector!
         *
         * @tparam TItem The item type.
         * @param vector The vector from which to pick an item.
         * @return A reference to the randomly picked item.
         */
        template<typename TItem>
        TItem& pickFromVector(std::vector<TItem>& vector) noexcept {
            size_t const index { createVectorIndexGenerator(vector)() };
            return vector[index];
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
