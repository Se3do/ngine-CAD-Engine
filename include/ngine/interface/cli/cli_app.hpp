#pragma once

#include <ngine/interface/cli/command_parser.hpp>
#include <ngine/interface/document.hpp>

#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>

namespace ngine {

class CliApp {
public:
    explicit CliApp(std::shared_ptr<Document> document);

    void run(std::istream& input = std::cin, std::ostream& output = std::cout);
    [[nodiscard]] std::string execute_command(const ParsedCommand& cmd);

private:
    using Handler = std::function<std::string(const ParsedCommand&)>;

    void register_handlers();

    std::string handle_create_point(const ParsedCommand& cmd);
    std::string handle_create_line(const ParsedCommand& cmd);
    std::string handle_create_circle(const ParsedCommand& cmd);
    std::string handle_create_segment(const ParsedCommand& cmd);
    std::string handle_intersect(const ParsedCommand& cmd);
    std::string handle_list(const ParsedCommand& cmd);
    std::string handle_delete(const ParsedCommand& cmd);
    std::string handle_move(const ParsedCommand& cmd);
    std::string handle_undo(const ParsedCommand& cmd);
    std::string handle_redo(const ParsedCommand& cmd);
    std::string handle_solve(const ParsedCommand& cmd);
    std::string handle_help(const ParsedCommand& cmd);

    std::shared_ptr<Document> document_;
    CommandParser parser_;
    std::unordered_map<std::string, Handler> handlers_;
};

}  // namespace ngine
