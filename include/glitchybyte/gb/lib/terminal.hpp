// Copyright 2023-2024 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "strings.hpp"
#include <string>
#include <cassert>
#include <format>

namespace gb::terminal {

    typedef uint32_t color_t;

    #define GB_TERMINAL_TOKEN "@"
    #define GB_TERMINAL_CSI "\u001b["
    #define GB_TERMINAL_CSI_COLOR_RESET GB_TERMINAL_CSI "0m"
    #define GB_TERMINAL_CSI_COLOR_FOREGROUND GB_TERMINAL_CSI "38;5;" GB_TERMINAL_TOKEN "m"
    #define GB_TERMINAL_CSI_CLEAR_CURRENT_LINE GB_TERMINAL_CSI "2K"
    #define GB_TERMINAL_CSI_CLEAR_TO_END_OF_LINE GB_TERMINAL_CSI "0K"
    #define GB_TERMINAL_CSI_CLEAR_TO_START_OF_LINE GB_TERMINAL_CSI "1K"
    #define GB_TERMINAL_CSI_CURSOR_UP GB_TERMINAL_CSI GB_TERMINAL_TOKEN "A"
    #define GB_TERMINAL_CSI_CURSOR_DOWN GB_TERMINAL_CSI GB_TERMINAL_TOKEN "B"
    #define GB_TERMINAL_CSI_CURSOR_FORWARD GB_TERMINAL_CSI GB_TERMINAL_TOKEN "C"
    #define GB_TERMINAL_CSI_CURSOR_BACK GB_TERMINAL_CSI GB_TERMINAL_TOKEN "D"

    /**
     * Converts rgb values into a single color code.
     *
     * @param r Red value between [0, 6).
     * @param g Green value between [0, 6).
     * @param b Blue value between [0, 6).
     * @return The color code.
     */
    [[nodiscard]]
    static inline constexpr color_t rgb(color_t const r, color_t const g, color_t const b) noexcept {
        assert(r < 6);
        assert(g < 6);
        assert(b < 6);
        return 16 + (36 * r) + (6 * g) + b;
    }

    /**
     * Converts a grey step into a color code.
     *
     * @param step Grey step between [0, 24).
     * @return The color code.
     */
    [[nodiscard]]
    static inline constexpr color_t grey(color_t const step) noexcept {
        assert(step < 24);
        return 232 + step;
    }

    /**
     * Returns a string that represents the given string in the given color.
     *
     * @param str String to apply color.
     * @param color Color.
     * @return The string in color.
     */
    [[nodiscard]]
    static inline constexpr std::string colorText(std::string_view const str, color_t const color) noexcept {
        if (str.empty()) {
            return "";
        }
        auto const startColor { gb::strings::replace(GB_TERMINAL_CSI_COLOR_FOREGROUND, GB_TERMINAL_TOKEN, std::to_string(color)) };
        return std::format("{}{}" GB_TERMINAL_CSI_COLOR_RESET, startColor, str);
    }

    /**
     * Returns a string that clears to end of line.
     *
     * @return The code string.
     */
    [[nodiscard]]
    static inline constexpr std::string clearToEndOfLine() noexcept {
        return GB_TERMINAL_CSI_CLEAR_CURRENT_LINE;
    }

    /**
     * Returns a string that clears to start of line.
     *
     * @return The code string.
     */
    [[nodiscard]]
    static inline constexpr std::string clearToStartOfLine() noexcept {
        return GB_TERMINAL_CSI_CLEAR_TO_START_OF_LINE;
    }

    /**
     * Returns a string that moves the cursor up.
     *
     * @param n Number of times to move cursor.
     * @return The code string.
     */
    [[nodiscard]]
    static inline constexpr std::string cursorUp(uint const n = 1) noexcept {
        return gb::strings::replace(GB_TERMINAL_CSI_CURSOR_UP, GB_TERMINAL_TOKEN, std::to_string(n));
    }

    /**
     * Returns a string that moves the cursor down.
     *
     * @param n Number of times to move cursor.
     * @return The code string.
     */
    [[nodiscard]]
    static inline constexpr std::string cursorDown(uint const n = 1) noexcept {
        return gb::strings::replace(GB_TERMINAL_CSI_CURSOR_DOWN, GB_TERMINAL_TOKEN, std::to_string(n));
    }

    /**
     * Returns a string that moves the cursor forward.
     *
     * @param n Number of times to move cursor.
     * @return The code string.
     */
    [[nodiscard]]
    static inline constexpr std::string cursorForward(uint const n = 1) noexcept {
        return gb::strings::replace(GB_TERMINAL_CSI_CURSOR_FORWARD, GB_TERMINAL_TOKEN, std::to_string(n));
    }

    /**
     * Returns a string that moves the cursor back.
     *
     * @param n Number of times to move cursor.
     * @return The code string.
     */
    [[nodiscard]]
    static inline constexpr std::string cursorBack(uint const n = 1) noexcept {
        return gb::strings::replace(GB_TERMINAL_CSI_CURSOR_BACK, GB_TERMINAL_TOKEN, std::to_string(n));
    }
}
