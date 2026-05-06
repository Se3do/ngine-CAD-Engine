#pragma once

#include <ngine/commands/command.hpp>

#include <cstddef>
#include <deque>
#include <memory>

namespace ngine {

class CommandHistory {
public:
    explicit CommandHistory(std::size_t max_depth = 100);

    void execute(std::unique_ptr<Command> cmd);
    bool undo();
    bool redo();

    [[nodiscard]] bool can_undo() const noexcept;
    [[nodiscard]] bool can_redo() const noexcept;
    [[nodiscard]] std::size_t undo_depth() const noexcept;
    [[nodiscard]] std::size_t redo_depth() const noexcept;

    void clear();

private:
    std::deque<std::unique_ptr<Command>> undo_stack_;
    std::deque<std::unique_ptr<Command>> redo_stack_;
    std::size_t max_depth_;
};

}  // namespace ngine
