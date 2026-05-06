#pragma once

#include <ngine/core/line.hpp>
#include <ngine/core/point.hpp>
#include <ngine/core/types.hpp>
#include <ngine/core/vector2d.hpp>

namespace ngine {

class Segment {
   public:
    Segment(Point start, Point end);

    [[nodiscard]] const Point& start() const noexcept { return start_; }
    [[nodiscard]] const Point& end() const noexcept { return end_; }
    [[nodiscard]] EntityId id() const noexcept { return id_; }

    [[nodiscard]] Real length() const noexcept;
    [[nodiscard]] Real length_squared() const noexcept;
    [[nodiscard]] Point midpoint() const noexcept;
    [[nodiscard]] Line supporting_line() const;
    [[nodiscard]] Vector2D direction() const noexcept;

    [[nodiscard]] bool contains(const Point& p, Real eps = Tolerance::absolute) const noexcept;
    [[nodiscard]] Real distance_to(const Point& p) const noexcept;
    [[nodiscard]] Point closest_point(const Point& p) const noexcept;

   private:
    Point start_;
    Point end_;
    EntityId id_{IdGenerator::next()};
};

}  // namespace ngine
