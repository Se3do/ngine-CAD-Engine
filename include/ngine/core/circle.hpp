#pragma once

#include <ngine/core/point.hpp>
#include <ngine/core/types.hpp>

namespace ngine {

class Circle {
public:
    Circle(Point center, Real radius);

    [[nodiscard]] const Point& center() const noexcept { return center_; }
    [[nodiscard]] Real radius() const noexcept { return radius_; }
    [[nodiscard]] EntityId id() const noexcept { return id_; }

    [[nodiscard]] Real area() const noexcept;
    [[nodiscard]] Real circumference() const noexcept;

    [[nodiscard]] bool contains(const Point& p) const noexcept;
    [[nodiscard]] bool on_boundary(const Point& p, Real eps = Tolerance::absolute) const noexcept;
    [[nodiscard]] Point point_at_angle(Real radians) const noexcept;

    void set_center(const Point& center) noexcept { center_ = center; }
    void set_radius(Real radius);

private:
    Point center_;
    Real radius_;
    EntityId id_{IdGenerator::next()};
};

}  // namespace ngine
