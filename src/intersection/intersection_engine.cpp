#include <ngine/intersection/intersection_engine.hpp>
#include <ngine/intersection/standard_intersection.hpp>

namespace ngine {

IntersectionEngine::IntersectionEngine() : strategy_(std::make_unique<StandardIntersection>()) {}

IntersectionEngine::IntersectionEngine(std::unique_ptr<IntersectionStrategy> strategy)
    : strategy_(std::move(strategy)) {}

void IntersectionEngine::set_strategy(std::unique_ptr<IntersectionStrategy> strategy) {
    strategy_ = std::move(strategy);
}

IntersectionResult IntersectionEngine::intersect(const Line& a, const Line& b) const {
    return strategy_->intersect_line_line(a, b);
}

IntersectionResult IntersectionEngine::intersect(const Line& line, const Circle& circle) const {
    return strategy_->intersect_line_circle(line, circle);
}

IntersectionResult IntersectionEngine::intersect(const Circle& a, const Circle& b) const {
    return strategy_->intersect_circle_circle(a, b);
}

IntersectionResult IntersectionEngine::intersect(const Segment& a, const Segment& b) const {
    return strategy_->intersect_segment_segment(a, b);
}

IntersectionResult IntersectionEngine::intersect(const Line& line, const Arc& arc) const {
    return strategy_->intersect_line_arc(line, arc);
}

IntersectionResult IntersectionEngine::intersect(const Circle& circle, const Arc& arc) const {
    return strategy_->intersect_circle_arc(circle, arc);
}

IntersectionResult IntersectionEngine::intersect(const Arc& a, const Arc& b) const {
    return strategy_->intersect_arc_arc(a, b);
}

}  // namespace ngine
