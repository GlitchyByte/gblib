// Copyright 2024 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <string>

namespace gb {

    /**
     * False typed RValue. Normally used with atomic compares.
     */
    inline constinit bool _false { false };

    /**
     * True typed RValue. Normally used with atomic compares.
     */
    inline constinit bool _true { true };

    template<typename T>
    concept Numeric = std::integral<T> || std::floating_point<T>;

    template<class TContainer>
    concept HasSize = requires(TContainer tContainer) {
        { tContainer.size() } -> std::convertible_to<size_t>;
    };

    template<class TContainer>
    concept HasIterator = requires(TContainer tContainer) {
        typename TContainer::value_type;
        typename TContainer::iterator;
        typename TContainer::const_iterator;
        std::is_same_v<typename std::iterator_traits<typename TContainer::iterator>::value_type, typename TContainer::value_type>;
        { tContainer.begin() } -> std::input_iterator;
    };

    template<class TContainer>
    concept HasConstValueTypeIterator = requires {
        requires HasIterator<TContainer>;
        requires std::is_same_v<typename TContainer::iterator, typename TContainer::const_iterator>;
    };

    template<class TContainer>
    concept HasNonConstValueTypeIterator = requires {
        requires HasIterator<TContainer>;
        requires !std::is_same_v<typename TContainer::iterator, typename TContainer::const_iterator>;
    };

    template<class TContainer>
    concept HasIndexOperator = requires(TContainer tContainer, size_t index) {
        typename TContainer::value_type;
        { tContainer[index] } -> std::same_as<typename TContainer::value_type&>;
    };

    template<class TContainer>
    concept IterableContainer = HasSize<TContainer> && HasIterator<TContainer>;

    template<class TContainer>
    concept IndexableContainer = HasSize<TContainer> && HasIndexOperator<TContainer>;
}
