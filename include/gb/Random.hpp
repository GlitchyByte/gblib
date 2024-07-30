// Copyright 2024 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cmath>
#include <random>
#include <functional>

namespace gb {

    template<typename T>
    concept IntType = std::is_integral_v<T>;

    template<typename T>
    concept FloatType = std::is_floating_point_v<T>;

    class Random {
    private:
        std::random_device device;
        std::mt19937 engine { device() };

    public:
        /**
         * Creates a random number generator function that generates integer numbers [low, high].
         *
         * @tparam T Integer type.
         * @param low Range low boundary, inclusive.
         * @param high Range high boundary, inclusive.
         * @return A function that generates random numbers in the range.
         */
        template<IntType T>
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
        template<FloatType T>
        std::function<T()> createFloatGenerator(T const low, T const high) noexcept {
            return [=, this]() {
                std::uniform_real_distribution<T> distribution { low, high };
                return distribution(engine);
            };
        }
    };
}
