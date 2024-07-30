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
            return [=, this]() {
                return std::generate_canonical<T>(engine);
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
            return [=, this]() {
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
            return [=, this]() {
                std::uniform_real_distribution<T> distribution { low, high };
                return distribution(engine);
            };
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
