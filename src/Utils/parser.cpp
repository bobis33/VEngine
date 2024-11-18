#include <iostream>
#include <sstream>

#include "VEngine/Utils/Parser.hpp"

void ven::Parser::handleLongOption(const std::string_view& arg, const std::vector<std::string_view>& argv, size_t& index) {
    const auto key = arg.substr(2);
    if (!isValidOption(key)) {
        throw std::invalid_argument("Unknown option: --" + std::string(key));
    }

    index + 1 < argv.size() && !argv[index + 1].starts_with('-') ? FUNCTION_MAP_OPT_LONG.at(key)(m_config, argv[++index]) : FUNCTION_MAP_OPT_LONG.at(key)(m_config, "");
}

void ven::Parser::handleShortOptions(const std::string_view& arg) {
    const auto key = arg.substr(1);
    for (size_t j = 0; j < key.length(); ++j) {
        const std::string_view singleOpt = key.substr(j, 1);

        if (!isValidOption(singleOpt)) {
            throw std::invalid_argument("Unknown option: -" + std::string(singleOpt));
        }
        FUNCTION_MAP_OPT_SHORT.at(singleOpt)(m_config);
    }
}

void ven::Parser::parseEnv(const std::unordered_map<std::string, std::string>& envp)
{
#ifdef  __linux__

    if (const auto display = envp.find("DISPLAY"); display == envp.end()) {
        throw std::runtime_error("DISPLAY environment variable not set");
    }

#endif
}

void ven::Parser::parseArgs(const std::vector<std::string_view>& argv)
{
    for (size_t i = 0; i < argv.size(); ++i) {
        const std::string_view& arg = argv[i];
        if (arg.empty()) { continue; }

        if (arg.starts_with("--")) {
            handleLongOption(arg, argv, i);
        } else if (arg.starts_with('-')) {
            handleShortOptions(arg);
        } else {
            throw std::invalid_argument("Unknown option: " + std::string(arg));
        }
    }
}

ven::Parser::Parser(const int argc, char* argv[], char* envp[])
{
    if (envp == nullptr) {
        throw std::runtime_error("cannot access environment variables");
    }

    std::unordered_map<std::string, std::string> envVariables;
    for (int i = 0; envp[i] != nullptr; ++i) {
        const std::string env(envp[i]);
        if (const auto pos = env.find('='); pos != std::string_view::npos) {
            envVariables.insert({ env.substr(0, pos), env.substr(pos + 1) });
        }
    }
    parseEnv(envVariables);

    if (argc > 1) {
        parseArgs(std::vector<std::string_view>(argv + 1, argv + argc));
    }
}
