#pragma once

#include <ngine/constraints/constraint.hpp>
#include <ngine/core/line.hpp>
#include <ngine/core/point.hpp>

#include <memory>

namespace ngine {

class CoincidentConstraint final : public Constraint {
public:
    CoincidentConstraint(std::shared_ptr<Point> p1, std::shared_ptr<Point> p2);

    [[nodiscard]] ConstraintStatus evaluate() const override;
    [[nodiscard]] Real error() const override;
    [[nodiscard]] std::vector<EntityId> involved_entities() const override;
    [[nodiscard]] std::string type_name() const override { return "Coincident"; }
    void apply_correction(Real step_size) override;

private:
    std::shared_ptr<Point> p1_;
    std::shared_ptr<Point> p2_;
};

class ParallelConstraint final : public Constraint {
public:
    ParallelConstraint(std::shared_ptr<Line> l1, std::shared_ptr<Line> l2);

    [[nodiscard]] ConstraintStatus evaluate() const override;
    [[nodiscard]] Real error() const override;
    [[nodiscard]] std::vector<EntityId> involved_entities() const override;
    [[nodiscard]] std::string type_name() const override { return "Parallel"; }
    void apply_correction(Real step_size) override;

private:
    std::shared_ptr<Line> l1_;
    std::shared_ptr<Line> l2_;
};

class PerpendicularConstraint final : public Constraint {
public:
    PerpendicularConstraint(std::shared_ptr<Line> l1, std::shared_ptr<Line> l2);

    [[nodiscard]] ConstraintStatus evaluate() const override;
    [[nodiscard]] Real error() const override;
    [[nodiscard]] std::vector<EntityId> involved_entities() const override;
    [[nodiscard]] std::string type_name() const override { return "Perpendicular"; }
    void apply_correction(Real step_size) override;

private:
    std::shared_ptr<Line> l1_;
    std::shared_ptr<Line> l2_;
};

class FixedDistanceConstraint final : public Constraint {
public:
    FixedDistanceConstraint(std::shared_ptr<Point> p1, std::shared_ptr<Point> p2, Real distance);

    [[nodiscard]] ConstraintStatus evaluate() const override;
    [[nodiscard]] Real error() const override;
    [[nodiscard]] std::vector<EntityId> involved_entities() const override;
    [[nodiscard]] std::string type_name() const override { return "FixedDistance"; }
    void apply_correction(Real step_size) override;

private:
    std::shared_ptr<Point> p1_;
    std::shared_ptr<Point> p2_;
    Real target_distance_;
};

class PointOnLineConstraint final : public Constraint {
public:
    PointOnLineConstraint(std::shared_ptr<Point> point, std::shared_ptr<Line> line);

    [[nodiscard]] ConstraintStatus evaluate() const override;
    [[nodiscard]] Real error() const override;
    [[nodiscard]] std::vector<EntityId> involved_entities() const override;
    [[nodiscard]] std::string type_name() const override { return "PointOnLine"; }
    void apply_correction(Real step_size) override;

private:
    std::shared_ptr<Point> point_;
    std::shared_ptr<Line> line_;
};

}  // namespace ngine
