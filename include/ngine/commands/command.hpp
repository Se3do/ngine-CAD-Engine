#pragma once

#include <string>

namespace ngine {

class Command {
public:
    virtual ~Command() = default;

    virtual void execute() = 0;
    virtual void undo() = 0;

    [[nodiscard]] virtual std::string description() const = 0;
    [[nodiscard]] virtual bool is_reversible() const noexcept { return true; }
};

}  // namespace ngine
