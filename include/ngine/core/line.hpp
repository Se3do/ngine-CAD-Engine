#pragma once

#include <ngine/core/point.hpp>
#include <ngine/core/types.hpp>
#include <ngine/core/vector2d.hpp>

namespace ngine {

class Line {
public:
    [[nodiscard]] static Line from_points(const Point& p1, const Point& p2);
    [[nodiscard]] static Line from_point_direction(const Point& p, const Vector2D& dir);
    [[nodiscard]] static Line from_coefficients(Real a, Real b, Real c);

    [[nodiscard]] Real a() const noexcept { return a_; }
    [[nodiscard]] Real b() const noexcept { return b_; }
    [[nodiscard]] Real c() const noexcept { return c_; }
    [[nodiscard]] EntityId id() const noexcept { return id_; }

    [[nodiscard]] Vector2D direction() const noexcept { return {b_, -a_}; }
    [[nodiscard]] Vector2D normal() const noexcept { return {a_, b_}; }

    [[nodiscard]] Real signed_distance(const Point& p) const noexcept {
        return a_ * p.x() + b_ * p.y() + c_;
    }

    [[nodiscard]] Real distance(const Point& p) const noexcept {
        return std::abs(signed_distance(p));
    }

    [[nodiscard]] Point project(const Point& p) const noexcept;
    [[nodiscard]] bool contains(const Point& p, Real eps = Tolerance::absolute) const noexcept;
    [[nodiscard]] bool is_parallel(const Line& other, Real eps = Tolerance::absolute) const noexcept;
    [[nodiscard]] bool is_perpendicular(const Line& other,
                                        Real eps = Tolerance::absolute) const noexcept;

private:
    Line(Real a, Real b, Real c) : a_(a), b_(b), c_(c), id_(IdGenerator::next()) {}

    Real a_{0.0};
    Real b_{0.0};
    Real c_{0.0};
    EntityId id_{0};
};

}  // namespace ngine
