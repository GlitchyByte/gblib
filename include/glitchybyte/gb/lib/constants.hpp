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
    concept IterableContainer = HasSize<TContainer> && HasIterator<TContainer>;
}
