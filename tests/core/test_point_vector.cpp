#include <ngine/core/point.hpp>
#include <ngine/core/types.hpp>
#include <ngine/core/vector2d.hpp>

#include <gtest/gtest.h>

#include <cmath>
#include <numbers>

using namespace ngine;

TEST(Vector2DTest, DefaultConstruction) {
    Vector2D v;
    EXPECT_DOUBLE_EQ(v.x(), 0.0);
    EXPECT_DOUBLE_EQ(v.y(), 0.0);
}

TEST(Vector2DTest, ParameterizedConstruction) {
    Vector2D v(3.0, 4.0);
    EXPECT_DOUBLE_EQ(v.x(), 3.0);
    EXPECT_DOUBLE_EQ(v.y(), 4.0);
}

TEST(Vector2DTest, Length) {
    Vector2D v(3.0, 4.0);
    EXPECT_DOUBLE_EQ(v.length(), 5.0);
    EXPECT_DOUBLE_EQ(v.length_squared(), 25.0);
}

TEST(Vector2DTest, Normalized) {
    Vector2D v(3.0, 4.0);
    auto n = v.normalized();
    EXPECT_NEAR(n.length(), 1.0, Tolerance::absolute);
    EXPECT_NEAR(n.x(), 0.6, Tolerance::absolute);
    EXPECT_NEAR(n.y(), 0.8, Tolerance::absolute);
}

TEST(Vector2DTest, NormalizedZeroVector) {
    Vector2D v(0.0, 0.0);
    auto n = v.normalized();
    EXPECT_DOUBLE_EQ(n.x(), 0.0);
    EXPECT_DOUBLE_EQ(n.y(), 0.0);
}

TEST(Vector2DTest, DotProduct) {
    Vector2D a(1.0, 0.0);
    Vector2D b(0.0, 1.0);
    EXPECT_DOUBLE_EQ(a.dot(b), 0.0);

    Vector2D c(2.0, 3.0);
    Vector2D d(4.0, 5.0);
    EXPECT_DOUBLE_EQ(c.dot(d), 23.0);
}

TEST(Vector2DTest, CrossProduct) {
    Vector2D a(1.0, 0.0);
    Vector2D b(0.0, 1.0);
    EXPECT_DOUBLE_EQ(a.cross(b), 1.0);
    EXPECT_DOUBLE_EQ(b.cross(a), -1.0);
}

TEST(Vector2DTest, Perpendicular) {
    Vector2D v(3.0, 4.0);
    auto perp = v.perpendicular();
    EXPECT_NEAR(v.dot(perp), 0.0, Tolerance::absolute);
}

TEST(Vector2DTest, Arithmetic) {
    Vector2D a(1.0, 2.0);
    Vector2D b(3.0, 4.0);

    auto sum = a + b;
    EXPECT_DOUBLE_EQ(sum.x(), 4.0);
    EXPECT_DOUBLE_EQ(sum.y(), 6.0);

    auto diff = a - b;
    EXPECT_DOUBLE_EQ(diff.x(), -2.0);
    EXPECT_DOUBLE_EQ(diff.y(), -2.0);

    auto scaled = a * 2.0;
    EXPECT_DOUBLE_EQ(scaled.x(), 2.0);
    EXPECT_DOUBLE_EQ(scaled.y(), 4.0);
}

TEST(PointTest, DefaultConstruction) {
    Point p;
    EXPECT_DOUBLE_EQ(p.x(), 0.0);
    EXPECT_DOUBLE_EQ(p.y(), 0.0);
}

TEST(PointTest, Distance) {
    Point a(0.0, 0.0);
    Point b(3.0, 4.0);
    EXPECT_DOUBLE_EQ(a.distance_to(b), 5.0);
}

TEST(PointTest, Coincident) {
    Point a(1.0, 2.0);
    Point b(1.0, 2.0);
    EXPECT_TRUE(a.coincident(b));

    Point c(1.0 + 1e-11, 2.0);
    EXPECT_TRUE(a.coincident(c));

    Point d(1.0 + 1e-5, 2.0);
    EXPECT_FALSE(a.coincident(d));
}

TEST(PointTest, Translation) {
    Point p(1.0, 2.0);
    Vector2D offset(3.0, 4.0);
    auto translated = p.translated(offset);
    EXPECT_DOUBLE_EQ(translated.x(), 4.0);
    EXPECT_DOUBLE_EQ(translated.y(), 6.0);
}

TEST(PointTest, VectorTo) {
    Point a(1.0, 2.0);
    Point b(4.0, 6.0);
    auto v = a.to(b);
    EXPECT_DOUBLE_EQ(v.x(), 3.0);
    EXPECT_DOUBLE_EQ(v.y(), 4.0);
}
