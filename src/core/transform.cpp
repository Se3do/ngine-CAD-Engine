#include <ngine/core/transform.hpp>

#include <cmath>
#include <stdexcept>

namespace ngine {

Transform Transform::identity() noexcept {
    return Transform();
}

Transform Transform::translation(Real dx, Real dy) noexcept {
    return Transform({1, 0, dx, 0, 1, dy, 0, 0, 1});
}

Transform Transform::rotation(Real radians, Point center) noexcept {
    Real cos_a = std::cos(radians);
    Real sin_a = std::sin(radians);
    Real cx = center.x();
    Real cy = center.y();

    return Transform({cos_a, -sin_a, cx - cos_a * cx + sin_a * cy, sin_a, cos_a,
                      cy - sin_a * cx - cos_a * cy, 0, 0, 1});
}

Transform Transform::scale(Real sx, Real sy, Point center) noexcept {
    Real cx = center.x();
    Real cy = center.y();

    return Transform({sx, 0, cx * (1 - sx), 0, sy, cy * (1 - sy), 0, 0, 1});
}

Point Transform::apply(const Point& p) const noexcept {
    Real x = m_[0] * p.x() + m_[1] * p.y() + m_[2];
    Real y = m_[3] * p.x() + m_[4] * p.y() + m_[5];
    return Point(x, y);
}

Vector2D Transform::apply(const Vector2D& v) const noexcept {
    Real x = m_[0] * v.x() + m_[1] * v.y();
    Real y = m_[3] * v.x() + m_[4] * v.y();
    return Vector2D(x, y);
}

Transform Transform::compose(const Transform& other) const noexcept {
    std::array<Real, 9> result{};
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            Real sum = 0.0;
            for (int k = 0; k < 3; ++k) {
                sum += m_[static_cast<std::size_t>(row * 3 + k)] *
                       other.m_[static_cast<std::size_t>(k * 3 + col)];
            }
            result[static_cast<std::size_t>(row * 3 + col)] = sum;
        }
    }
    return Transform(result);
}

Transform Transform::inverse() const {
    Real det = m_[0] * m_[4] - m_[1] * m_[3];
    if (nearly_zero(det)) {
        throw std::runtime_error("Transform is not invertible (determinant is zero)");
    }

    Real inv_det = 1.0 / det;
    std::array<Real, 9> inv{};
    inv[0] = m_[4] * inv_det;
    inv[1] = -m_[1] * inv_det;
    inv[2] = (m_[1] * m_[5] - m_[4] * m_[2]) * inv_det;
    inv[3] = -m_[3] * inv_det;
    inv[4] = m_[0] * inv_det;
    inv[5] = (m_[3] * m_[2] - m_[0] * m_[5]) * inv_det;
    inv[6] = 0;
    inv[7] = 0;
    inv[8] = 1;

    return Transform(inv);
}

}  // namespace ngine
