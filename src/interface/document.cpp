#include <ngine/interface/document.hpp>

namespace ngine {

Document::Document() = default;

EntityId Document::add_entity(GeometryEntity entity) {
    EntityId id = next_id_++;
    entities_.emplace(id, std::move(entity));
    return id;
}

void Document::remove_entity(EntityId id) {
    entities_.erase(id);
}

GeometryEntity* Document::get_entity(EntityId id) {
    auto it = entities_.find(id);
    return it != entities_.end() ? &it->second : nullptr;
}

const GeometryEntity* Document::get_entity(EntityId id) const {
    auto it = entities_.find(id);
    return it != entities_.end() ? &it->second : nullptr;
}

std::vector<EntityId> Document::all_entity_ids() const {
    std::vector<EntityId> ids;
    ids.reserve(entities_.size());
    for (const auto& [id, _] : entities_) {
        ids.push_back(id);
    }
    return ids;
}

std::size_t Document::entity_count() const noexcept {
    return entities_.size();
}

void Document::clear() {
    entities_.clear();
    history_.clear();
    constraints_.clear();
}

}  // namespace ngine
