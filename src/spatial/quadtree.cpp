#include <ngine/spatial/quadtree.hpp>

#include <cmath>

namespace ngine {

Quadtree::Quadtree(Config config) : config_(config) {
    root_ = std::make_unique<Node>();
    root_->bounds = config_.bounds;
    root_->depth = 0;
}

void Quadtree::insert(EntityId id, const BoundingBox& bounds) {
    Entry entry{id, bounds};
    insert_into_node(*root_, entry);
    entries_[id] = bounds;
    ++size_;
}

void Quadtree::remove(EntityId id) {
    auto it = entries_.find(id);
    if (it == entries_.end()) return;

    entries_.erase(it);
    --size_;

    // Rebuild (simple approach — production would use lazy deletion)
    auto saved = entries_;
    clear();
    for (const auto& [eid, bounds] : saved) {
        insert(eid, bounds);
    }
}

void Quadtree::update(EntityId id, const BoundingBox& new_bounds) {
    remove(id);
    insert(id, new_bounds);
}

std::vector<EntityId> Quadtree::query(const BoundingBox& region) const {
    std::vector<EntityId> results;
    query_node(*root_, region, results);
    return results;
}

std::vector<EntityId> Quadtree::query_radius(const Point& center, Real radius) const {
    BoundingBox region(center.x() - radius, center.y() - radius, center.x() + radius,
                       center.y() + radius);

    auto candidates = query(region);
    std::vector<EntityId> results;

    Real r_sq = radius * radius;
    for (EntityId id : candidates) {
        auto it = entries_.find(id);
        if (it != entries_.end()) {
            Point box_center = it->second.center();
            Real dx = box_center.x() - center.x();
            Real dy = box_center.y() - center.y();
            if (dx * dx + dy * dy <= r_sq) {
                results.push_back(id);
            }
        }
    }

    return results;
}

void Quadtree::clear() {
    root_ = std::make_unique<Node>();
    root_->bounds = config_.bounds;
    root_->depth = 0;
    entries_.clear();
    size_ = 0;
}

std::size_t Quadtree::size() const noexcept {
    return size_;
}

void Quadtree::insert_into_node(Node& node, const Entry& entry) {
    if (!node.is_leaf()) {
        int quadrant = get_quadrant(node, entry.bounds);
        if (quadrant != -1) {
            insert_into_node(*node.children[static_cast<std::size_t>(quadrant)], entry);
            return;
        }
    }

    node.entries.push_back(entry);

    if (node.entries.size() > config_.max_objects_per_node &&
        node.depth < config_.max_depth && node.is_leaf()) {
        subdivide(node);

        auto old_entries = std::move(node.entries);
        node.entries.clear();

        for (const auto& e : old_entries) {
            int quadrant = get_quadrant(node, e.bounds);
            if (quadrant != -1) {
                insert_into_node(*node.children[static_cast<std::size_t>(quadrant)], e);
            } else {
                node.entries.push_back(e);
            }
        }
    }
}

void Quadtree::subdivide(Node& node) {
    Real mid_x = (node.bounds.min_x() + node.bounds.max_x()) / 2.0;
    Real mid_y = (node.bounds.min_y() + node.bounds.max_y()) / 2.0;

    node.children[0] = std::make_unique<Node>();
    node.children[0]->bounds = BoundingBox(node.bounds.min_x(), mid_y, mid_x, node.bounds.max_y());
    node.children[0]->depth = node.depth + 1;

    node.children[1] = std::make_unique<Node>();
    node.children[1]->bounds = BoundingBox(mid_x, mid_y, node.bounds.max_x(), node.bounds.max_y());
    node.children[1]->depth = node.depth + 1;

    node.children[2] = std::make_unique<Node>();
    node.children[2]->bounds = BoundingBox(node.bounds.min_x(), node.bounds.min_y(), mid_x, mid_y);
    node.children[2]->depth = node.depth + 1;

    node.children[3] = std::make_unique<Node>();
    node.children[3]->bounds = BoundingBox(mid_x, node.bounds.min_y(), node.bounds.max_x(), mid_y);
    node.children[3]->depth = node.depth + 1;
}

void Quadtree::query_node(const Node& node, const BoundingBox& region,
                           std::vector<EntityId>& results) const {
    if (!node.bounds.intersects(region)) return;

    for (const auto& entry : node.entries) {
        if (entry.bounds.intersects(region)) {
            results.push_back(entry.id);
        }
    }

    if (!node.is_leaf()) {
        for (const auto& child : node.children) {
            if (child) {
                query_node(*child, region, results);
            }
        }
    }
}

int Quadtree::get_quadrant(const Node& node, const BoundingBox& bounds) const {
    Real mid_x = (node.bounds.min_x() + node.bounds.max_x()) / 2.0;
    Real mid_y = (node.bounds.min_y() + node.bounds.max_y()) / 2.0;

    bool top = bounds.min_y() >= mid_y;
    bool bottom = bounds.max_y() <= mid_y;
    bool left = bounds.max_x() <= mid_x;
    bool right = bounds.min_x() >= mid_x;

    if (top && left) return 0;
    if (top && right) return 1;
    if (bottom && left) return 2;
    if (bottom && right) return 3;

    return -1;  // Spans multiple quadrants
}

}  // namespace ngine
