#pragma once

#include <ngine/spatial/spatial_index.hpp>

#include <array>
#include <memory>
#include <unordered_map>
#include <vector>

namespace ngine {

class Quadtree final : public SpatialIndex {
   public:
    struct Config {
        BoundingBox bounds;
        std::size_t max_objects_per_node;
        std::size_t max_depth;

        Config() : bounds{-1000, -1000, 1000, 1000}, max_objects_per_node{8}, max_depth{8} {}
    };

    explicit Quadtree(Config config = Config());

    void insert(EntityId id, const BoundingBox& bounds) override;
    void remove(EntityId id) override;
    void update(EntityId id, const BoundingBox& new_bounds) override;

    [[nodiscard]] std::vector<EntityId> query(const BoundingBox& region) const override;
    [[nodiscard]] std::vector<EntityId> query_radius(const Point& center,
                                                     Real radius) const override;

    void clear() override;
    [[nodiscard]] std::size_t size() const noexcept override;

   private:
    struct Entry {
        EntityId id;
        BoundingBox bounds;
    };

    struct Node {
        BoundingBox bounds;
        std::vector<Entry> entries;
        std::array<std::unique_ptr<Node>, 4> children;
        std::size_t depth{0};

        [[nodiscard]] bool is_leaf() const noexcept { return !children[0]; }
    };

    void insert_into_node(Node& node, const Entry& entry);
    void subdivide(Node& node);
    void query_node(const Node& node, const BoundingBox& region,
                    std::vector<EntityId>& results) const;
    [[nodiscard]] int get_quadrant(const Node& node, const BoundingBox& bounds) const;

    std::unique_ptr<Node> root_;
    Config config_;
    std::size_t size_{0};
    std::unordered_map<EntityId, BoundingBox> entries_;
};

}  // namespace ngine
