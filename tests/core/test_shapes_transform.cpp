#include <ngine/core/circle.hpp>
#include <ngine/core/polygon.hpp>
#include <ngine/core/transform.hpp>

#include <cmath>
#include <gtest/gtest.h>
#include <numbers>

using namespace ngine;

// --- Circle Tests ---

TEST(CircleTest, Construction) {
    Circle c(Point(1, 2), 5.0);
    EXPECT_DOUBLE_EQ(c.center().x(), 1.0);
    EXPECT_DOUBLE_EQ(c.center().y(), 2.0);
    EXPECT_DOUBLE_EQ(c.radius(), 5.0);
}

TEST(CircleTest, InvalidRadiusThrows) {
    EXPECT_THROW(Circle(Point(0, 0), 0.0), std::invalid_argument);
    EXPECT_THROW(Circle(Point(0, 0), -1.0), std::invalid_argument);
}

TEST(CircleTest, Area) {
    Circle c(Point(0, 0), 1.0);
    EXPECT_NEAR(c.area(), std::numbers::pi, 1e-10);
}

TEST(CircleTest, Circumference) {
    Circle c(Point(0, 0), 1.0);
    EXPECT_NEAR(c.circumference(), 2.0 * std::numbers::pi, 1e-10);
}

TEST(CircleTest, Contains) {
    Circle c(Point(0, 0), 5.0);
    EXPECT_TRUE(c.contains(Point(0, 0)));
    EXPECT_TRUE(c.contains(Point(3, 4)));
    EXPECT_FALSE(c.contains(Point(4, 4)));
}

TEST(CircleTest, OnBoundary) {
    Circle c(Point(0, 0), 5.0);
    EXPECT_TRUE(c.on_boundary(Point(5, 0)));
    EXPECT_TRUE(c.on_boundary(Point(3, 4)));
    EXPECT_FALSE(c.on_boundary(Point(0, 0)));
}

TEST(CircleTest, PointAtAngle) {
    Circle c(Point(0, 0), 1.0);
    auto p = c.point_at_angle(0.0);
    EXPECT_NEAR(p.x(), 1.0, 1e-10);
    EXPECT_NEAR(p.y(), 0.0, 1e-10);

    auto p2 = c.point_at_angle(std::numbers::pi / 2.0);
    EXPECT_NEAR(p2.x(), 0.0, 1e-10);
    EXPECT_NEAR(p2.y(), 1.0, 1e-10);
}

// --- Polygon Tests ---

TEST(PolygonTest, Triangle) {
    Polygon tri({Point(0, 0), Point(4, 0), Point(0, 3)});
    EXPECT_EQ(tri.vertex_count(), 3u);
    EXPECT_NEAR(tri.area(), 6.0, 1e-10);
}

TEST(PolygonTest, TooFewVerticesThrows) {
    EXPECT_THROW(Polygon({Point(0, 0), Point(1, 1)}), std::invalid_argument);
}

TEST(PolygonTest, Square) {
    Polygon sq({Point(0, 0), Point(2, 0), Point(2, 2), Point(0, 2)});
    EXPECT_NEAR(sq.area(), 4.0, 1e-10);
    EXPECT_NEAR(sq.perimeter(), 8.0, 1e-10);
    EXPECT_TRUE(sq.is_convex());
}

TEST(PolygonTest, ContainsPoint) {
    Polygon sq({Point(0, 0), Point(10, 0), Point(10, 10), Point(0, 10)});
    EXPECT_TRUE(sq.contains(Point(5, 5)));
    EXPECT_FALSE(sq.contains(Point(15, 5)));
    EXPECT_FALSE(sq.contains(Point(-1, -1)));
}

TEST(PolygonTest, Centroid) {
    Polygon sq({Point(0, 0), Point(4, 0), Point(4, 4), Point(0, 4)});
    auto c = sq.centroid();
    EXPECT_NEAR(c.x(), 2.0, 1e-10);
    EXPECT_NEAR(c.y(), 2.0, 1e-10);
}

TEST(PolygonTest, NonConvex) {
    Polygon concave({Point(0, 0), Point(4, 0), Point(4, 4), Point(2, 2), Point(0, 4)});
    EXPECT_FALSE(concave.is_convex());
}

// --- Transform Tests ---

TEST(TransformTest, Identity) {
    auto t = Transform::identity();
    Point p(3, 4);
    auto result = t.apply(p);
    EXPECT_NEAR(result.x(), 3.0, 1e-10);
    EXPECT_NEAR(result.y(), 4.0, 1e-10);
}

TEST(TransformTest, Translation) {
    auto t = Transform::translation(5, -3);
    Point p(1, 2);
    auto result = t.apply(p);
    EXPECT_NEAR(result.x(), 6.0, 1e-10);
    EXPECT_NEAR(result.y(), -1.0, 1e-10);
}

TEST(TransformTest, Rotation90) {
    auto t = Transform::rotation(std::numbers::pi / 2.0);
    Point p(1, 0);
    auto result = t.apply(p);
    EXPECT_NEAR(result.x(), 0.0, 1e-10);
    EXPECT_NEAR(result.y(), 1.0, 1e-10);
}

TEST(TransformTest, RotationAroundCenter) {
    auto t = Transform::rotation(std::numbers::pi, Point(1, 1));
    Point p(2, 1);
    auto result = t.apply(p);
    EXPECT_NEAR(result.x(), 0.0, 1e-10);
    EXPECT_NEAR(result.y(), 1.0, 1e-10);
}

TEST(TransformTest, Scale) {
    auto t = Transform::scale(2.0, 3.0);
    Point p(1, 1);
    auto result = t.apply(p);
    EXPECT_NEAR(result.x(), 2.0, 1e-10);
    EXPECT_NEAR(result.y(), 3.0, 1e-10);
}

TEST(TransformTest, Compose) {
    auto t1 = Transform::translation(1, 0);
    auto t2 = Transform::translation(0, 1);
    auto composed = t1.compose(t2);
    Point p(0, 0);
    auto result = composed.apply(p);
    EXPECT_NEAR(result.x(), 1.0, 1e-10);
    EXPECT_NEAR(result.y(), 1.0, 1e-10);
}

TEST(TransformTest, Inverse) {
    auto t = Transform::translation(3, 4);
    auto inv = t.inverse();
    Point p(5, 6);
    auto result = inv.apply(t.apply(p));
    EXPECT_NEAR(result.x(), 5.0, 1e-10);
    EXPECT_NEAR(result.y(), 6.0, 1e-10);
}
