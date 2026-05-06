#include <ngine/interface/cli/command_parser.hpp>

#include <algorithm>
#include <sstream>

namespace ngine {

std::vector<std::string> CommandParser::tokenize(std::string_view input) const {
    std::vector<std::string> tokens;
    std::string token;
    bool in_quotes = false;

    for (char ch : input) {
        if (ch == '"') {
            in_quotes = !in_quotes;
        } else if (std::isspace(static_cast<unsigned char>(ch)) && !in_quotes) {
            if (!token.empty()) {
                tokens.push_back(std::move(token));
                token.clear();
            }
        } else {
            token += ch;
        }
    }

    if (!token.empty()) {
        tokens.push_back(std::move(token));
    }

    return tokens;
}

std::optional<ParsedCommand> CommandParser::parse(std::string_view input) const {
    auto tokens = tokenize(input);
    if (tokens.empty()) {
        return std::nullopt;
    }

    ParsedCommand cmd;
    cmd.verb = tokens[0];
    std::transform(cmd.verb.begin(), cmd.verb.end(), cmd.verb.begin(),
                   [](unsigned char c) { return static_cast<char>(std::toupper(c)); });

    for (std::size_t i = 1; i < tokens.size(); ++i) {
        if (tokens[i].starts_with("--") && i + 1 < tokens.size()) {
            std::string key = tokens[i].substr(2);
            cmd.options[key] = tokens[++i];
        } else {
            cmd.arguments.push_back(tokens[i]);
        }
    }

    return cmd;
}

}  // namespace ngine
