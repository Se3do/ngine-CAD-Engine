#pragma once

#include <ngine/core/point.hpp>
#include <ngine/core/types.hpp>
#include <ngine/core/vector2d.hpp>

#include <array>

namespace ngine {

class Transform {
public:
    [[nodiscard]] static Transform identity() noexcept;
    [[nodiscard]] static Transform translation(Real dx, Real dy) noexcept;
    [[nodiscard]] static Transform rotation(Real radians, Point center = {0, 0}) noexcept;
    [[nodiscard]] static Transform scale(Real sx, Real sy, Point center = {0, 0}) noexcept;

    [[nodiscard]] Point apply(const Point& p) const noexcept;
    [[nodiscard]] Vector2D apply(const Vector2D& v) const noexcept;
    [[nodiscard]] Transform compose(const Transform& other) const noexcept;
    [[nodiscard]] Transform inverse() const;

    [[nodiscard]] const std::array<Real, 9>& matrix() const noexcept { return m_; }

private:
    Transform() noexcept = default;
    explicit Transform(std::array<Real, 9> m) noexcept : m_(m) {}

    // Row-major 3x3: [a b tx; c d ty; 0 0 1]
    std::array<Real, 9> m_{1, 0, 0, 0, 1, 0, 0, 0, 1};
};

}  // namespace ngine
