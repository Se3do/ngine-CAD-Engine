#include <ngine/core/circle.hpp>
#include <ngine/core/line.hpp>
#include <ngine/core/segment.hpp>
#include <ngine/intersection/intersection_engine.hpp>

#include <cmath>
#include <gtest/gtest.h>
#include <numbers>

using namespace ngine;

class IntersectionTest : public ::testing::Test {
   protected:
    IntersectionEngine engine;
};

// --- Line-Line ---

TEST_F(IntersectionTest, LineLineIntersecting) {
    auto l1 = Line::from_points(Point(0, 0), Point(1, 1));
    auto l2 = Line::from_points(Point(0, 1), Point(1, 0));

    auto result = engine.intersect(l1, l2);
    ASSERT_TRUE(result.has_intersection());
    ASSERT_EQ(result.count(), 1u);
    EXPECT_NEAR(result.points[0].x(), 0.5, 1e-10);
    EXPECT_NEAR(result.points[0].y(), 0.5, 1e-10);
}

TEST_F(IntersectionTest, LineLineParallel) {
    auto l1 = Line::from_points(Point(0, 0), Point(1, 0));
    auto l2 = Line::from_points(Point(0, 5), Point(1, 5));

    auto result = engine.intersect(l1, l2);
    EXPECT_FALSE(result.has_intersection());
    EXPECT_EQ(result.type, IntersectionType::None);
}

TEST_F(IntersectionTest, LineLinePerpendicular) {
    auto l1 = Line::from_points(Point(0, 0), Point(10, 0));
    auto l2 = Line::from_points(Point(5, -5), Point(5, 5));

    auto result = engine.intersect(l1, l2);
    ASSERT_EQ(result.count(), 1u);
    EXPECT_NEAR(result.points[0].x(), 5.0, 1e-10);
    EXPECT_NEAR(result.points[0].y(), 0.0, 1e-10);
}

TEST_F(IntersectionTest, LineLineAtOrigin) {
    auto l1 = Line::from_points(Point(-1, 0), Point(1, 0));
    auto l2 = Line::from_points(Point(0, -1), Point(0, 1));

    auto result = engine.intersect(l1, l2);
    ASSERT_EQ(result.count(), 1u);
    EXPECT_NEAR(result.points[0].x(), 0.0, 1e-10);
    EXPECT_NEAR(result.points[0].y(), 0.0, 1e-10);
}

// --- Line-Circle ---

TEST_F(IntersectionTest, LineCircleTwoPoints) {
    auto line = Line::from_points(Point(-10, 0), Point(10, 0));
    Circle circle(Point(0, 0), 5.0);

    auto result = engine.intersect(line, circle);
    ASSERT_EQ(result.type, IntersectionType::TwoPoints);
    ASSERT_EQ(result.count(), 2u);

    // Points should be at (-5,0) and (5,0)
    bool found_neg = false, found_pos = false;
    for (const auto& p : result.points) {
        EXPECT_NEAR(p.y(), 0.0, 1e-10);
        if (std::abs(p.x() - 5.0) < 1e-10)
            found_pos = true;
        if (std::abs(p.x() + 5.0) < 1e-10)
            found_neg = true;
    }
    EXPECT_TRUE(found_neg);
    EXPECT_TRUE(found_pos);
}

TEST_F(IntersectionTest, LineCircleTangent) {
    auto line = Line::from_points(Point(-10, 5), Point(10, 5));
    Circle circle(Point(0, 0), 5.0);

    auto result = engine.intersect(line, circle);
    ASSERT_EQ(result.type, IntersectionType::Point);
    ASSERT_EQ(result.count(), 1u);
    EXPECT_NEAR(result.points[0].x(), 0.0, 1e-10);
    EXPECT_NEAR(result.points[0].y(), 5.0, 1e-10);
}

TEST_F(IntersectionTest, LineCircleNoIntersection) {
    auto line = Line::from_points(Point(-10, 10), Point(10, 10));
    Circle circle(Point(0, 0), 5.0);

    auto result = engine.intersect(line, circle);
    EXPECT_EQ(result.type, IntersectionType::None);
}

// --- Circle-Circle ---

TEST_F(IntersectionTest, CircleCircleTwoPoints) {
    Circle c1(Point(0, 0), 5.0);
    Circle c2(Point(6, 0), 5.0);

    auto result = engine.intersect(c1, c2);
    ASSERT_EQ(result.type, IntersectionType::TwoPoints);
    ASSERT_EQ(result.count(), 2u);

    for (const auto& p : result.points) {
        EXPECT_NEAR(c1.center().distance_to(p), 5.0, 1e-8);
        EXPECT_NEAR(c2.center().distance_to(p), 5.0, 1e-8);
    }
}

TEST_F(IntersectionTest, CircleCircleExternalTangent) {
    Circle c1(Point(0, 0), 3.0);
    Circle c2(Point(6, 0), 3.0);

    auto result = engine.intersect(c1, c2);
    ASSERT_EQ(result.type, IntersectionType::Point);
    ASSERT_EQ(result.count(), 1u);
    EXPECT_NEAR(result.points[0].x(), 3.0, 1e-10);
    EXPECT_NEAR(result.points[0].y(), 0.0, 1e-10);
}

TEST_F(IntersectionTest, CircleCircleNoIntersection) {
    Circle c1(Point(0, 0), 2.0);
    Circle c2(Point(10, 0), 2.0);

    auto result = engine.intersect(c1, c2);
    EXPECT_EQ(result.type, IntersectionType::None);
}

TEST_F(IntersectionTest, CircleCircleContained) {
    Circle c1(Point(0, 0), 10.0);
    Circle c2(Point(0, 0), 3.0);

    auto result = engine.intersect(c1, c2);
    EXPECT_EQ(result.type, IntersectionType::None);
}

TEST_F(IntersectionTest, CircleCircleCoincident) {
    Circle c1(Point(0, 0), 5.0);
    Circle c2(Point(0, 0), 5.0);

    auto result = engine.intersect(c1, c2);
    EXPECT_EQ(result.type, IntersectionType::Coincident);
}

// --- Segment-Segment ---

TEST_F(IntersectionTest, SegmentSegmentCross) {
    Segment s1(Point(0, 0), Point(10, 10));
    Segment s2(Point(0, 10), Point(10, 0));

    auto result = engine.intersect(s1, s2);
    ASSERT_EQ(result.count(), 1u);
    EXPECT_NEAR(result.points[0].x(), 5.0, 1e-10);
    EXPECT_NEAR(result.points[0].y(), 5.0, 1e-10);
}

TEST_F(IntersectionTest, SegmentSegmentNoIntersection) {
    Segment s1(Point(0, 0), Point(5, 0));
    Segment s2(Point(0, 1), Point(5, 1));

    auto result = engine.intersect(s1, s2);
    EXPECT_EQ(result.type, IntersectionType::None);
}

TEST_F(IntersectionTest, SegmentSegmentEndpoint) {
    Segment s1(Point(0, 0), Point(5, 5));
    Segment s2(Point(5, 5), Point(10, 0));

    auto result = engine.intersect(s1, s2);
    ASSERT_EQ(result.count(), 1u);
    EXPECT_NEAR(result.points[0].x(), 5.0, 1e-10);
    EXPECT_NEAR(result.points[0].y(), 5.0, 1e-10);
}

TEST_F(IntersectionTest, SegmentSegmentCollinearNoOverlap) {
    Segment s1(Point(0, 0), Point(3, 0));
    Segment s2(Point(5, 0), Point(8, 0));

    auto result = engine.intersect(s1, s2);
    EXPECT_EQ(result.type, IntersectionType::None);
}
