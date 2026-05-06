#include <ngine/commands/geometry_commands.hpp>
#include <ngine/core/arc.hpp>
#include <ngine/core/circle.hpp>
#include <ngine/core/line.hpp>
#include <ngine/core/point.hpp>
#include <ngine/core/segment.hpp>
#include <ngine/interface/cli/cli_app.hpp>

#include <format>
#include <sstream>
#include <stdexcept>

namespace ngine {

CliApp::CliApp(std::shared_ptr<Document> document) : document_(std::move(document)) {
    register_handlers();
}

void CliApp::register_handlers() {
    handlers_["CREATE_POINT"] = [this](const ParsedCommand& c) { return handle_create_point(c); };
    handlers_["CREATE_LINE"] = [this](const ParsedCommand& c) { return handle_create_line(c); };
    handlers_["CREATE_CIRCLE"] = [this](const ParsedCommand& c) { return handle_create_circle(c); };
    handlers_["CREATE_SEGMENT"] = [this](const ParsedCommand& c) {
        return handle_create_segment(c);
    };
    handlers_["CREATE_ARC"] = [this](const ParsedCommand& c) { return handle_create_arc(c); };
    handlers_["INTERSECT"] = [this](const ParsedCommand& c) { return handle_intersect(c); };
    handlers_["LIST"] = [this](const ParsedCommand& c) { return handle_list(c); };
    handlers_["DELETE"] = [this](const ParsedCommand& c) { return handle_delete(c); };
    handlers_["MOVE"] = [this](const ParsedCommand& c) { return handle_move(c); };
    handlers_["UNDO"] = [this](const ParsedCommand& c) { return handle_undo(c); };
    handlers_["REDO"] = [this](const ParsedCommand& c) { return handle_redo(c); };
    handlers_["SOLVE"] = [this](const ParsedCommand& c) { return handle_solve(c); };
    handlers_["HELP"] = [this](const ParsedCommand& c) { return handle_help(c); };
}

void CliApp::run(std::istream& input, std::ostream& output) {
    std::string line;
    output << "ngine> ";
    while (std::getline(input, line)) {
        if (line == "EXIT" || line == "QUIT" || line == "exit" || line == "quit") {
            break;
        }

        auto parsed = parser_.parse(line);
        if (!parsed) {
            output << "ngine> ";
            continue;
        }

        std::string result = execute_command(*parsed);
        if (!result.empty()) {
            output << result << "\n";
        }
        output << "ngine> ";
    }
}

std::string CliApp::execute_command(const ParsedCommand& cmd) {
    auto it = handlers_.find(cmd.verb);
    if (it == handlers_.end()) {
        return std::format("ERROR: Unknown command '{}'", cmd.verb);
    }

    try {
        return it->second(cmd);
    } catch (const std::exception& e) {
        return std::format("ERROR: {}", e.what());
    }
}

std::string CliApp::handle_create_point(const ParsedCommand& cmd) {
    if (cmd.arguments.size() < 2) {
        return "ERROR: CREATE_POINT requires x y";
    }
    Real x = std::stod(cmd.arguments[0]);
    Real y = std::stod(cmd.arguments[1]);

    EntityId id = document_->add_entity(Point(x, y));
    return std::format("OK: Created point #{} at ({}, {})", id, x, y);
}

std::string CliApp::handle_create_line(const ParsedCommand& cmd) {
    if (cmd.arguments.size() < 4) {
        return "ERROR: CREATE_LINE requires x1 y1 x2 y2";
    }
    Real x1 = std::stod(cmd.arguments[0]);
    Real y1 = std::stod(cmd.arguments[1]);
    Real x2 = std::stod(cmd.arguments[2]);
    Real y2 = std::stod(cmd.arguments[3]);

    auto line = Line::from_points(Point(x1, y1), Point(x2, y2));
    EntityId id = document_->add_entity(line);
    return std::format("OK: Created line #{} through ({},{}) and ({},{})", id, x1, y1, x2, y2);
}

std::string CliApp::handle_create_circle(const ParsedCommand& cmd) {
    if (cmd.arguments.size() < 3) {
        return "ERROR: CREATE_CIRCLE requires cx cy radius";
    }
    Real cx = std::stod(cmd.arguments[0]);
    Real cy = std::stod(cmd.arguments[1]);
    Real r = std::stod(cmd.arguments[2]);

    EntityId id = document_->add_entity(Circle(Point(cx, cy), r));
    return std::format("OK: Created circle #{} center ({},{}) radius {}", id, cx, cy, r);
}

std::string CliApp::handle_create_segment(const ParsedCommand& cmd) {
    if (cmd.arguments.size() < 4) {
        return "ERROR: CREATE_SEGMENT requires x1 y1 x2 y2";
    }
    Real x1 = std::stod(cmd.arguments[0]);
    Real y1 = std::stod(cmd.arguments[1]);
    Real x2 = std::stod(cmd.arguments[2]);
    Real y2 = std::stod(cmd.arguments[3]);

    EntityId id = document_->add_entity(Segment(Point(x1, y1), Point(x2, y2)));
    return std::format("OK: Created segment #{}", id);
}

std::string CliApp::handle_create_arc(const ParsedCommand& cmd) {
    if (cmd.arguments.size() < 5) {
        return "ERROR: CREATE_ARC requires cx cy radius start_angle end_angle";
    }
    Real cx = std::stod(cmd.arguments[0]);
    Real cy = std::stod(cmd.arguments[1]);
    Real r = std::stod(cmd.arguments[2]);
    Real sa = std::stod(cmd.arguments[3]);
    Real ea = std::stod(cmd.arguments[4]);

    EntityId id = document_->add_entity(Arc(Point(cx, cy), r, sa, ea));
    return std::format("OK: Created arc #{} center ({},{}) radius {} angles [{},{}]", id, cx, cy, r,
                       sa, ea);
}

std::string CliApp::handle_intersect(const ParsedCommand& cmd) {
    if (cmd.arguments.size() < 2) {
        return "ERROR: INTERSECT requires two entity IDs";
    }
    EntityId id1 = std::stoull(cmd.arguments[0]);
    EntityId id2 = std::stoull(cmd.arguments[1]);

    auto* e1 = document_->get_entity(id1);
    auto* e2 = document_->get_entity(id2);
    if (!e1 || !e2) {
        return "ERROR: Entity not found";
    }

    auto& engine = document_->intersections();
    IntersectionResult result;

    auto try_intersect = [&]() -> bool {
        if (auto* l1 = std::get_if<Line>(e1)) {
            if (auto* l2 = std::get_if<Line>(e2)) {
                result = engine.intersect(*l1, *l2);
                return true;
            }
            if (auto* c2 = std::get_if<Circle>(e2)) {
                result = engine.intersect(*l1, *c2);
                return true;
            }
        }
        if (auto* c1 = std::get_if<Circle>(e1)) {
            if (auto* l2 = std::get_if<Line>(e2)) {
                result = engine.intersect(*l2, *c1);
                return true;
            }
            if (auto* c2 = std::get_if<Circle>(e2)) {
                result = engine.intersect(*c1, *c2);
                return true;
            }
        }
        if (auto* s1 = std::get_if<Segment>(e1)) {
            if (auto* s2 = std::get_if<Segment>(e2)) {
                result = engine.intersect(*s1, *s2);
                return true;
            }
        }
        if (auto* a1 = std::get_if<Arc>(e1)) {
            if (auto* l2 = std::get_if<Line>(e2)) {
                result = engine.intersect(*l2, *a1);
                return true;
            }
            if (auto* c2 = std::get_if<Circle>(e2)) {
                result = engine.intersect(*c2, *a1);
                return true;
            }
            if (auto* a2 = std::get_if<Arc>(e2)) {
                result = engine.intersect(*a1, *a2);
                return true;
            }
        }
        if (auto* l1 = std::get_if<Line>(e1)) {
            if (auto* a2 = std::get_if<Arc>(e2)) {
                result = engine.intersect(*l1, *a2);
                return true;
            }
        }
        if (auto* c1_2 = std::get_if<Circle>(e1)) {
            if (auto* a2 = std::get_if<Arc>(e2)) {
                result = engine.intersect(*c1_2, *a2);
                return true;
            }
        }
        return false;
    };

    if (!try_intersect()) {
        return "ERROR: Unsupported intersection pair";
    }

    if (!result.has_intersection()) {
        return "No intersection";
    }

    std::ostringstream oss;
    oss << "Intersection: " << result.count() << " point(s)";
    for (const auto& p : result.points) {
        oss << std::format("\n  ({}, {})", p.x(), p.y());
    }
    return oss.str();
}

std::string CliApp::handle_list(const ParsedCommand& /*cmd*/) {
    auto ids = document_->all_entity_ids();
    if (ids.empty()) {
        return "No entities";
    }

    std::ostringstream oss;
    oss << "Entities (" << ids.size() << "):";
    for (EntityId id : ids) {
        auto* entity = document_->get_entity(id);
        std::string type_name = std::visit(
            [](const auto& e) -> std::string {
                using T = std::decay_t<decltype(e)>;
                if constexpr (std::is_same_v<T, Point>) {
                    return "Point";
                } else if constexpr (std::is_same_v<T, Line>) {
                    return "Line";
                } else if constexpr (std::is_same_v<T, Segment>) {
                    return "Segment";
                } else if constexpr (std::is_same_v<T, Circle>) {
                    return "Circle";
                } else if constexpr (std::is_same_v<T, Polygon>) {
                    return "Polygon";
                } else if constexpr (std::is_same_v<T, Arc>) {
                    return "Arc";
                } else {
                    return "Unknown";
                }
            },
            *entity);
        oss << std::format("\n  #{}: {}", id, type_name);
    }
    return oss.str();
}

std::string CliApp::handle_delete(const ParsedCommand& cmd) {
    if (cmd.arguments.empty()) {
        return "ERROR: DELETE requires entity ID";
    }
    EntityId id = std::stoull(cmd.arguments[0]);
    if (!document_->get_entity(id)) {
        return "ERROR: Entity not found";
    }
    document_->remove_entity(id);
    return std::format("OK: Deleted entity #{}", id);
}

std::string CliApp::handle_move(const ParsedCommand& cmd) {
    if (cmd.arguments.size() < 3) {
        return "ERROR: MOVE requires id dx dy";
    }
    EntityId id = std::stoull(cmd.arguments[0]);
    Real dx = std::stod(cmd.arguments[1]);
    Real dy = std::stod(cmd.arguments[2]);

    auto* entity = document_->get_entity(id);
    if (!entity) {
        return "ERROR: Entity not found";
    }

    auto get_fn = [this](EntityId eid) -> GeometryEntity* { return document_->get_entity(eid); };
    auto move_cmd = std::make_unique<MoveCommand>(id, Vector2D(dx, dy), get_fn);
    document_->history().execute(std::move(move_cmd));

    return std::format("OK: Moved entity #{} by ({}, {})", id, dx, dy);
}

std::string CliApp::handle_undo(const ParsedCommand& /*cmd*/) {
    if (document_->history().undo()) {
        return "OK: Undo";
    }
    return "Nothing to undo";
}

std::string CliApp::handle_redo(const ParsedCommand& /*cmd*/) {
    if (document_->history().redo()) {
        return "OK: Redo";
    }
    return "Nothing to redo";
}

std::string CliApp::handle_solve(const ParsedCommand& /*cmd*/) {
    auto result = document_->constraints().solve();
    if (result.converged) {
        return std::format("OK: Converged in {} iterations (error: {:.2e})", result.iterations_used,
                           result.final_error);
    }
    return std::format("WARN: Did not converge after {} iterations (error: {:.2e})",
                       result.iterations_used, result.final_error);
}

std::string CliApp::handle_help(const ParsedCommand& /*cmd*/) {
    return "Commands:\n"
           "  CREATE_POINT x y\n"
           "  CREATE_LINE x1 y1 x2 y2\n"
           "  CREATE_CIRCLE cx cy radius\n"
           "  CREATE_SEGMENT x1 y1 x2 y2\n"
           "  CREATE_ARC cx cy radius start_angle end_angle\n"
           "  INTERSECT id1 id2\n"
           "  LIST\n"
           "  DELETE id\n"
           "  MOVE id dx dy\n"
           "  UNDO\n"
           "  REDO\n"
           "  SOLVE\n"
           "  HELP\n"
           "  EXIT";
}

}  // namespace ngine
