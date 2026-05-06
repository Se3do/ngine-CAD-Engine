#pragma once

#include <ngine/core/point.hpp>
#include <ngine/core/types.hpp>

#include <cmath>

namespace ngine {

class BoundingBox;

class Arc {
   public:
    /// Construct an arc. Angles in radians, counter-clockwise.
    /// start_angle and end_angle are normalized to [0, 2π).
    Arc(const Point& center, Real radius, Real start_angle, Real end_angle);

    [[nodiscard]] const Point& center() const noexcept { return center_; }
    [[nodiscard]] Real radius() const noexcept { return radius_; }
    [[nodiscard]] Real start_angle() const noexcept { return start_angle_; }
    [[nodiscard]] Real end_angle() const noexcept { return end_angle_; }
    [[nodiscard]] EntityId id() const noexcept { return id_; }

    /// The angular span of the arc (always positive, in (0, 2π]).
    [[nodiscard]] Real sweep_angle() const noexcept;

    /// Arc length.
    [[nodiscard]] Real length() const noexcept;

    /// Point at the start of the arc.
    [[nodiscard]] Point start_point() const noexcept;

    /// Point at the end of the arc.
    [[nodiscard]] Point end_point() const noexcept;

    /// Point at the angular midpoint of the arc.
    [[nodiscard]] Point midpoint() const noexcept;

    /// Point at parameter t ∈ [0, 1] along the arc.
    [[nodiscard]] Point point_at(Real t) const noexcept;

    /// Point at a given angle on the underlying circle.
    [[nodiscard]] Point point_at_angle(Real radians) const noexcept;

    /// Check whether an angle (radians) lies within the arc span.
    [[nodiscard]] bool contains_angle(Real angle) const noexcept;

    /// Check whether a point on the underlying circle lies on this arc.
    [[nodiscard]] bool contains(const Point& p, Real eps = Tolerance::absolute) const noexcept;

    /// Check whether a point lies on the arc boundary.
    [[nodiscard]] bool on_boundary(const Point& p, Real eps = Tolerance::absolute) const noexcept;

    /// Axis-aligned bounding box.
    [[nodiscard]] BoundingBox bounding_box() const noexcept;

    void set_center(const Point& center) noexcept { center_ = center; }
    void set_radius(Real radius);

   private:
    /// Normalize an angle to [0, 2π).
    [[nodiscard]] static Real normalize_angle(Real angle) noexcept;

    Point center_;
    Real radius_;
    Real start_angle_;
    Real end_angle_;
    EntityId id_{IdGenerator::next()};
};

}  // namespace ngine
