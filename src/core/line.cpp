#include <ngine/core/line.hpp>

#include <cmath>
#include <stdexcept>

namespace ngine {

Line Line::from_points(const Point& p1, const Point& p2) {
    Real dx = p2.x() - p1.x();
    Real dy = p2.y() - p1.y();

    if (nearly_zero(dx) && nearly_zero(dy)) {
        throw std::invalid_argument("Cannot create line from two coincident points");
    }

    Real a = -dy;
    Real b = dx;
    Real len = std::sqrt(a * a + b * b);
    a /= len;
    b /= len;
    Real c = -(a * p1.x() + b * p1.y());

    return Line(a, b, c);
}

Line Line::from_point_direction(const Point& p, const Vector2D& dir) {
    if (nearly_zero(dir.length_squared())) {
        throw std::invalid_argument("Cannot create line from zero direction vector");
    }

    Real a = -dir.y();
    Real b = dir.x();
    Real len = std::sqrt(a * a + b * b);
    a /= len;
    b /= len;
    Real c = -(a * p.x() + b * p.y());

    return Line(a, b, c);
}

Line Line::from_coefficients(Real a, Real b, Real c) {
    Real len = std::sqrt(a * a + b * b);
    if (nearly_zero(len)) {
        throw std::invalid_argument("Invalid line coefficients: a and b cannot both be zero");
    }
    return Line(a / len, b / len, c / len);
}

Point Line::project(const Point& p) const noexcept {
    Real dist = signed_distance(p);
    return Point(p.x() - a_ * dist, p.y() - b_ * dist);
}

bool Line::contains(const Point& p, Real eps) const noexcept {
    return std::abs(signed_distance(p)) <= eps;
}

bool Line::is_parallel(const Line& other, Real eps) const noexcept {
    Real cross = a_ * other.b_ - b_ * other.a_;
    return std::abs(cross) <= eps;
}

bool Line::is_perpendicular(const Line& other, Real eps) const noexcept {
    Real dot = a_ * other.a_ + b_ * other.b_;
    return std::abs(dot) <= eps;
}

}  // namespace ngine
