#pragma once

#include <ngine/core/point.hpp>
#include <ngine/core/types.hpp>
#include <ngine/spatial/bounding_box.hpp>

#include <vector>

namespace ngine {

class SpatialIndex {
public:
    virtual ~SpatialIndex() = default;

    virtual void insert(EntityId id, const BoundingBox& bounds) = 0;
    virtual void remove(EntityId id) = 0;
    virtual void update(EntityId id, const BoundingBox& new_bounds) = 0;

    [[nodiscard]] virtual std::vector<EntityId> query(const BoundingBox& region) const = 0;
    [[nodiscard]] virtual std::vector<EntityId> query_radius(const Point& center,
                                                             Real radius) const = 0;

    virtual void clear() = 0;
    [[nodiscard]] virtual std::size_t size() const noexcept = 0;
};

}  // namespace ngine
