///
/// @file Parser.hpp
/// @brief This file contains the Parser class
/// @namespace ven
///

#pragma once

#include <algorithm>
#include <functional>
#include <unordered_map>
#include <vector>

#include "VEngine/Utils/Config.hpp"
#include "VEngine/Utils/Message.hpp"

namespace ven {

    ///
    /// @class ParserException
    /// @brief Custom exception class for parsing errors.
    /// @namespace ven
    ///
    class ParserException final : public std::exception {

        public:

            explicit ParserException(std::string msg) : m_msg{std::move(msg)} {};
            ~ParserException() override = default;

            ParserException(const ParserException&) = delete;
            ParserException& operator=(const ParserException&) = delete;
            ParserException(ParserException&&) = delete;
            ParserException& operator=(ParserException&&) = delete;

            [[nodiscard]] const char *what() const noexcept override { return m_msg.c_str(); };

        private:

            std::string m_msg{0};

    }; // class ParserException

    inline bool isNumeric(const std::string_view str) { return !str.empty() && std::ranges::all_of(str, ::isdigit); }

    static const std::unordered_map<std::string_view, std::function<void(Config& conf, std::string_view arg)>> FUNCTION_MAP_OPT_LONG = {
        { "help", [](Config& conf, std::string_view arg) { std::cout << HELP_MESSAGE; throw ParserException(""); } },
        { "version", [](Config& conf, std::string_view arg) { std::cout << VERSION_MESSAGE; throw ParserException(""); } },
        { "width", [](Config& conf, const std::string_view arg)
        {
            if (!isNumeric(arg)) {
                throw std::invalid_argument("Invalid value for width: " + std::string(arg));
            }
            conf.window.width = std::stoi(std::string(arg));
        } },
        { "height", [](Config& conf, const std::string_view arg)
        {
            if (!isNumeric(arg)) {
                throw std::invalid_argument("Invalid value for height: " + std::string(arg));
            }
            conf.window.height = std::stoi(std::string(arg));
        } },
        { "fullscreen", [](Config& conf, std::string_view arg) { conf.window.fullscreen = true; } },
        { "vsync", [](Config& conf, std::string_view arg) { conf.vsync = true; } },
        { "fov", [](Config& conf, const std::string_view arg)
        {
            if (!isNumeric(arg)) {
                throw std::invalid_argument("Invalid value for fov: " + std::string(arg));
            }
            const float value = std::stof(std::string(arg));
            if (value < 1.0F || value > 300.0F) {
                throw std::out_of_range("Field of view must be between 1 and 300");
            }
            conf.camera.fov = value;
        } },
        { "mspeed", [](Config& conf, const std::string_view arg)
        {
            if (!isNumeric(arg)) {
                throw std::invalid_argument("Invalid value for move speed: " + std::string(arg));
            }
            const float value = std::stof(std::string(arg));
            if (value < 0.1F || value > 100.0F) {
                throw std::out_of_range("Move speed must be between 0.1 and 100.0");
            }
            conf.camera.move_speed = value;
        } },
        { "lspeed", [](Config& conf, const std::string_view arg)
        {
            if (!isNumeric(arg)) {
                throw std::invalid_argument("Invalid value for look speed: " + std::string(arg));
            }
            const float value = std::stof(std::string(arg));
            if (value < 0.1F || value > 100.0F) {
                throw std::out_of_range("Look speed must be between 0.1 and 100.0");
            }
            conf.camera.look_speed = value;
        } },
        { "far", [](Config& conf, const std::string_view arg)
        {
            if (!isNumeric(arg)) {
                throw std::invalid_argument("Invalid value for far: " + std::string(arg));
            }
            const float value = std::stof(std::string(arg));
            if (value < 0.1F || value > 100.0F) {
                throw std::out_of_range("Far plane must be between 0.1 and 100.0");
            }
            conf.camera.far = value;
        } },
        { "near", [](Config& conf, const std::string_view arg)
        {
            if (!isNumeric(arg)) {
                throw std::invalid_argument("Invalid value for near: " + std::string(arg));
            }
            const float value = std::stof(std::string(arg));
            if (value < 0.1F || value > 100.0F) {
                throw std::out_of_range("Near plane must be between 0.1 and 100.0");
            }
            conf.camera.near = value;
        } }
    };

    static const std::unordered_map<std::string_view, std::function<void(Config& conf)>> FUNCTION_MAP_OPT_SHORT = {
        { "h", [](Config& conf) { std::cout << HELP_MESSAGE; throw ParserException(""); } },
        { "v", [](Config& conf) { std::cout << VERSION_MESSAGE; throw ParserException(""); } },
        { "f", [](Config& conf) { conf.window.fullscreen = true; } },
        { "V", [](Config& conf) { conf.vsync = true; } }
    };

    ///
    /// @class Parser
    /// @brief Class for Parser
    /// @namespace ven
    ///
    class Parser {

        public:

            Parser(int argc, char* argv[], char* envp[]);
            ~Parser() = default;

            Parser(const Parser&) = delete;
            Parser& operator=(const Parser&) = delete;
            Parser(Parser&&) = delete;
            Parser& operator=(Parser&&) = delete;

            void printArguments() const;

            [[nodiscard]] Config getConfig() const { return m_config; }

        private:

            Config m_config;
            bool m_state = true;

            void parseArgs(const std::vector<std::string_view>& argv);
            void parseEnv(const std::unordered_map<std::string_view, std::string_view>& envp);

            void handleLongOption(const std::string_view& arg, const std::vector<std::string_view>& argv, size_t& index);
            void handleShortOptions(const std::string_view& arg, const std::vector<std::string_view>& argv, size_t& index);

            [[nodiscard]] static bool isValidOption(const std::string_view& option) { return FUNCTION_MAP_OPT_LONG.contains(option) || FUNCTION_MAP_OPT_SHORT.contains(option); }

    }; // class Parser

} // namespace ven
