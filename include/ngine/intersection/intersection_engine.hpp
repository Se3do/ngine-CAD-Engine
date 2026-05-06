#pragma once

#include <ngine/core/circle.hpp>
#include <ngine/core/line.hpp>
#include <ngine/core/segment.hpp>
#include <ngine/intersection/intersection_result.hpp>
#include <ngine/intersection/intersection_strategy.hpp>

#include <memory>

namespace ngine {

class IntersectionEngine {
   public:
    IntersectionEngine();
    explicit IntersectionEngine(std::unique_ptr<IntersectionStrategy> strategy);

    void set_strategy(std::unique_ptr<IntersectionStrategy> strategy);

    [[nodiscard]] IntersectionResult intersect(const Line& a, const Line& b) const;
    [[nodiscard]] IntersectionResult intersect(const Line& line, const Circle& circle) const;
    [[nodiscard]] IntersectionResult intersect(const Circle& a, const Circle& b) const;
    [[nodiscard]] IntersectionResult intersect(const Segment& a, const Segment& b) const;

   private:
    std::unique_ptr<IntersectionStrategy> strategy_;
};

}  // namespace ngine
