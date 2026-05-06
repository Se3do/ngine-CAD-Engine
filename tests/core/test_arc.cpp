#include <ngine/core/arc.hpp>
#include <ngine/spatial/bounding_box.hpp>

#include <cmath>
#include <gtest/gtest.h>
#include <numbers>
#include <stdexcept>

using namespace ngine;

constexpr Real pi = std::numbers::pi;

TEST(ArcTest, ConstructionValid) {
    Arc arc(Point(0, 0), 5.0, 0.0, pi / 2.0);
    EXPECT_DOUBLE_EQ(arc.radius(), 5.0);
    EXPECT_DOUBLE_EQ(arc.start_angle(), 0.0);
    EXPECT_DOUBLE_EQ(arc.end_angle(), pi / 2.0);
}

TEST(ArcTest, ConstructionInvalidRadius) {
    EXPECT_THROW(Arc(Point(0, 0), 0.0, 0.0, pi), std::invalid_argument);
    EXPECT_THROW(Arc(Point(0, 0), -1.0, 0.0, pi), std::invalid_argument);
}

TEST(ArcTest, SweepAngle) {
    // Quarter circle
    Arc quarter(Point(0, 0), 1.0, 0.0, pi / 2.0);
    EXPECT_NEAR(quarter.sweep_angle(), pi / 2.0, 1e-10);

    // Half circle
    Arc half(Point(0, 0), 1.0, 0.0, pi);
    EXPECT_NEAR(half.sweep_angle(), pi, 1e-10);

    // Wrapping arc (from 3π/2 to π/2 = 270° sweep going through 0)
    Arc wrap(Point(0, 0), 1.0, 3.0 * pi / 2.0, pi / 2.0);
    EXPECT_NEAR(wrap.sweep_angle(), pi, 1e-10);
}

TEST(ArcTest, Length) {
    Arc quarter(Point(0, 0), 2.0, 0.0, pi / 2.0);
    EXPECT_NEAR(quarter.length(), 2.0 * pi / 2.0, 1e-10);
}

TEST(ArcTest, StartEndPoints) {
    Arc arc(Point(0, 0), 1.0, 0.0, pi / 2.0);

    Point sp = arc.start_point();
    EXPECT_NEAR(sp.x(), 1.0, 1e-10);
    EXPECT_NEAR(sp.y(), 0.0, 1e-10);

    Point ep = arc.end_point();
    EXPECT_NEAR(ep.x(), 0.0, 1e-10);
    EXPECT_NEAR(ep.y(), 1.0, 1e-10);
}

TEST(ArcTest, Midpoint) {
    Arc arc(Point(0, 0), 1.0, 0.0, pi / 2.0);
    Point mp = arc.midpoint();
    EXPECT_NEAR(mp.x(), std::cos(pi / 4.0), 1e-10);
    EXPECT_NEAR(mp.y(), std::sin(pi / 4.0), 1e-10);
}

TEST(ArcTest, PointAt) {
    Arc arc(Point(0, 0), 1.0, 0.0, pi);
    Point p = arc.point_at(0.5);  // midpoint at π/2
    EXPECT_NEAR(p.x(), 0.0, 1e-10);
    EXPECT_NEAR(p.y(), 1.0, 1e-10);
}

TEST(ArcTest, ContainsAngle) {
    Arc arc(Point(0, 0), 1.0, 0.0, pi / 2.0);
    EXPECT_TRUE(arc.contains_angle(0.0));
    EXPECT_TRUE(arc.contains_angle(pi / 4.0));
    EXPECT_TRUE(arc.contains_angle(pi / 2.0));
    EXPECT_FALSE(arc.contains_angle(pi));
    EXPECT_FALSE(arc.contains_angle(3.0 * pi / 2.0));
}

TEST(ArcTest, ContainsAngleWrapping) {
    // Arc from 300° to 60° (wrapping through 0°)
    Arc arc(Point(0, 0), 1.0, 5.0 * pi / 3.0, pi / 3.0);
    EXPECT_TRUE(arc.contains_angle(0.0));
    EXPECT_TRUE(arc.contains_angle(pi / 6.0));
    EXPECT_FALSE(arc.contains_angle(pi));
}

TEST(ArcTest, ContainsPoint) {
    Arc arc(Point(0, 0), 1.0, 0.0, pi / 2.0);
    EXPECT_TRUE(arc.contains(Point(1, 0)));
    EXPECT_TRUE(arc.contains(Point(0, 1)));
    EXPECT_TRUE(arc.contains(Point(std::cos(pi / 4.0), std::sin(pi / 4.0))));
    EXPECT_FALSE(arc.contains(Point(-1, 0)));
    EXPECT_FALSE(arc.contains(Point(0.5, 0)));  // inside circle, not on boundary
}

TEST(ArcTest, BoundingBox) {
    // Quarter circle in the first quadrant
    Arc arc(Point(0, 0), 1.0, 0.0, pi / 2.0);
    BoundingBox bb = arc.bounding_box();
    EXPECT_NEAR(bb.min_x(), 0.0, 1e-10);
    EXPECT_NEAR(bb.max_x(), 1.0, 1e-10);
    EXPECT_NEAR(bb.min_y(), 0.0, 1e-10);
    EXPECT_NEAR(bb.max_y(), 1.0, 1e-10);
}

TEST(ArcTest, BoundingBoxFullCircle) {
    // Nearly full circle (0 to almost 2π)
    Arc arc(Point(0, 0), 1.0, 0.0, 3.0 * pi / 2.0);
    BoundingBox bb = arc.bounding_box();
    EXPECT_NEAR(bb.min_x(), -1.0, 1e-10);
    EXPECT_NEAR(bb.max_x(), 1.0, 1e-10);
    EXPECT_NEAR(bb.min_y(), -1.0, 1e-10);
    EXPECT_NEAR(bb.max_y(), 1.0, 1e-10);
}

TEST(ArcTest, SetRadius) {
    Arc arc(Point(0, 0), 1.0, 0.0, pi);
    arc.set_radius(5.0);
    EXPECT_DOUBLE_EQ(arc.radius(), 5.0);
    EXPECT_THROW(arc.set_radius(-1.0), std::invalid_argument);
}

TEST(ArcTest, SetCenter) {
    Arc arc(Point(0, 0), 1.0, 0.0, pi);
    arc.set_center(Point(3, 4));
    EXPECT_NEAR(arc.center().x(), 3.0, 1e-10);
    EXPECT_NEAR(arc.center().y(), 4.0, 1e-10);
}

TEST(ArcTest, NegativeAngleNormalization) {
    Arc arc(Point(0, 0), 1.0, -pi / 2.0, pi / 2.0);
    // -π/2 normalizes to 3π/2
    EXPECT_NEAR(arc.start_angle(), 3.0 * pi / 2.0, 1e-10);
    EXPECT_NEAR(arc.end_angle(), pi / 2.0, 1e-10);
}
