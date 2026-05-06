#pragma once

#include <ngine/core/point.hpp>

#include <cstdint>
#include <vector>

namespace ngine {

enum class IntersectionType : std::uint8_t {
    None,
    Point,
    TwoPoints,
    Segment,
    Coincident
};

struct IntersectionResult {
    IntersectionType type{IntersectionType::None};
    std::vector<Point> points;

    [[nodiscard]] bool has_intersection() const noexcept {
        return type != IntersectionType::None;
    }

    [[nodiscard]] std::size_t count() const noexcept { return points.size(); }

    [[nodiscard]] static IntersectionResult none() { return {}; }

    [[nodiscard]] static IntersectionResult single(Point p) {
        return {IntersectionType::Point, {std::move(p)}};
    }

    [[nodiscard]] static IntersectionResult two(Point p1, Point p2) {
        return {IntersectionType::TwoPoints, {std::move(p1), std::move(p2)}};
    }

    [[nodiscard]] static IntersectionResult coincident() {
        return {IntersectionType::Coincident, {}};
    }
};

}  // namespace ngine
