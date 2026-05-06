#include <ngine/core/point.hpp>
#include <ngine/interface/cli/cli_app.hpp>
#include <ngine/interface/cli/command_parser.hpp>
#include <ngine/interface/document.hpp>
#include <ngine/serialization/json_serializer.hpp>

#include <gtest/gtest.h>
#include <memory>
#include <string>

using namespace ngine;

namespace {

class IntegrationTest : public ::testing::Test {
   protected:
    std::shared_ptr<Document> doc = std::make_shared<Document>();
    CliApp app{doc};
    CommandParser parser;
};

TEST_F(IntegrationTest, CreateAndIntersectLines) {
    const auto cmd1 = parser.parse("CREATE_LINE 0 0 10 10");
    const auto cmd2 = parser.parse("CREATE_LINE 0 10 10 0");

    const auto r1 = app.execute_command(*cmd1);
    const auto r2 = app.execute_command(*cmd2);

    EXPECT_TRUE(r1.starts_with("OK"));
    EXPECT_TRUE(r2.starts_with("OK"));

    const auto cmd3 = parser.parse("INTERSECT 1 2");
    const auto r3 = app.execute_command(*cmd3);
    EXPECT_TRUE(r3.find("Intersection") != std::string::npos);
}

TEST_F(IntegrationTest, CreateListDelete) {
    (void)app.execute_command(*parser.parse("CREATE_POINT 1 2"));
    (void)app.execute_command(*parser.parse("CREATE_POINT 3 4"));

    const auto list_result = app.execute_command(*parser.parse("LIST"));
    EXPECT_TRUE(list_result.find('2') != std::string::npos);

    (void)app.execute_command(*parser.parse("DELETE 1"));
    const auto list_after = app.execute_command(*parser.parse("LIST"));
    EXPECT_TRUE(list_after.find('1') != std::string::npos);
    EXPECT_EQ(doc->entity_count(), 1U);
}

TEST_F(IntegrationTest, MoveWithUndoRedo) {
    (void)app.execute_command(*parser.parse("CREATE_POINT 0 0"));
    (void)app.execute_command(*parser.parse("MOVE 1 5 3"));

    auto* entity = doc->get_entity(1);
    auto& point = std::get<Point>(*entity);
    EXPECT_NEAR(point.x(), 5.0, 1e-10);

    (void)app.execute_command(*parser.parse("UNDO"));
    entity = doc->get_entity(1);
    auto& point2 = std::get<Point>(*entity);
    EXPECT_NEAR(point2.x(), 0.0, 1e-10);

    (void)app.execute_command(*parser.parse("REDO"));
    entity = doc->get_entity(1);
    auto& point3 = std::get<Point>(*entity);
    EXPECT_NEAR(point3.x(), 5.0, 1e-10);
}

TEST_F(IntegrationTest, SerializeDeserializeWorkflow) {
    (void)app.execute_command(*parser.parse("CREATE_POINT 1 2"));
    (void)app.execute_command(*parser.parse("CREATE_CIRCLE 0 0 5"));

    const JsonSerializer serializer;
    const auto json = serializer.serialize(*doc);

    const auto loaded = serializer.deserialize(json);
    EXPECT_EQ(loaded->entity_count(), 2U);
}

TEST_F(IntegrationTest, UnknownCommand) {
    auto result = app.execute_command(*parser.parse("FOOBAR"));
    EXPECT_TRUE(result.find("ERROR") != std::string::npos);
}

TEST_F(IntegrationTest, InvalidArguments) {
    auto result = app.execute_command(*parser.parse("CREATE_POINT"));
    EXPECT_TRUE(result.find("ERROR") != std::string::npos);
}

}  // namespace
