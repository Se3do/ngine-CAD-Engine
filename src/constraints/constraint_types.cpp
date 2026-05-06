#include <ngine/constraints/constraint_types.hpp>

#include <cmath>
#include <stdexcept>

namespace ngine {

// --- CoincidentConstraint ---

CoincidentConstraint::CoincidentConstraint(std::shared_ptr<Point> p1, std::shared_ptr<Point> p2)
    : p1_(std::move(p1)), p2_(std::move(p2)) {
    if (!p1_ || !p2_) {
        throw std::invalid_argument("CoincidentConstraint requires non-null points");
    }
}

ConstraintStatus CoincidentConstraint::evaluate() const {
    return p1_->coincident(*p2_) ? ConstraintStatus::Satisfied : ConstraintStatus::Violated;
}

Real CoincidentConstraint::error() const {
    return p1_->distance_to(*p2_);
}

std::vector<EntityId> CoincidentConstraint::involved_entities() const {
    return {p1_->id(), p2_->id()};
}

void CoincidentConstraint::apply_correction(Real step_size) {
    Real dx = p2_->x() - p1_->x();
    Real dy = p2_->y() - p1_->y();
    Real half_step = step_size * 0.5;

    p1_->set_position(p1_->x() + dx * half_step, p1_->y() + dy * half_step);
    p2_->set_position(p2_->x() - dx * half_step, p2_->y() - dy * half_step);
}

// --- ParallelConstraint ---

ParallelConstraint::ParallelConstraint(std::shared_ptr<Line> l1, std::shared_ptr<Line> l2)
    : l1_(std::move(l1)), l2_(std::move(l2)) {
    if (!l1_ || !l2_) {
        throw std::invalid_argument("ParallelConstraint requires non-null lines");
    }
}

ConstraintStatus ParallelConstraint::evaluate() const {
    return l1_->is_parallel(*l2_) ? ConstraintStatus::Satisfied : ConstraintStatus::Violated;
}

Real ParallelConstraint::error() const {
    Real cross = l1_->a() * l2_->b() - l1_->b() * l2_->a();
    return std::abs(cross);
}

std::vector<EntityId> ParallelConstraint::involved_entities() const {
    return {l1_->id(), l2_->id()};
}

void ParallelConstraint::apply_correction(Real /*step_size*/) {
    // Lines are immutable in implicit form — constraint solving for lines
    // requires re-parameterization. Left as no-op for sequential solver;
    // Newton-Raphson solver would handle via Jacobian.
}

// --- PerpendicularConstraint ---

PerpendicularConstraint::PerpendicularConstraint(std::shared_ptr<Line> l1,
                                                  std::shared_ptr<Line> l2)
    : l1_(std::move(l1)), l2_(std::move(l2)) {
    if (!l1_ || !l2_) {
        throw std::invalid_argument("PerpendicularConstraint requires non-null lines");
    }
}

ConstraintStatus PerpendicularConstraint::evaluate() const {
    return l1_->is_perpendicular(*l2_) ? ConstraintStatus::Satisfied : ConstraintStatus::Violated;
}

Real PerpendicularConstraint::error() const {
    Real dot = l1_->a() * l2_->a() + l1_->b() * l2_->b();
    return std::abs(dot);
}

std::vector<EntityId> PerpendicularConstraint::involved_entities() const {
    return {l1_->id(), l2_->id()};
}

void PerpendicularConstraint::apply_correction(Real /*step_size*/) {
    // Same as parallel — line constraints need re-parameterization
}

// --- FixedDistanceConstraint ---

FixedDistanceConstraint::FixedDistanceConstraint(std::shared_ptr<Point> p1,
                                                  std::shared_ptr<Point> p2, Real distance)
    : p1_(std::move(p1)), p2_(std::move(p2)), target_distance_(distance) {
    if (!p1_ || !p2_) {
        throw std::invalid_argument("FixedDistanceConstraint requires non-null points");
    }
    if (distance < 0.0) {
        throw std::invalid_argument("Distance must be non-negative");
    }
}

ConstraintStatus FixedDistanceConstraint::evaluate() const {
    return nearly_equal(p1_->distance_to(*p2_), target_distance_) ? ConstraintStatus::Satisfied
                                                                   : ConstraintStatus::Violated;
}

Real FixedDistanceConstraint::error() const {
    return std::abs(p1_->distance_to(*p2_) - target_distance_);
}

std::vector<EntityId> FixedDistanceConstraint::involved_entities() const {
    return {p1_->id(), p2_->id()};
}

void FixedDistanceConstraint::apply_correction(Real step_size) {
    Real current_dist = p1_->distance_to(*p2_);
    if (nearly_zero(current_dist)) {
        p2_->set_position(p2_->x() + target_distance_ * step_size, p2_->y());
        return;
    }

    Real scale = (target_distance_ - current_dist) / current_dist;
    Real dx = p2_->x() - p1_->x();
    Real dy = p2_->y() - p1_->y();
    Real half_correction = scale * step_size * 0.5;

    p1_->set_position(p1_->x() - dx * half_correction, p1_->y() - dy * half_correction);
    p2_->set_position(p2_->x() + dx * half_correction, p2_->y() + dy * half_correction);
}

// --- PointOnLineConstraint ---

PointOnLineConstraint::PointOnLineConstraint(std::shared_ptr<Point> point,
                                              std::shared_ptr<Line> line)
    : point_(std::move(point)), line_(std::move(line)) {
    if (!point_ || !line_) {
        throw std::invalid_argument("PointOnLineConstraint requires non-null point and line");
    }
}

ConstraintStatus PointOnLineConstraint::evaluate() const {
    return line_->contains(*point_) ? ConstraintStatus::Satisfied : ConstraintStatus::Violated;
}

Real PointOnLineConstraint::error() const {
    return line_->distance(*point_);
}

std::vector<EntityId> PointOnLineConstraint::involved_entities() const {
    return {point_->id(), line_->id()};
}

void PointOnLineConstraint::apply_correction(Real step_size) {
    Point projected = line_->project(*point_);
    Real dx = projected.x() - point_->x();
    Real dy = projected.y() - point_->y();
    point_->set_position(point_->x() + dx * step_size, point_->y() + dy * step_size);
}

}  // namespace ngine
