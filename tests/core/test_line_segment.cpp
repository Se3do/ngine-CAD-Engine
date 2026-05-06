#include <ngine/core/line.hpp>
#include <ngine/core/segment.hpp>

#include <cmath>
#include <gtest/gtest.h>
#include <numbers>

using namespace ngine;

TEST(LineTest, FromTwoPoints) {
    auto line = Line::from_points(Point(0, 0), Point(1, 0));
    EXPECT_NEAR(line.distance(Point(0, 0)), 0.0, Tolerance::absolute);
    EXPECT_NEAR(line.distance(Point(5, 0)), 0.0, Tolerance::absolute);
    EXPECT_NEAR(line.distance(Point(0, 1)), 1.0, Tolerance::absolute);
}

TEST(LineTest, FromPointDirection) {
    auto line = Line::from_point_direction(Point(0, 0), Vector2D(1, 1));
    EXPECT_TRUE(line.contains(Point(0, 0)));
    EXPECT_TRUE(line.contains(Point(5, 5)));
    EXPECT_FALSE(line.contains(Point(1, 0)));
}

TEST(LineTest, CoincidentPointsThrows) {
    EXPECT_THROW((void)Line::from_points(Point(1, 1), Point(1, 1)), std::invalid_argument);
}

TEST(LineTest, ZeroDirectionThrows) {
    EXPECT_THROW((void)Line::from_point_direction(Point(0, 0), Vector2D(0, 0)),
                 std::invalid_argument);
}

TEST(LineTest, SignedDistance) {
    auto line = Line::from_points(Point(0, 0), Point(1, 0));
    Real d = line.signed_distance(Point(0, 1));
    EXPECT_GT(std::abs(d), 0.0);
}

TEST(LineTest, Projection) {
    auto line = Line::from_points(Point(0, 0), Point(1, 0));
    Point projected = line.project(Point(3, 5));
    EXPECT_NEAR(projected.y(), 0.0, Tolerance::absolute);
    EXPECT_NEAR(projected.x(), 3.0, Tolerance::absolute);
}

TEST(LineTest, Parallel) {
    auto l1 = Line::from_points(Point(0, 0), Point(1, 0));
    auto l2 = Line::from_points(Point(0, 5), Point(1, 5));
    EXPECT_TRUE(l1.is_parallel(l2));

    auto l3 = Line::from_points(Point(0, 0), Point(1, 1));
    EXPECT_FALSE(l1.is_parallel(l3));
}

TEST(LineTest, Perpendicular) {
    auto l1 = Line::from_points(Point(0, 0), Point(1, 0));
    auto l2 = Line::from_points(Point(0, 0), Point(0, 1));
    EXPECT_TRUE(l1.is_perpendicular(l2));
}

TEST(SegmentTest, Construction) {
    Segment s(Point(0, 0), Point(3, 4));
    EXPECT_DOUBLE_EQ(s.length(), 5.0);
}

TEST(SegmentTest, DegenerateThrows) {
    EXPECT_THROW(Segment(Point(1, 1), Point(1, 1)), std::invalid_argument);
}

TEST(SegmentTest, Midpoint) {
    Segment s(Point(0, 0), Point(4, 6));
    auto mid = s.midpoint();
    EXPECT_DOUBLE_EQ(mid.x(), 2.0);
    EXPECT_DOUBLE_EQ(mid.y(), 3.0);
}

TEST(SegmentTest, Contains) {
    Segment s(Point(0, 0), Point(10, 0));
    EXPECT_TRUE(s.contains(Point(5, 0)));
    EXPECT_TRUE(s.contains(Point(0, 0)));
    EXPECT_TRUE(s.contains(Point(10, 0)));
    EXPECT_FALSE(s.contains(Point(11, 0)));
    EXPECT_FALSE(s.contains(Point(5, 1)));
}

TEST(SegmentTest, ClosestPoint) {
    Segment s(Point(0, 0), Point(10, 0));

    auto cp1 = s.closest_point(Point(5, 3));
    EXPECT_NEAR(cp1.x(), 5.0, Tolerance::absolute);
    EXPECT_NEAR(cp1.y(), 0.0, Tolerance::absolute);

    auto cp2 = s.closest_point(Point(-5, 3));
    EXPECT_NEAR(cp2.x(), 0.0, Tolerance::absolute);
    EXPECT_NEAR(cp2.y(), 0.0, Tolerance::absolute);
}
