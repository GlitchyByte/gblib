// Copyright 2023-2024 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <vector>
#include <set>
#include <sstream>
#include <iomanip>

namespace gb::strings {

    /**
     * Converts a C string array into a vector of string_views.
     *
     * <p>The contents of the vector will be valid as long as the original array is valid.
     *
     * <p>This function is specifically designed to convert main function args into a string vector.
     *
     * @param argc String array size.
     * @param argv Array of C strings.
     * @return A string vector of the arguments.
     */
    constexpr inline std::vector<std::string_view> createVectorStringViewFromCArray(int const argc, char const* argv[]) noexcept {
        size_t const argsSize { static_cast<size_t>(argc) };
        std::vector<std::string_view> args { argsSize };
        for (size_t i = 0; i < argsSize; ++i) {
            args[i] = std::string_view { argv[i] };
        }
        return args;
    }

    /**
     * Converts a C string array into a vector of strings.
     *
     * <p>This function is specifically designed to convert main function args into a string vector.
     *
     * @param argc String array size.
     * @param argv Array of C strings.
     * @return A string vector of the arguments.
     */
    constexpr inline std::vector<std::string> createVectorStringFromCArray(int const argc, char const* argv[]) noexcept {
        size_t const argsSize { static_cast<size_t>(argc) };
        std::vector<std::string> args { argsSize };
        for (size_t i = 0; i < argsSize; ++i) {
            args[i] = std::string { argv[i] };
        }
        return args;
    }

    /**
     * Convenience replace of a token in a string.
     *
     * <p>This creates a new string, so it is only a shortcut, not efficient for replacing many tokens.
     *
     * @param str String to search for token.
     * @param token Token to find.
     * @param value Value to replace the token with.
     * @return The replaced string with the token replaced.
     */
    [[nodiscard]]
    constexpr inline std::string replace(std::string_view const& str, std::string_view const& token, std::string_view const& value) noexcept {
        size_t const pos = { str.find(token) };
        std::string newStr { str };
        if (pos == std::string::npos) {
            return newStr;
        }
        return newStr.replace(pos, token.length(), value);
    }

    /**
     * Convenience replace of a token in a string.
     *
     * <p>This modifies the string,
     *
     * @param str String to search for token.
     * @param token Token to find.
     * @param value Value to replace the token with.
     * @return The replaced string with the token replaced.
     */
    constexpr inline std::string& replaceInPlace(std::string& str, std::string_view const& token, std::string_view const& value) noexcept {
        size_t const pos = { str.find(token) };
        if (pos != std::string::npos) {
            return str.replace(pos, token.length(), value);
        }
        return str;
    }

    /**
     * Splits a string by the delimiter into a vector of strings.
     *
     * <p>Storage is weak. Meaning, the resulting vector is actually pointing at sections in the original
     * string. This is faster than regular split, but if the original string goes away, the contents of this
     * vector are not valid.
     *
     * @param str String to split.
     * @param delimiter Delimiter to split the string.
     * @return A vector of strings.
     */
    [[nodiscard]]
    constexpr inline std::vector<std::string_view> splitWeak(std::string_view const& str, std::string_view const& delimiter) noexcept {
        size_t const delimiterSize { delimiter.size() };
        size_t pos { str.find(delimiter) };
        if ((delimiterSize == 0) || (pos == std::string::npos)) {
            return std::vector<std::string_view> { str };
        }
        size_t start { 0 };
        std::vector<std::string_view> lines;
        while (pos != std::string::npos) {
            lines.emplace_back(str.substr(start, pos - start));
            start = pos + delimiterSize;
            pos = str.find(delimiter, start);
        }
        lines.emplace_back(str.substr(start));
        return lines;
    }

    /**
     * Splits a string by the delimiter into a vector of strings.
     *
     * @param str String to split.
     * @param delimiter Delimiter to split the string.
     * @return A vector of strings.
     */
    [[nodiscard]]
    constexpr inline std::vector<std::string> split(std::string_view const& str, std::string_view const& delimiter) noexcept {
        std::vector<std::string_view> const weak { std::move(splitWeak(str, delimiter)) };
        size_t const weakSize { weak.size() };
        std::vector<std::string> lines { weakSize };
        for (size_t i = 0; i < weakSize; ++i) {
            lines[i] = weak[i];
        }
        return lines;
    }

