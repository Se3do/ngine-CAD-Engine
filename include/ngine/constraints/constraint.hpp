#pragma once

#include <ngine/core/types.hpp>

#include <cstdint>
#include <string>
#include <vector>

namespace ngine {

enum class ConstraintStatus : std::uint8_t {
    Satisfied,
    Violated,
    UnderConstrained,
    OverConstrained
};

class Constraint {
public:
    virtual ~Constraint() = default;

    [[nodiscard]] virtual ConstraintStatus evaluate() const = 0;
    [[nodiscard]] virtual Real error() const = 0;
    [[nodiscard]] virtual std::vector<EntityId> involved_entities() const = 0;
    [[nodiscard]] virtual std::string type_name() const = 0;

    virtual void apply_correction(Real step_size) = 0;

    [[nodiscard]] EntityId id() const noexcept { return id_; }

protected:
    EntityId id_{IdGenerator::next()};
};

}  // namespace ngine
