#pragma once

#include <ngine/interface/document.hpp>

#include <filesystem>
#include <string>
#include <string_view>

namespace ngine {

class Serializer {
public:
    virtual ~Serializer() = default;

    [[nodiscard]] virtual std::string serialize(const Document& doc) const = 0;
    [[nodiscard]] virtual std::shared_ptr<Document> deserialize(std::string_view data) const = 0;

    virtual void save_to_file(const Document& doc,
                              const std::filesystem::path& path) const = 0;
    [[nodiscard]] virtual std::shared_ptr<Document> load_from_file(
        const std::filesystem::path& path) const = 0;

    [[nodiscard]] virtual std::string format_name() const = 0;
};

}  // namespace ngine
