#pragma once

#include <ngine/commands/command_history.hpp>
#include <ngine/commands/geometry_commands.hpp>
#include <ngine/constraints/constraint_system.hpp>
#include <ngine/core/types.hpp>
#include <ngine/intersection/intersection_engine.hpp>

#include <optional>
#include <unordered_map>
#include <vector>

namespace ngine {

class Document {
public:
    Document();

    EntityId add_entity(GeometryEntity entity);
    void remove_entity(EntityId id);
    [[nodiscard]] GeometryEntity* get_entity(EntityId id);
    [[nodiscard]] const GeometryEntity* get_entity(EntityId id) const;
    [[nodiscard]] std::vector<EntityId> all_entity_ids() const;
    [[nodiscard]] std::size_t entity_count() const noexcept;

    CommandHistory& history() noexcept { return history_; }
    ConstraintSystem& constraints() noexcept { return constraints_; }
    IntersectionEngine& intersections() noexcept { return intersections_; }

    void clear();

private:
    std::unordered_map<EntityId, GeometryEntity> entities_;
    CommandHistory history_;
    ConstraintSystem constraints_;
    IntersectionEngine intersections_;
    EntityId next_id_{1};
};

}  // namespace ngine
