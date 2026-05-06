#include <ngine/interface/document.hpp>
#include <ngine/serialization/json_serializer.hpp>

#include <gtest/gtest.h>

using namespace ngine;

TEST(SerializationTest, EmptyDocument) {
    Document doc;
    JsonSerializer serializer;

    auto json = serializer.serialize(doc);
    auto loaded = serializer.deserialize(json);

    EXPECT_EQ(loaded->entity_count(), 0u);
}

TEST(SerializationTest, RoundTripPoint) {
    Document doc;
    doc.add_entity(Point(3.14, 2.71));

    JsonSerializer serializer;
    auto json = serializer.serialize(doc);
    auto loaded = serializer.deserialize(json);

    ASSERT_EQ(loaded->entity_count(), 1u);
    auto ids = loaded->all_entity_ids();
    auto* entity = loaded->get_entity(ids[0]);
    ASSERT_NE(entity, nullptr);

    auto& point = std::get<Point>(*entity);
    EXPECT_NEAR(point.x(), 3.14, 1e-10);
    EXPECT_NEAR(point.y(), 2.71, 1e-10);
}

TEST(SerializationTest, RoundTripLine) {
    Document doc;
    doc.add_entity(Line::from_points(Point(0, 0), Point(1, 1)));

    JsonSerializer serializer;
    auto json = serializer.serialize(doc);
    auto loaded = serializer.deserialize(json);

    ASSERT_EQ(loaded->entity_count(), 1u);
    auto ids = loaded->all_entity_ids();
    auto* entity = loaded->get_entity(ids[0]);
    auto& line = std::get<Line>(*entity);
    EXPECT_TRUE(line.contains(Point(5, 5), 1e-8));
}

TEST(SerializationTest, RoundTripCircle) {
    Document doc;
    doc.add_entity(Circle(Point(1, 2), 5.0));

    JsonSerializer serializer;
    auto json = serializer.serialize(doc);
    auto loaded = serializer.deserialize(json);

    ASSERT_EQ(loaded->entity_count(), 1u);
    auto ids = loaded->all_entity_ids();
    auto* entity = loaded->get_entity(ids[0]);
    auto& circle = std::get<Circle>(*entity);
    EXPECT_NEAR(circle.center().x(), 1.0, 1e-10);
    EXPECT_NEAR(circle.center().y(), 2.0, 1e-10);
    EXPECT_NEAR(circle.radius(), 5.0, 1e-10);
}

TEST(SerializationTest, RoundTripMultipleEntities) {
    Document doc;
    doc.add_entity(Point(1, 2));
    doc.add_entity(Point(3, 4));
    doc.add_entity(Circle(Point(0, 0), 1.0));

    JsonSerializer serializer;
    auto json = serializer.serialize(doc);
    auto loaded = serializer.deserialize(json);

    EXPECT_EQ(loaded->entity_count(), 3u);
}

TEST(SerializationTest, RoundTripPolygon) {
    Document doc;
    doc.add_entity(Polygon({Point(0, 0), Point(1, 0), Point(0, 1)}));

    JsonSerializer serializer;
    auto json = serializer.serialize(doc);
    auto loaded = serializer.deserialize(json);

    ASSERT_EQ(loaded->entity_count(), 1u);
    auto ids = loaded->all_entity_ids();
    auto* entity = loaded->get_entity(ids[0]);
    auto& poly = std::get<Polygon>(*entity);
    EXPECT_EQ(poly.vertex_count(), 3u);
}
