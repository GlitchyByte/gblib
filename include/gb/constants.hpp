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
}