    /**
     * Unindents a multiline block of text by removing all common spaces or tabs
     * from the beginning of the lines. Empty lines are left alone.
     *
     * <p>This function is specifically designed to use with raw string literals.
     * And it will remove indentation created to align the string with the rest
     * of the surrounding code.
     *
     * <p>Only spaces are considered when recognizing indentation.
     *
     * @param str String to unindent.
     * @return The unindented string.
     */
    [[nodiscard]]
    inline std::string unindent(std::string_view const& str) noexcept {
        if (str.empty()) {
            return "";
        }
        auto lines { std::move(splitWeak(str, "\n")) };
        if (lines[0].empty()) {
            // We allow a 1st new line so that it looks good in code.
            // We remove that extra first line.
            lines.erase(lines.begin());
        }
        // Find indentation size.
        size_t indent { std::string_view::npos };
        std::set<size_t> emptyLines;
        for (int i = 0; i < lines.size(); ++i) {
            auto const& line = lines[i];
            size_t const pos = line.find_first_not_of(' ');
            if (pos == std::string_view::npos) {
                emptyLines.insert(i);
                continue;
            }
            if ((indent == std::string_view::npos) || (pos < indent)) {
                indent = pos;
            }
        }
        if (indent == std::string::npos) {
            indent = 0;
        }
        // Reassemble while removing indentation.
        std::ostringstream ss;
        for (int i = 0; i < lines.size(); ++i) {
            auto const& line = lines[i];
            if (i > 0) {
                ss << '\n';
            }
            if (emptyLines.contains(i)) {
                continue;
            }
            if (indent == 0) {
                ss << line;
            } else {
                ss << line.substr(indent);
            }
        }
        return ss.str();
    }

    /**
     * Joins a string representation of all elements into a string.
     * By default each item is separated by a comma and a space.
     *
     * @tparam T The type of item.
     * @param vector Vector of items to join.
     * @return A joined string of all items.
     */
    template <typename T>
    [[nodiscard]]
    inline std::string fromVector(std::vector<T> const& vector, std::string_view const& separator = ", ") noexcept {
        std::ostringstream ss;
        bool first { true };
        for (const T& item: vector) {
            if (first) {
                first = false;
            } else {
                ss << separator;
            }
            ss << item;
        }
        return ss.str();
    }

    /**
     * Adds thousand separators in the given string that must be a numeric representation.
     *
     * @param str String numeric representation.
     * @return The numeric representation with thousand separators.
     */
    [[nodiscard]]
    constexpr inline std::string addThousandSeparators(std::string_view const& str) noexcept {
        std::string number { str };
        size_t const period { number.find('.') };
        size_t const start { period == std::string::npos ? number.length() : period };
        size_t const finish { static_cast<size_t>(number.starts_with('-') ? 4 : 3) };
        size_t index { start };
        while (index > finish) {
            index -= 3;
            number.insert(index, ",");
        }
        return number;
    }

    /**
     * Returns a string representation of the given time.
     *
     * @param time Time to convert to string.
     * @param format Time format.
     * @return String representation of the given time.
     */
    [[nodiscard]]
    inline std::string fromTime(std::time_t const& time, std::string const& format) noexcept {
        std::stringstream ss;
        ss << std::put_time(std::localtime(&time), format.c_str());
        return ss.str();
    }

    /**
     * Converts an integral value to a human representation.
     *
     * @tparam T Integer type.
     * @param value Value.
     * @param withThousandSeparators True to have thousand separators included in the string.
     * @return String representation of value.
     */
    template <std::integral T>
    constexpr inline std::string fromIntegral(T const value, bool const withThousandSeparators = false) noexcept {
        std::string const str { std::to_string(value) };
        return withThousandSeparators ? addThousandSeparators(str) : str;
    }

    /**
     * fromFloatingPoint precision: Default precision for floating point values.
     */
    constexpr int DefaultPrecision { -1 };

    /**
     * fromFloatingPoint precision: Maximum precision for floating point values.
     */
    constexpr int MaxPrecision { -2 };

    /**
     * Converts a floating point value to a human representation.
     *
     * @tparam T Floating point type.
     * @param value Value.
     * @param precision Number of decimals to show. Default shows all needed.
     * @param withThousandSeparators True to have thousand separators included in the string.
     * @return String representation of value.
     */
    template <std::floating_point T>
    inline std::string fromFloatingPoint(T const value, int const precision = DefaultPrecision, bool const withThousandSeparators = false) noexcept {
        std::string str;
        if (precision == DefaultPrecision) {
            str = std::to_string(value);
        } else {
            std::ostringstream ss;
            if (precision == MaxPrecision) {
                ss << std::fixed << std::setprecision(std::numeric_limits<T>::digits10 + 1) << value;
            } else {
                ss << std::fixed << std::setprecision(precision) << value;
            }
            str = ss.str();
        }
        return withThousandSeparators ? addThousandSeparators(str) : str;
    }
}
