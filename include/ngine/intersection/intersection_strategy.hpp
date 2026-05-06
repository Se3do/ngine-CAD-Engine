#pragma once

#include <ngine/core/circle.hpp>
#include <ngine/core/line.hpp>
#include <ngine/core/segment.hpp>
#include <ngine/intersection/intersection_result.hpp>

namespace ngine {

class IntersectionStrategy {
public:
    virtual ~IntersectionStrategy() = default;

    [[nodiscard]] virtual IntersectionResult intersect_line_line(const Line& a,
                                                                 const Line& b) const = 0;
    [[nodiscard]] virtual IntersectionResult intersect_line_circle(const Line& line,
                                                                    const Circle& circle) const = 0;
    [[nodiscard]] virtual IntersectionResult intersect_circle_circle(
        const Circle& a, const Circle& b) const = 0;
    [[nodiscard]] virtual IntersectionResult intersect_segment_segment(
        const Segment& a, const Segment& b) const = 0;
};

}  // namespace ngine
