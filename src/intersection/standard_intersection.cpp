#include <ngine/intersection/standard_intersection.hpp>

#include <algorithm>
#include <cmath>

namespace ngine {

IntersectionResult StandardIntersection::intersect_line_line(const Line& a, const Line& b) const {
    Real det = a.a() * b.b() - a.b() * b.a();

    if (nearly_zero(det)) {
        Real dist = std::abs(a.c() - b.c());
        if (nearly_zero(dist) ||
            (a.is_parallel(b) && nearly_zero(a.distance(Point(0, 0)) - b.distance(Point(0, 0))))) {
            if (nearly_zero(a.a() * b.c() - b.a() * a.c()) &&
                nearly_zero(a.b() * b.c() - b.b() * a.c())) {
                return IntersectionResult::coincident();
            }
        }
        return IntersectionResult::none();
    }

    Real x = (a.b() * b.c() - b.b() * a.c()) / det;
    Real y = (b.a() * a.c() - a.a() * b.c()) / det;

    return IntersectionResult::single(Point(x, y));
}

IntersectionResult StandardIntersection::intersect_line_circle(const Line& line,
                                                               const Circle& circle) const {
    Real dist = line.distance(circle.center());
    Real radius = circle.radius();

    if (dist > radius + Tolerance::absolute) {
        return IntersectionResult::none();
    }

    Point foot = line.project(circle.center());

    if (nearly_equal(dist, radius)) {
        return IntersectionResult::single(foot);
    }

    Real half_chord = std::sqrt(radius * radius - dist * dist);
    Vector2D dir = line.direction();

    Point p1(foot.x() + dir.x() * half_chord, foot.y() + dir.y() * half_chord);
    Point p2(foot.x() - dir.x() * half_chord, foot.y() - dir.y() * half_chord);

    return IntersectionResult::two(p1, p2);
}

IntersectionResult StandardIntersection::intersect_circle_circle(const Circle& a,
                                                                 const Circle& b) const {
    Real dx = b.center().x() - a.center().x();
    Real dy = b.center().y() - a.center().y();
    Real d_sq = dx * dx + dy * dy;
    Real d = std::sqrt(d_sq);

    if (nearly_zero(d) && nearly_equal(a.radius(), b.radius())) {
        return IntersectionResult::coincident();
    }

    if (d > a.radius() + b.radius() + Tolerance::absolute) {
        return IntersectionResult::none();
    }

    if (d < std::abs(a.radius() - b.radius()) - Tolerance::absolute) {
        return IntersectionResult::none();
    }

    if (nearly_equal(d, a.radius() + b.radius()) ||
        nearly_equal(d, std::abs(a.radius() - b.radius()))) {
        Real t = a.radius() / d;
        Point p(a.center().x() + t * dx, a.center().y() + t * dy);
        if (nearly_equal(d, std::abs(a.radius() - b.radius()))) {
            if (a.radius() < b.radius()) {
                t = -a.radius() / d;
                p = Point(a.center().x() + t * dx, a.center().y() + t * dy);
            }
        }
        return IntersectionResult::single(p);
    }

    Real r1_sq = a.radius() * a.radius();
    Real r2_sq = b.radius() * b.radius();
    Real aa = (r1_sq - r2_sq + d_sq) / (2.0 * d);
    Real h = std::sqrt(r1_sq - aa * aa);

    Real mx = a.center().x() + aa * dx / d;
    Real my = a.center().y() + aa * dy / d;

    Point p1(mx + h * dy / d, my - h * dx / d);
    Point p2(mx - h * dy / d, my + h * dx / d);

    return IntersectionResult::two(p1, p2);
}

IntersectionResult StandardIntersection::intersect_segment_segment(const Segment& a,
                                                                   const Segment& b) const {
    Vector2D d1 = a.start().to(a.end());
    Vector2D d2 = b.start().to(b.end());

    Real cross = d1.cross(d2);
    Vector2D d3 = a.start().to(b.start());

    if (nearly_zero(cross)) {
        Real cross2 = d3.cross(d1);
        if (!nearly_zero(cross2)) {
            return IntersectionResult::none();
        }
        // Collinear — check overlap (simplified: return none for now if no single point)
        // Project onto parameterized line
        Real len_sq = d1.length_squared();
        if (nearly_zero(len_sq)) {
            return IntersectionResult::none();
        }
        Real t0 = d3.dot(d1) / len_sq;
        Vector2D d4 = a.start().to(b.end());
        Real t1 = d4.dot(d1) / len_sq;

        if (t0 > t1)
            std::swap(t0, t1);

        if (t1 < -Tolerance::absolute || t0 > 1.0 + Tolerance::absolute) {
            return IntersectionResult::none();
        }

        if (nearly_equal(std::max(t0, 0.0), std::min(t1, 1.0))) {
            Real t = std::clamp((t0 + t1) / 2.0, 0.0, 1.0);
            return IntersectionResult::single(
                Point(a.start().x() + t * d1.x(), a.start().y() + t * d1.y()));
        }

        return IntersectionResult::coincident();
    }

    Real t = d3.cross(d2) / cross;
    Real u = d3.cross(d1) / cross;

    if (t >= -Tolerance::absolute && t <= 1.0 + Tolerance::absolute && u >= -Tolerance::absolute &&
        u <= 1.0 + Tolerance::absolute) {
        t = std::clamp(t, 0.0, 1.0);
        return IntersectionResult::single(
            Point(a.start().x() + t * d1.x(), a.start().y() + t * d1.y()));
    }

    return IntersectionResult::none();
}

IntersectionResult StandardIntersection::intersect_line_arc(const Line& line,
                                                             const Arc& arc) const {
    // Compute line-circle intersection, then filter points to those on the arc
    Circle full_circle(arc.center(), arc.radius());
    IntersectionResult circle_result = intersect_line_circle(line, full_circle);

    if (circle_result.type == IntersectionType::None) {
        return IntersectionResult::none();
    }

    std::vector<Point> on_arc;
    for (const auto& p : circle_result.points) {
        if (arc.contains(p)) {
            on_arc.push_back(p);
        }
    }

    if (on_arc.empty()) {
        return IntersectionResult::none();
    }
    if (on_arc.size() == 1) {
        return IntersectionResult::single(on_arc[0]);
    }
    return IntersectionResult::two(on_arc[0], on_arc[1]);
}

IntersectionResult StandardIntersection::intersect_circle_arc(const Circle& circle,
                                                               const Arc& arc) const {
    // Compute circle-circle intersection, then filter points to those on the arc
    Circle arc_circle(arc.center(), arc.radius());
    IntersectionResult cc_result = intersect_circle_circle(circle, arc_circle);

    if (cc_result.type == IntersectionType::None) {
        return IntersectionResult::none();
    }

    if (cc_result.type == IntersectionType::Coincident) {
        // The full circles are coincident — the intersection is the arc itself
        // We can't represent a continuous arc segment in IntersectionResult,
        // so return coincident to indicate overlap.
        return IntersectionResult::coincident();
    }

    std::vector<Point> on_arc;
    for (const auto& p : cc_result.points) {
        if (arc.contains(p)) {
            on_arc.push_back(p);
        }
    }

    if (on_arc.empty()) {
        return IntersectionResult::none();
    }
    if (on_arc.size() == 1) {
        return IntersectionResult::single(on_arc[0]);
    }
    return IntersectionResult::two(on_arc[0], on_arc[1]);
}

IntersectionResult StandardIntersection::intersect_arc_arc(const Arc& a, const Arc& b) const {
    // Compute circle-circle intersection, then filter points to those on both arcs
    Circle ca(a.center(), a.radius());
    Circle cb(b.center(), b.radius());
    IntersectionResult cc_result = intersect_circle_circle(ca, cb);

    if (cc_result.type == IntersectionType::None) {
        return IntersectionResult::none();
    }

    if (cc_result.type == IntersectionType::Coincident) {
        // Full circles are coincident — check if the arc spans overlap
        // Simplified: return coincident (overlap detection is complex)
        return IntersectionResult::coincident();
    }

    std::vector<Point> on_both;
    for (const auto& p : cc_result.points) {
        if (a.contains(p) && b.contains(p)) {
            on_both.push_back(p);
        }
    }

    if (on_both.empty()) {
        return IntersectionResult::none();
    }
    if (on_both.size() == 1) {
        return IntersectionResult::single(on_both[0]);
    }
    return IntersectionResult::two(on_both[0], on_both[1]);
}

}  // namespace ngine

