#include <ngine/core/arc.hpp>
#include <ngine/spatial/bounding_box.hpp>

#include <algorithm>
#include <cmath>
#include <numbers>
#include <stdexcept>

namespace ngine {

namespace {

constexpr Real two_pi = 2.0 * std::numbers::pi;

}  // namespace

Real Arc::normalize_angle(Real angle) noexcept {
    angle = std::fmod(angle, two_pi);
    if (angle < 0.0) {
        angle += two_pi;
    }
    return angle;
}

Arc::Arc(const Point& center, Real radius, Real start_angle, Real end_angle)
    : center_(center),
      radius_(radius),
      start_angle_(normalize_angle(start_angle)),
      end_angle_(normalize_angle(end_angle)) {
    if (radius <= 0.0) {
        throw std::invalid_argument("Arc radius must be positive");
    }
}

Real Arc::sweep_angle() const noexcept {
    Real sweep = end_angle_ - start_angle_;
    if (sweep <= 0.0) {
        sweep += two_pi;
    }
    return sweep;
}

Real Arc::length() const noexcept {
    return radius_ * sweep_angle();
}

Point Arc::start_point() const noexcept {
    return point_at_angle(start_angle_);
}

Point Arc::end_point() const noexcept {
    return point_at_angle(end_angle_);
}

Point Arc::midpoint() const noexcept {
    return point_at(0.5);
}

Point Arc::point_at(Real t) const noexcept {
    Real angle = start_angle_ + t * sweep_angle();
    return point_at_angle(angle);
}

Point Arc::point_at_angle(Real radians) const noexcept {
    return Point(center_.x() + radius_ * std::cos(radians),
                 center_.y() + radius_ * std::sin(radians));
}

bool Arc::contains_angle(Real angle) const noexcept {
    angle = normalize_angle(angle);
    Real sweep = sweep_angle();

    Real delta = normalize_angle(angle - start_angle_);
    return delta <= sweep + Tolerance::absolute;
}

bool Arc::contains(const Point& p, Real eps) const noexcept {
    Real dist = center_.distance_to(p);
    if (std::abs(dist - radius_) > eps) {
        return false;
    }
    Real angle = std::atan2(p.y() - center_.y(), p.x() - center_.x());
    return contains_angle(angle);
}

bool Arc::on_boundary(const Point& p, Real eps) const noexcept {
    return contains(p, eps);
}

BoundingBox Arc::bounding_box() const noexcept {
    // Start with the endpoints
    Point sp = start_point();
    Point ep = end_point();

    Real min_x = std::min(sp.x(), ep.x());
    Real max_x = std::max(sp.x(), ep.x());
    Real min_y = std::min(sp.y(), ep.y());
    Real max_y = std::max(sp.y(), ep.y());

    // Check each cardinal direction (0, π/2, π, 3π/2)
    // If the arc passes through that angle, expand the box
    auto check_angle = [&](Real angle, Real px, Real py) {
        if (contains_angle(angle)) {
            min_x = std::min(min_x, px);
            max_x = std::max(max_x, px);
            min_y = std::min(min_y, py);
            max_y = std::max(max_y, py);
        }
    };

    check_angle(0.0, center_.x() + radius_, center_.y());                        // right
    check_angle(std::numbers::pi / 2.0, center_.x(), center_.y() + radius_);     // top
    check_angle(std::numbers::pi, center_.x() - radius_, center_.y());           // left
    check_angle(3.0 * std::numbers::pi / 2.0, center_.x(), center_.y() - radius_);  // bottom

    return BoundingBox(min_x, min_y, max_x, max_y);
}

void Arc::set_radius(Real radius) {
    if (radius <= 0.0) {
        throw std::invalid_argument("Arc radius must be positive");
    }
    radius_ = radius;
}

}  // namespace ngine
