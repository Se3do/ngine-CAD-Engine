#include <ngine/core/segment.hpp>

#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace ngine {

Segment::Segment(Point start, Point end) : start_(std::move(start)), end_(std::move(end)) {
    if (start_.coincident(end_)) {
        throw std::invalid_argument("Cannot create segment from two coincident points");
    }
}

Real Segment::length() const noexcept {
    return start_.distance_to(end_);
}

Real Segment::length_squared() const noexcept {
    return start_.distance_squared_to(end_);
}

Point Segment::midpoint() const noexcept {
    return Point((start_.x() + end_.x()) / 2.0, (start_.y() + end_.y()) / 2.0);
}

Line Segment::supporting_line() const {
    return Line::from_points(start_, end_);
}

Vector2D Segment::direction() const noexcept {
    return start_.to(end_).normalized();
}

bool Segment::contains(const Point& p, Real eps) const noexcept {
    Real d1 = start_.distance_to(p);
    Real d2 = p.distance_to(end_);
    Real len = length();
    return std::abs(d1 + d2 - len) <= eps;
}

Real Segment::distance_to(const Point& p) const noexcept {
    return p.distance_to(closest_point(p));
}

Point Segment::closest_point(const Point& p) const noexcept {
    Vector2D seg_vec = start_.to(end_);
    Vector2D point_vec = start_.to(p);

    Real t = point_vec.dot(seg_vec) / seg_vec.length_squared();
    t = std::clamp(t, 0.0, 1.0);

    return Point(start_.x() + t * seg_vec.x(), start_.y() + t * seg_vec.y());
}

}  // namespace ngine
