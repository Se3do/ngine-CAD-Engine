#pragma once

#include <ngine/core/types.hpp>

#include <cmath>

namespace ngine {

class Vector2D {
public:
    constexpr Vector2D() noexcept = default;
    constexpr Vector2D(Real x, Real y) noexcept : x_(x), y_(y) {}

    [[nodiscard]] constexpr Real x() const noexcept { return x_; }
    [[nodiscard]] constexpr Real y() const noexcept { return y_; }

    [[nodiscard]] constexpr Real length_squared() const noexcept { return x_ * x_ + y_ * y_; }

    [[nodiscard]] Real length() const noexcept { return std::sqrt(length_squared()); }

    [[nodiscard]] Vector2D normalized() const {
        Real len = length();
        if (nearly_zero(len)) {
            return {0.0, 0.0};
        }
        return {x_ / len, y_ / len};
    }

    [[nodiscard]] constexpr Vector2D perpendicular() const noexcept { return {-y_, x_}; }

    [[nodiscard]] constexpr Real dot(const Vector2D& other) const noexcept {
        return x_ * other.x_ + y_ * other.y_;
    }

    [[nodiscard]] constexpr Real cross(const Vector2D& other) const noexcept {
        return x_ * other.y_ - y_ * other.x_;
    }

    constexpr Vector2D operator+(const Vector2D& rhs) const noexcept {
        return {x_ + rhs.x_, y_ + rhs.y_};
    }

    constexpr Vector2D operator-(const Vector2D& rhs) const noexcept {
        return {x_ - rhs.x_, y_ - rhs.y_};
    }

    constexpr Vector2D operator*(Real scalar) const noexcept {
        return {x_ * scalar, y_ * scalar};
    }

    constexpr Vector2D operator-() const noexcept { return {-x_, -y_}; }

    [[nodiscard]] bool operator==(const Vector2D& rhs) const noexcept {
        return nearly_equal(x_, rhs.x_) && nearly_equal(y_, rhs.y_);
    }

private:
    Real x_{0.0};
    Real y_{0.0};
};

[[nodiscard]] constexpr Vector2D operator*(Real scalar, const Vector2D& v) noexcept {
    return v * scalar;
}

}  // namespace ngine
