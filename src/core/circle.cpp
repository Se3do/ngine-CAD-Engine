#include <ngine/core/circle.hpp>

#include <cmath>
#include <numbers>
#include <stdexcept>

namespace ngine {

Circle::Circle(Point center, Real radius) : center_(std::move(center)), radius_(radius) {
    if (radius <= 0.0) {
        throw std::invalid_argument("Circle radius must be positive");
    }
}

Real Circle::area() const noexcept {
    return std::numbers::pi * radius_ * radius_;
}

Real Circle::circumference() const noexcept {
    return 2.0 * std::numbers::pi * radius_;
}

bool Circle::contains(const Point& p) const noexcept {
    return center_.distance_squared_to(p) <= radius_ * radius_;
}

bool Circle::on_boundary(const Point& p, Real eps) const noexcept {
    Real dist = center_.distance_to(p);
    return std::abs(dist - radius_) <= eps;
}

Point Circle::point_at_angle(Real radians) const noexcept {
    return Point(center_.x() + radius_ * std::cos(radians),
                 center_.y() + radius_ * std::sin(radians));
}

void Circle::set_radius(Real radius) {
    if (radius <= 0.0) {
        throw std::invalid_argument("Circle radius must be positive");
    }
    radius_ = radius;
}

}  // namespace ngine
