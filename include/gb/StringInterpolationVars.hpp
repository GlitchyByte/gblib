// Copyright 2023-2024 GlitchyByte
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <unordered_map>

namespace gb {

    /**
     * Class to hold variable names and their values for string interpolation.
     *
     * <p>Variables in strings are of the form "${myVar}".
     *
     * <p>Set that variable on this object by calling set("myVar", "5").
     */
    class StringInterpolationVars {
    private:
        std::unordered_map<std::string, std::string> vars;

    public:
        /**
         * Creates a template string var container.
         */
        StringInterpolationVars() noexcept = default;

        /**
         * Copy constructor.
         *
         * @param other Other object to copy from.
         */
        StringInterpolationVars(StringInterpolationVars const& other) noexcept {
            vars = other.vars;
        }

        /**
         * Move constructor.
         *
         * @param other Other object to move the contents from.
         */
        StringInterpolationVars(StringInterpolationVars&& other) noexcept {
            vars = std::move(other.vars);
        }

        /**
         * Copy assignment.
         *
         * @param other Other object to copy from.
         * @return This object with new contents.
         */
        StringInterpolationVars& operator=(StringInterpolationVars const& other) noexcept {
            if (this != &other) {
                vars = other.vars;
            }
            return *this;
        }

        /**
         * Move assignment.
         *
         * @param other Other object to move the contents from.
         * @return This object with new contents.
         */
        StringInterpolationVars& operator=(StringInterpolationVars&& other) noexcept {
            if (this != &other) {
                vars = std::move(other.vars);
            }
            return *this;
        }

        /**
         * Sets a template variable and its value.
         *
         * @param name Variable name.
         * @param value Value.
         * @return This object.
         */
        StringInterpolationVars& set(std::string const& name, std::string const& value) noexcept {
            std::string const decoratedName { "${" + name + "}" };
            vars[decoratedName] = value;
            return *this;
        }

        /**
         * Substitutes variables in a string template.
         *
         * @param str String template.
         * @return The interpolated string.
         */
        [[nodiscard]]
        std::string interpolate(std::string_view const& str) const noexcept {
            std::string interpolated { str };
            for (auto const& [ name, value ]: vars) {
                size_t pos { interpolated.find(name) };
                while (pos != std::string::npos) {
                    interpolated.replace(pos, name.length(), value);
                    pos = interpolated.find(name, pos + value.length());
                }
            }
            return interpolated;
        }

        /**
         * String representation.
         *
         * @return The string representation.
         */
        [[nodiscard]]
        std::string string() const noexcept {
            std::ostringstream ss;
            ss << "{ ";
            bool first = true;
            for (auto const& [ name, value ]: vars) {
                if (first) {
                    first = false;
                } else {
                    ss << ", ";
                }
                ss << '"' << name << "\": \"" << value << '"';
            }
            ss << " }";
            return ss.str();
        }
    };
}
