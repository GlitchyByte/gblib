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
         * <p>If container is empty, behavior is undefined. Don't call with an empty container!
         *
         * @tparam TContainer Container type.
         * @param container The container from which to pick an item.
         * @return A reference to the randomly picked item.
         */
        template<IndexableContainer TContainer>
        TContainer::value_type& pickFromContainer(TContainer& container) noexcept {
            size_t const index { createContainerIndexGenerator(container)() };
            return container[index];
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
            requires IterableContainer<TContainer> && (!HasIndexOperator<TContainer>) && HasNonConstValueTypeIterator<TContainer>
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
            requires IterableContainer<TContainer> && (!HasIndexOperator<TContainer>) && HasConstValueTypeIterator<TContainer>
        TContainer::value_type const& pickFromContainer(TContainer& container) noexcept {
            size_t const index { createContainerIndexGenerator(container)() };
            auto it { container.begin() };
            std::advance(it, index);
            return *it;
        }

        /**
         * Picks a random index given ordered weighted probabilities.
         *
         * @tparam TWP Weighted probability type.
         * @tparam N Number of weights.
         * @param wps Weighted probabilities.
         * @return A random index.
         */
        template<Numeric TWP, size_t N>
        size_t pickIndexFromWeightedProbability(const TWP (&wps)[N]) noexcept {
            std::array<TWP, N> container;
            for (size_t i = 0; i < N; ++i) {
                container[i] = wps[i];
            }
            return pickIndexFromWeightedProbability(container);
        }

        /**
         * Picks a random index given ordered weighted probabilities.
         *
         * @tparam TContainer Container type.
         * @param wps Weighted probabilities.
         * @return A random index.
         */
        template<class TContainer>
            requires IndexableContainer<TContainer> && Numeric<typename TContainer::value_type>
        size_t pickIndexFromWeightedProbability(TContainer const& wps) noexcept {
            size_t const count { wps.size() };
            std::vector<double_t> calculatedWeights(count);
            double_t total { 0 };
            for (size_t index = 0; index < count; ++index) {
                double_t const w { static_cast<double_t>(wps[index]) };
                total += w;
                calculatedWeights[index] = total;
            }
            double_t const r { createFloatGenerator(0.0, total)() };
            for (size_t index = 0; index < count; ++index) {
                if (r < calculatedWeights[index]) {
                    return index;
                }
            }
            // This will only happen if there is a rounding error.
            return count - 1;
        }

        /**
         * Picks a random index given ordered weighted probabilities.
         *
         * <p>This version accepts zeroes and negative values, and normalizes values to be able to
         * pick an index.
         *
         * @tparam TWP Weighted probability type.
         * @tparam N Number of weights.
         * @param wps Weighted probabilities.
         * @return A random index.
         */
        template<Numeric TWP, size_t N>
        size_t pickIndexFromNormalizedWeightedProbability(const TWP (&wps)[N]) noexcept {
            std::array<TWP, N> container;
            for (size_t i = 0; i < N; ++i) {
                container[i] = wps[i];
            }
            return pickIndexFromNormalizedWeightedProbability(container);
        }

        /**
         * Picks a random index given ordered weighted probabilities.
         *
         * <p>This version accepts zeroes and negative values, and normalizes values to be able to
         * pick an index.
         *
         * @tparam TContainer Container type.
         * @param wps Weighted probabilities.
         * @return A random index.
         */
        template<class TContainer>
            requires IterableContainer<TContainer> && IndexableContainer<TContainer> && Numeric<typename TContainer::value_type>
        size_t pickIndexFromNormalizedWeightedProbability(TContainer const& wps) noexcept {
            typedef typename TContainer::value_type TWP;
            size_t const count { wps.size() };
            bool hasZero { false };
            bool hasAbsMin { false };
            TWP absMin { 0 };
            bool hasAbsLowestNegative { false };
            TWP absLowestNegative { 0 };
            for (TWP const wp: wps) {
                if (wp == 0) {
                    hasZero = true;
                } else {
                    TWP value { abs(wp) };
                    if (hasAbsMin) {
                        if (value < absMin) {
                            absMin = value;
                        }
                    } else {
                        absMin = value;
                        hasAbsMin = true;
                    }
                    if (wp < 0) {
                        if (hasAbsLowestNegative) {
                            if (value > absLowestNegative) {
                                absLowestNegative = value;
                            }
                        } else {
                            absLowestNegative = value;
                            hasAbsLowestNegative = true;
                        }
                    }
                }
            }
            if (hasZero && !hasAbsMin && !hasAbsLowestNegative) {
                return createIntGenerator(static_cast<size_t>(0), count - 1)();
            }
            TWP const normalizer { hasAbsLowestNegative ? absMin + absLowestNegative : hasZero && hasAbsMin ? absMin : 0 };
            std::vector<double_t> normalized(count);
            for (size_t index = 0; index < count; ++index) {
                normalized[index] = normalizer + wps[index];
            }
            return pickIndexFromWeightedProbability(normalized);
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
