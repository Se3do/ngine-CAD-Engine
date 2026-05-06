#pragma once

#include <ngine/core/types.hpp>
#include <ngine/core/vector2d.hpp>

#include <cmath>

namespace ngine {

class Point {
public:
    Point() noexcept = default;
    Point(Real x, Real y) noexcept : x_(x), y_(y), id_(IdGenerator::next()) {}

    [[nodiscard]] Real x() const noexcept { return x_; }
    [[nodiscard]] Real y() const noexcept { return y_; }
    [[nodiscard]] EntityId id() const noexcept { return id_; }

    void set_x(Real x) noexcept { x_ = x; }
    void set_y(Real y) noexcept { y_ = y; }
    void set_position(Real x, Real y) noexcept {
        x_ = x;
        y_ = y;
    }

    [[nodiscard]] Real distance_to(const Point& other) const noexcept {
        Real dx = x_ - other.x_;
        Real dy = y_ - other.y_;
        return std::sqrt(dx * dx + dy * dy);
    }

    [[nodiscard]] Real distance_squared_to(const Point& other) const noexcept {
        Real dx = x_ - other.x_;
        Real dy = y_ - other.y_;
        return dx * dx + dy * dy;
    }

    [[nodiscard]] Vector2D to(const Point& other) const noexcept {
        return {other.x_ - x_, other.y_ - y_};
    }

    [[nodiscard]] Point translated(const Vector2D& offset) const noexcept {
        Point result(x_ + offset.x(), y_ + offset.y());
        return result;
    }

    [[nodiscard]] bool coincident(const Point& other,
                                    Real eps = Tolerance::absolute) const noexcept {
        return distance_squared_to(other) <= eps * eps;
    }

    [[nodiscard]] bool operator==(const Point& rhs) const noexcept {
        return coincident(rhs);
    }

private:
    Real x_{0.0};
    Real y_{0.0};
    EntityId id_{0};
};

}  // namespace ngine
