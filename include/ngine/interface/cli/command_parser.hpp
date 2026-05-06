#pragma once

#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace ngine {

struct ParsedCommand {
    std::string verb;
    std::vector<std::string> arguments;
    std::unordered_map<std::string, std::string> options;
};

class CommandParser {
public:
    [[nodiscard]] std::optional<ParsedCommand> parse(std::string_view input) const;

private:
    [[nodiscard]] std::vector<std::string> tokenize(std::string_view input) const;
};

}  // namespace ngine
