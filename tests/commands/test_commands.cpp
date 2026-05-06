#include <ngine/commands/command_history.hpp>
#include <ngine/commands/geometry_commands.hpp>
#include <ngine/interface/document.hpp>

#include <gtest/gtest.h>

using namespace ngine;

class CommandTest : public ::testing::Test {
protected:
    std::shared_ptr<Document> doc = std::make_shared<Document>();
};

TEST_F(CommandTest, UndoRedoEmpty) {
    EXPECT_FALSE(doc->history().can_undo());
    EXPECT_FALSE(doc->history().can_redo());
    EXPECT_FALSE(doc->history().undo());
    EXPECT_FALSE(doc->history().redo());
}

TEST_F(CommandTest, MoveUndoRedo) {
    EntityId id = doc->add_entity(Point(0.0, 0.0));

    auto get_fn = [this](EntityId eid) -> GeometryEntity* { return doc->get_entity(eid); };
    auto cmd = std::make_unique<MoveCommand>(id, Vector2D(5.0, 3.0), get_fn);
    doc->history().execute(std::move(cmd));

    auto* entity = doc->get_entity(id);
    auto& point = std::get<Point>(*entity);
    EXPECT_NEAR(point.x(), 5.0, 1e-10);
    EXPECT_NEAR(point.y(), 3.0, 1e-10);

    doc->history().undo();
    entity = doc->get_entity(id);
    auto& point_after_undo = std::get<Point>(*entity);
    EXPECT_NEAR(point_after_undo.x(), 0.0, 1e-10);
    EXPECT_NEAR(point_after_undo.y(), 0.0, 1e-10);

    doc->history().redo();
    entity = doc->get_entity(id);
    auto& point_after_redo = std::get<Point>(*entity);
    EXPECT_NEAR(point_after_redo.x(), 5.0, 1e-10);
    EXPECT_NEAR(point_after_redo.y(), 3.0, 1e-10);
}

TEST_F(CommandTest, HistoryDepth) {
    CommandHistory history(3);

    auto get_fn = [this](EntityId eid) -> GeometryEntity* { return doc->get_entity(eid); };
    EntityId id = doc->add_entity(Point(0.0, 0.0));

    for (int i = 0; i < 5; ++i) {
        history.execute(
            std::make_unique<MoveCommand>(id, Vector2D(1.0, 0.0), get_fn));
    }

    EXPECT_EQ(history.undo_depth(), 3u);
}

TEST_F(CommandTest, RedoClearedOnNewCommand) {
    auto get_fn = [this](EntityId eid) -> GeometryEntity* { return doc->get_entity(eid); };
    EntityId id = doc->add_entity(Point(0.0, 0.0));

    doc->history().execute(
        std::make_unique<MoveCommand>(id, Vector2D(1.0, 0.0), get_fn));
    doc->history().execute(
        std::make_unique<MoveCommand>(id, Vector2D(1.0, 0.0), get_fn));

    doc->history().undo();
    EXPECT_TRUE(doc->history().can_redo());

    doc->history().execute(
        std::make_unique<MoveCommand>(id, Vector2D(0.0, 1.0), get_fn));
    EXPECT_FALSE(doc->history().can_redo());
}
