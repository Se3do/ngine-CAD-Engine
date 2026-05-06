#include <ngine/commands/geometry_commands.hpp>
#include <ngine/core/transform.hpp>

#include <stdexcept>

namespace ngine {

namespace {

void apply_move_to_entity(GeometryEntity* entity, const Vector2D& offset) {
    std::visit(
        [&offset](auto& e) {
            using T = std::decay_t<decltype(e)>;
            if constexpr (std::is_same_v<T, Point>) {
                e.set_position(e.x() + offset.x(), e.y() + offset.y());
            } else if constexpr (std::is_same_v<T, Circle>) {
                Point new_center(e.center().x() + offset.x(), e.center().y() + offset.y());
                e.set_center(new_center);
            }
        },
        *entity);
}

void apply_transform_to_entity(GeometryEntity* entity, const Transform& t) {
    std::visit(
        [&t](auto& e) {
            using T = std::decay_t<decltype(e)>;
            if constexpr (std::is_same_v<T, Point>) {
                Point result = t.apply(e);
                e.set_position(result.x(), result.y());
            } else if constexpr (std::is_same_v<T, Circle>) {
                Point new_center = t.apply(e.center());
                e.set_center(new_center);
            }
        },
        *entity);
}

}  // namespace

// --- CreateCommand ---

CreateCommand::CreateCommand(GeometryEntity entity, AddEntityFn add_fn, RemoveEntityFn remove_fn)
    : entity_(std::move(entity)), add_fn_(std::move(add_fn)), remove_fn_(std::move(remove_fn)) {}

void CreateCommand::execute() {
    created_id_ = add_fn_(entity_);
}

void CreateCommand::undo() {
    remove_fn_(created_id_);
}

std::string CreateCommand::description() const {
    return "Create geometry entity";
}

// --- DeleteCommand ---

DeleteCommand::DeleteCommand(EntityId id, GetEntityFn get_fn, AddEntityFn add_fn,
                             RemoveEntityFn remove_fn)
    : id_(id),
      get_fn_(std::move(get_fn)),
      add_fn_(std::move(add_fn)),
      remove_fn_(std::move(remove_fn)) {}

void DeleteCommand::execute() {
    auto* entity = get_fn_(id_);
    if (!entity) {
        throw std::runtime_error("Entity not found for deletion");
    }
    saved_entity_ = *entity;
    remove_fn_(id_);
}

void DeleteCommand::undo() {
    add_fn_(saved_entity_);
}

std::string DeleteCommand::description() const {
    return "Delete geometry entity";
}

// --- MoveCommand ---

MoveCommand::MoveCommand(EntityId id, Vector2D offset, GetEntityFn get_fn)
    : id_(id), offset_(offset), get_fn_(std::move(get_fn)) {}

void MoveCommand::execute() {
    auto* entity = get_fn_(id_);
    if (!entity) {
        throw std::runtime_error("Entity not found for move");
    }
    apply_move_to_entity(entity, offset_);
}

void MoveCommand::undo() {
    auto* entity = get_fn_(id_);
    if (!entity) {
        throw std::runtime_error("Entity not found for undo move");
    }
    apply_move_to_entity(entity, -offset_);
}

std::string MoveCommand::description() const {
    return "Move geometry entity";
}

// --- TransformCommand ---

TransformCommand::TransformCommand(EntityId id, Transform transform, GetEntityFn get_fn)
    : id_(id), transform_(transform), inverse_(transform.inverse()), get_fn_(std::move(get_fn)) {}

void TransformCommand::execute() {
    auto* entity = get_fn_(id_);
    if (!entity) {
        throw std::runtime_error("Entity not found for transform");
    }
    apply_transform_to_entity(entity, transform_);
}

void TransformCommand::undo() {
    auto* entity = get_fn_(id_);
    if (!entity) {
        throw std::runtime_error("Entity not found for undo transform");
    }
    apply_transform_to_entity(entity, inverse_);
}

std::string TransformCommand::description() const {
    return "Transform geometry entity";
}

}  // namespace ngine
