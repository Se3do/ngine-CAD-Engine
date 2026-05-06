#pragma once

#include <ngine/core/point.hpp>
#include <ngine/core/types.hpp>

namespace ngine {

class BoundingBox {
   public:
    constexpr BoundingBox() noexcept = default;
    constexpr BoundingBox(Real min_x, Real min_y, Real max_x, Real max_y) noexcept
        : min_x_(min_x), min_y_(min_y), max_x_(max_x), max_y_(max_y) {}

    [[nodiscard]] constexpr Real min_x() const noexcept { return min_x_; }
    [[nodiscard]] constexpr Real min_y() const noexcept { return min_y_; }
    [[nodiscard]] constexpr Real max_x() const noexcept { return max_x_; }
    [[nodiscard]] constexpr Real max_y() const noexcept { return max_y_; }
    [[nodiscard]] constexpr Real width() const noexcept { return max_x_ - min_x_; }
    [[nodiscard]] constexpr Real height() const noexcept { return max_y_ - min_y_; }

    [[nodiscard]] Point center() const noexcept {
        return Point((min_x_ + max_x_) / 2.0, (min_y_ + max_y_) / 2.0);
    }

    [[nodiscard]] bool contains(const Point& p) const noexcept {
        return p.x() >= min_x_ && p.x() <= max_x_ && p.y() >= min_y_ && p.y() <= max_y_;
    }

    [[nodiscard]] constexpr bool intersects(const BoundingBox& other) const noexcept {
        return !(other.max_x_ < min_x_ || other.min_x_ > max_x_ || other.max_y_ < min_y_ ||
                 other.min_y_ > max_y_);
    }

    [[nodiscard]] constexpr bool contains_box(const BoundingBox& other) const noexcept {
        return other.min_x_ >= min_x_ && other.max_x_ <= max_x_ && other.min_y_ >= min_y_ &&
               other.max_y_ <= max_y_;
    }

    [[nodiscard]] BoundingBox expanded(Real margin) const noexcept {
        return {min_x_ - margin, min_y_ - margin, max_x_ + margin, max_y_ + margin};
    }

   private:
    Real min_x_{0.0};
    Real min_y_{0.0};
    Real max_x_{0.0};
    Real max_y_{0.0};
};

}  // namespace ngine
