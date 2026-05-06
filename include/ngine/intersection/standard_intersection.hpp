#pragma once

#include <ngine/intersection/intersection_strategy.hpp>

namespace ngine {

class StandardIntersection final : public IntersectionStrategy {
public:
    [[nodiscard]] IntersectionResult intersect_line_line(const Line& a,
                                                         const Line& b) const override;
    [[nodiscard]] IntersectionResult intersect_line_circle(const Line& line,
                                                            const Circle& circle) const override;
    [[nodiscard]] IntersectionResult intersect_circle_circle(const Circle& a,
                                                              const Circle& b) const override;
    [[nodiscard]] IntersectionResult intersect_segment_segment(const Segment& a,
                                                                const Segment& b) const override;
};

}  // namespace ngine
