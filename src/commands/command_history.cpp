#include <ngine/commands/command_history.hpp>

namespace ngine {

CommandHistory::CommandHistory(std::size_t max_depth) : max_depth_(max_depth) {}

void CommandHistory::execute(std::unique_ptr<Command> cmd) {
    cmd->execute();
    undo_stack_.push_back(std::move(cmd));
    redo_stack_.clear();

    if (undo_stack_.size() > max_depth_) {
        undo_stack_.pop_front();
    }
}

bool CommandHistory::undo() {
    if (undo_stack_.empty()) {
        return false;
    }

    auto cmd = std::move(undo_stack_.back());
    undo_stack_.pop_back();
    cmd->undo();
    redo_stack_.push_back(std::move(cmd));
    return true;
}

bool CommandHistory::redo() {
    if (redo_stack_.empty()) {
        return false;
    }

    auto cmd = std::move(redo_stack_.back());
    redo_stack_.pop_back();
    cmd->execute();
    undo_stack_.push_back(std::move(cmd));
    return true;
}

bool CommandHistory::can_undo() const noexcept {
    return !undo_stack_.empty();
}

bool CommandHistory::can_redo() const noexcept {
    return !redo_stack_.empty();
}

std::size_t CommandHistory::undo_depth() const noexcept {
    return undo_stack_.size();
}

std::size_t CommandHistory::redo_depth() const noexcept {
    return redo_stack_.size();
}

void CommandHistory::clear() {
    undo_stack_.clear();
    redo_stack_.clear();
}

}  // namespace ngine
