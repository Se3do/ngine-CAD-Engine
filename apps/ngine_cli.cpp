#include <ngine/interface/cli/cli_app.hpp>
#include <ngine/interface/document.hpp>
#include <ngine/logging/console_sink.hpp>
#include <ngine/logging/logger.hpp>

#include <iostream>
#include <memory>

int main() {
    ngine::Logger::instance().add_sink(std::make_shared<ngine::ConsoleSink>());
    ngine::Logger::instance().set_level(ngine::LogLevel::Warn);

    auto document = std::make_shared<ngine::Document>();
    ngine::CliApp app(document);

    std::cout << "ngine - Geometry Engine v1.0.0\n";
    std::cout << "Type HELP for commands, EXIT to quit.\n\n";

    app.run();
    return 0;
}
