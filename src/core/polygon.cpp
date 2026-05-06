#include <ngine/core/polygon.hpp>

#include <cmath>
#include <stdexcept>

namespace ngine {

Polygon::Polygon(std::vector<Point> vertices) : vertices_(std::move(vertices)) {
    if (vertices_.size() < 3) {
        throw std::invalid_argument("Polygon requires at least 3 vertices");
    }
}

std::vector<Segment> Polygon::edges() const {
    std::vector<Segment> result;
    result.reserve(vertices_.size());
    for (std::size_t i = 0; i < vertices_.size(); ++i) {
        std::size_t next = (i + 1) % vertices_.size();
        result.emplace_back(vertices_[i], vertices_[next]);
    }
    return result;
}

Real Polygon::area() const noexcept {
    Real sum = 0.0;
    std::size_t n = vertices_.size();
    for (std::size_t i = 0; i < n; ++i) {
        std::size_t j = (i + 1) % n;
        sum += vertices_[i].x() * vertices_[j].y();
        sum -= vertices_[j].x() * vertices_[i].y();
    }
    return std::abs(sum) / 2.0;
}

Real Polygon::perimeter() const noexcept {
    Real sum = 0.0;
    std::size_t n = vertices_.size();
    for (std::size_t i = 0; i < n; ++i) {
        std::size_t j = (i + 1) % n;
        sum += vertices_[i].distance_to(vertices_[j]);
    }
    return sum;
}

Point Polygon::centroid() const noexcept {
    Real cx = 0.0;
    Real cy = 0.0;
    Real signed_area = 0.0;
    std::size_t n = vertices_.size();

    for (std::size_t i = 0; i < n; ++i) {
        std::size_t j = (i + 1) % n;
        Real cross = vertices_[i].x() * vertices_[j].y() - vertices_[j].x() * vertices_[i].y();
        signed_area += cross;
        cx += (vertices_[i].x() + vertices_[j].x()) * cross;
        cy += (vertices_[i].y() + vertices_[j].y()) * cross;
    }

    signed_area /= 2.0;
    cx /= (6.0 * signed_area);
    cy /= (6.0 * signed_area);

    return Point(cx, cy);
}

bool Polygon::is_convex() const noexcept {
    std::size_t n = vertices_.size();
    if (n < 3) return false;

    bool positive = false;
    bool negative = false;

    for (std::size_t i = 0; i < n; ++i) {
        std::size_t j = (i + 1) % n;
        std::size_t k = (i + 2) % n;

        Vector2D v1 = vertices_[i].to(vertices_[j]);
        Vector2D v2 = vertices_[j].to(vertices_[k]);
        Real cross = v1.cross(v2);

        if (cross > Tolerance::absolute) positive = true;
        if (cross < -Tolerance::absolute) negative = true;
        if (positive && negative) return false;
    }

    return true;
}

bool Polygon::contains(const Point& p) const noexcept {
    // Winding number algorithm
    int winding = 0;
    std::size_t n = vertices_.size();

    for (std::size_t i = 0; i < n; ++i) {
        std::size_t j = (i + 1) % n;
        const Point& v1 = vertices_[i];
        const Point& v2 = vertices_[j];

        if (v1.y() <= p.y()) {
            if (v2.y() > p.y()) {
                Vector2D edge = v1.to(v2);
                Vector2D to_point = v1.to(p);
                if (edge.cross(to_point) > 0) {
                    ++winding;
                }
            }
        } else {
            if (v2.y() <= p.y()) {
                Vector2D edge = v1.to(v2);
                Vector2D to_point = v1.to(p);
                if (edge.cross(to_point) < 0) {
                    --winding;
                }
            }
        }
    }

    return winding != 0;
}

bool Polygon::is_valid() const noexcept {
    if (vertices_.size() < 3) return false;

    // Check for duplicate consecutive vertices
    for (std::size_t i = 0; i < vertices_.size(); ++i) {
        std::size_t j = (i + 1) % vertices_.size();
        if (vertices_[i].coincident(vertices_[j])) return false;
    }

    return true;
}

}  // namespace ngine
