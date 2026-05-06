#pragma once

#include <ngine/commands/command.hpp>
#include <ngine/core/circle.hpp>
#include <ngine/core/line.hpp>
#include <ngine/core/point.hpp>
#include <ngine/core/polygon.hpp>
#include <ngine/core/segment.hpp>
#include <ngine/core/transform.hpp>
#include <ngine/core/types.hpp>

#include <functional>
#include <memory>
#include <variant>

namespace ngine {

using GeometryEntity = std::variant<Point, Line, Segment, Circle, Polygon>;

using AddEntityFn = std::function<EntityId(GeometryEntity)>;
using RemoveEntityFn = std::function<void(EntityId)>;
using GetEntityFn = std::function<GeometryEntity*(EntityId)>;

class CreateCommand final : public Command {
   public:
    CreateCommand(GeometryEntity entity, AddEntityFn add_fn, RemoveEntityFn remove_fn);

    void execute() override;
    void undo() override;
    [[nodiscard]] std::string description() const override;

   private:
    GeometryEntity entity_;
    EntityId created_id_{0};
    AddEntityFn add_fn_;
    RemoveEntityFn remove_fn_;
};

class DeleteCommand final : public Command {
   public:
    DeleteCommand(EntityId id, GetEntityFn get_fn, AddEntityFn add_fn, RemoveEntityFn remove_fn);

    void execute() override;
    void undo() override;
    [[nodiscard]] std::string description() const override;

   private:
    EntityId id_;
    GeometryEntity saved_entity_;
    GetEntityFn get_fn_;
    AddEntityFn add_fn_;
    RemoveEntityFn remove_fn_;
};

class MoveCommand final : public Command {
   public:
    MoveCommand(EntityId id, Vector2D offset, GetEntityFn get_fn);

    void execute() override;
    void undo() override;
    [[nodiscard]] std::string description() const override;

   private:
    EntityId id_;
    Vector2D offset_;
    GetEntityFn get_fn_;
};

class TransformCommand final : public Command {
   public:
    TransformCommand(EntityId id, Transform transform, GetEntityFn get_fn);

    void execute() override;
    void undo() override;
    [[nodiscard]] std::string description() const override;

   private:
    EntityId id_;
    Transform transform_;
    Transform inverse_;
    GetEntityFn get_fn_;
};

}  // namespace ngine
