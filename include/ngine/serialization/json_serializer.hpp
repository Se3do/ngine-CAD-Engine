#pragma once

#include <ngine/serialization/serializer.hpp>

namespace ngine {

class JsonSerializer final : public Serializer {
   public:
    [[nodiscard]] std::string serialize(const Document& doc) const override;
    [[nodiscard]] std::shared_ptr<Document> deserialize(std::string_view data) const override;

    void save_to_file(const Document& doc, const std::filesystem::path& path) const override;
    [[nodiscard]] std::shared_ptr<Document> load_from_file(
        const std::filesystem::path& path) const override;

    [[nodiscard]] std::string format_name() const override { return "JSON"; }
};

}  // namespace ngine
