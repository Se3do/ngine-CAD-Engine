#pragma once

#include <ngine/core/point.hpp>
#include <ngine/core/segment.hpp>
#include <ngine/core/types.hpp>

#include <vector>

namespace ngine {

class Polygon {
public:
    explicit Polygon(std::vector<Point> vertices);

    [[nodiscard]] const std::vector<Point>& vertices() const noexcept { return vertices_; }
    [[nodiscard]] std::size_t vertex_count() const noexcept { return vertices_.size(); }
    [[nodiscard]] EntityId id() const noexcept { return id_; }

    [[nodiscard]] std::vector<Segment> edges() const;
    [[nodiscard]] Real area() const noexcept;
    [[nodiscard]] Real perimeter() const noexcept;
    [[nodiscard]] Point centroid() const noexcept;
    [[nodiscard]] bool is_convex() const noexcept;
    [[nodiscard]] bool contains(const Point& p) const noexcept;
    [[nodiscard]] bool is_valid() const noexcept;

private:
    std::vector<Point> vertices_;
    EntityId id_{IdGenerator::next()};
};

}  // namespace ngine
