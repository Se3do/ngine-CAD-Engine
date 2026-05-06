#include <ngine/serialization/json_serializer.hpp>

#include <fstream>
#include <nlohmann/json.hpp>
#include <stdexcept>

namespace ngine {

using json = nlohmann::json;

namespace {

json point_to_json(const Point& p) {
    return {{"type", "point"}, {"x", p.x()}, {"y", p.y()}};
}

json line_to_json(const Line& l) {
    return {{"type", "line"}, {"a", l.a()}, {"b", l.b()}, {"c", l.c()}};
}

json segment_to_json(const Segment& s) {
    return {{"type", "segment"},
            {"x1", s.start().x()},
            {"y1", s.start().y()},
            {"x2", s.end().x()},
            {"y2", s.end().y()}};
}

json circle_to_json(const Circle& c) {
    return {{"type", "circle"}, {"cx", c.center().x()}, {"cy", c.center().y()}, {"r", c.radius()}};
}

json polygon_to_json(const Polygon& p) {
    json vertices = json::array();
    for (const auto& v : p.vertices()) {
        vertices.push_back({{"x", v.x()}, {"y", v.y()}});
    }
    return {{"type", "polygon"}, {"vertices", vertices}};
}

json entity_to_json(EntityId id, const GeometryEntity& entity) {
    json j = std::visit(
        [](const auto& e) -> json {
            using T = std::decay_t<decltype(e)>;
            if constexpr (std::is_same_v<T, Point>)
                return point_to_json(e);
            else if constexpr (std::is_same_v<T, Line>)
                return line_to_json(e);
            else if constexpr (std::is_same_v<T, Segment>)
                return segment_to_json(e);
            else if constexpr (std::is_same_v<T, Circle>)
                return circle_to_json(e);
            else if constexpr (std::is_same_v<T, Polygon>)
                return polygon_to_json(e);
            else
                return {};
        },
        entity);
    j["id"] = id;
    return j;
}

GeometryEntity json_to_entity(const json& j) {
    std::string type = j.at("type").get<std::string>();

    if (type == "point") {
        return Point(j.at("x").get<Real>(), j.at("y").get<Real>());
    }
    if (type == "line") {
        return Line::from_coefficients(j.at("a").get<Real>(), j.at("b").get<Real>(),
                                       j.at("c").get<Real>());
    }
    if (type == "segment") {
        return Segment(Point(j.at("x1").get<Real>(), j.at("y1").get<Real>()),
                       Point(j.at("x2").get<Real>(), j.at("y2").get<Real>()));
    }
    if (type == "circle") {
        return Circle(Point(j.at("cx").get<Real>(), j.at("cy").get<Real>()), j.at("r").get<Real>());
    }
    if (type == "polygon") {
        std::vector<Point> vertices;
        for (const auto& v : j.at("vertices")) {
            vertices.emplace_back(v.at("x").get<Real>(), v.at("y").get<Real>());
        }
        return Polygon(std::move(vertices));
    }

    throw std::runtime_error("Unknown entity type: " + type);
}

}  // namespace

std::string JsonSerializer::serialize(const Document& doc) const {
    json root;
    root["version"] = "1.0";
    root["entities"] = json::array();

    for (EntityId id : doc.all_entity_ids()) {
        const auto* entity = doc.get_entity(id);
        if (entity) {
            root["entities"].push_back(entity_to_json(id, *entity));
        }
    }

    return root.dump(2);
}

std::shared_ptr<Document> JsonSerializer::deserialize(std::string_view data) const {
    auto doc = std::make_shared<Document>();
    json root = json::parse(data);

    for (const auto& entity_json : root.at("entities")) {
        doc->add_entity(json_to_entity(entity_json));
    }

    return doc;
}

void JsonSerializer::save_to_file(const Document& doc, const std::filesystem::path& path) const {
    std::ofstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file for writing: " + path.string());
    }
    file << serialize(doc);
}

std::shared_ptr<Document> JsonSerializer::load_from_file(const std::filesystem::path& path) const {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file for reading: " + path.string());
    }
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return deserialize(content);
}

}  // namespace ngine
