#include <ngine/constraints/constraint.hpp>
#include <ngine/constraints/constraint_system.hpp>
#include <ngine/constraints/constraint_types.hpp>
#include <ngine/core/line.hpp>
#include <ngine/core/point.hpp>
#include <ngine/core/types.hpp>

#include <gtest/gtest.h>
#include <memory>
#include <stdexcept>

using namespace ngine;

TEST(ConstraintTest, CoincidentSatisfied) {
    const auto p1 = std::make_shared<Point>(1.0, 2.0);
    const auto p2 = std::make_shared<Point>(1.0, 2.0);

    const CoincidentConstraint c(p1, p2);
    EXPECT_EQ(c.evaluate(), ConstraintStatus::Satisfied);
    EXPECT_NEAR(c.error(), 0.0, Tolerance::absolute);
}

TEST(ConstraintTest, CoincidentViolated) {
    const auto p1 = std::make_shared<Point>(0.0, 0.0);
    const auto p2 = std::make_shared<Point>(3.0, 4.0);

    const CoincidentConstraint c(p1, p2);
    EXPECT_EQ(c.evaluate(), ConstraintStatus::Violated);
    EXPECT_NEAR(c.error(), 5.0, 1e-10);
}

TEST(ConstraintTest, FixedDistanceSatisfied) {
    const auto p1 = std::make_shared<Point>(0.0, 0.0);
    const auto p2 = std::make_shared<Point>(3.0, 4.0);

    const FixedDistanceConstraint c(p1, p2, 5.0);
    EXPECT_EQ(c.evaluate(), ConstraintStatus::Satisfied);
}

TEST(ConstraintTest, FixedDistanceViolated) {
    const auto p1 = std::make_shared<Point>(0.0, 0.0);
    const auto p2 = std::make_shared<Point>(3.0, 4.0);

    const FixedDistanceConstraint c(p1, p2, 10.0);
    EXPECT_EQ(c.evaluate(), ConstraintStatus::Violated);
    EXPECT_NEAR(c.error(), 5.0, 1e-10);
}

TEST(ConstraintTest, PointOnLineSatisfied) {
    const auto point = std::make_shared<Point>(5.0, 0.0);
    const auto line = std::make_shared<Line>(Line::from_points(Point(0, 0), Point(10, 0)));

    const PointOnLineConstraint c(point, line);
    EXPECT_EQ(c.evaluate(), ConstraintStatus::Satisfied);
}

TEST(ConstraintTest, PointOnLineViolated) {
    const auto point = std::make_shared<Point>(5.0, 3.0);
    const auto line = std::make_shared<Line>(Line::from_points(Point(0, 0), Point(10, 0)));

    const PointOnLineConstraint c(point, line);
    EXPECT_EQ(c.evaluate(), ConstraintStatus::Violated);
    EXPECT_NEAR(c.error(), 3.0, 1e-10);
}

TEST(ConstraintTest, NullPointsThrow) {
    EXPECT_THROW(CoincidentConstraint(nullptr, std::make_shared<Point>(0, 0)),
                 std::invalid_argument);
}

// --- Solver Tests ---

TEST(SolverTest, CoincidentConverges) {
    const auto p1 = std::make_shared<Point>(0.0, 0.0);
    const auto p2 = std::make_shared<Point>(1.0, 1.0);

    ConstraintSystem system;
    system.add_constraint(std::make_shared<CoincidentConstraint>(p1, p2));

    const auto result = system.solve();
    EXPECT_TRUE(result.converged);
    EXPECT_NEAR(p1->distance_to(*p2), 0.0, 1e-6);
}

TEST(SolverTest, FixedDistanceConverges) {
    const auto p1 = std::make_shared<Point>(0.0, 0.0);
    const auto p2 = std::make_shared<Point>(1.0, 0.0);

    ConstraintSystem system;
    system.add_constraint(std::make_shared<FixedDistanceConstraint>(p1, p2, 5.0));

    const auto result = system.solve();
    EXPECT_TRUE(result.converged);
    EXPECT_NEAR(p1->distance_to(*p2), 5.0, 1e-6);
}

TEST(SolverTest, PointOnLineConverges) {
    const auto point = std::make_shared<Point>(5.0, 3.0);
    const auto line = std::make_shared<Line>(Line::from_points(Point(0, 0), Point(10, 0)));

    ConstraintSystem system;
    system.add_constraint(std::make_shared<PointOnLineConstraint>(point, line));

    const auto result = system.solve();
    EXPECT_TRUE(result.converged);
    EXPECT_TRUE(line->contains(*point, 1e-6));
}

TEST(SolverTest, EmptySystemConverges) {
    ConstraintSystem system;
    const auto result = system.solve();
    EXPECT_TRUE(result.converged);
    EXPECT_EQ(result.iterations_used, 0U);
}

TEST(SolverTest, MultipleConstraints) {
    const auto p1 = std::make_shared<Point>(0.0, 0.0);
    const auto p2 = std::make_shared<Point>(10.0, 0.0);
    const auto p3 = std::make_shared<Point>(5.0, 5.0);

    ConstraintSystem system;
    system.add_constraint(std::make_shared<FixedDistanceConstraint>(p1, p2, 4.0));
    system.add_constraint(std::make_shared<FixedDistanceConstraint>(p2, p3, 3.0));

    const auto result = system.solve();
    EXPECT_TRUE(result.converged);
    EXPECT_NEAR(p1->distance_to(*p2), 4.0, 1e-6);
    EXPECT_NEAR(p2->distance_to(*p3), 3.0, 1e-6);
}
