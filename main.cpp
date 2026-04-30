#include "Config.h"
#include "ConfigParser.h"
#include "AppConfig.h"
#include "Binder.h"
#include <iostream>

// Example application entry point for the ConfigParser project.
//
// Constructs a sample configuration text, parses it into a Config object,
// then reads typed values back from that object and prints them.
int main() {
    const std::string configText =
        "port=8080\n"
        "debug=true\n"
        "app_name=ConfigParser\n"
        "timeout=1.5\n"
        "db.host = localhost\n"
        "db.port = 5432\n"
        "a.b.c.d = nested_value\n";

    // Parse the sample configuration text.
    ConfigParser parser;
    Config cfg = parser.parseConfig(configText);

    // Retrieve typed values from the parsed configuration.
    int port = cfg.get<int>("port").value_or(0);
    bool debug = cfg.get<bool>("debug").value_or(false);
    std::string appName = cfg.get<std::string>("app_name").value_or("<unknown>");
    double timeout = cfg.get<double>("timeout").value_or(0.0);
    std::string unknownParameter = cfg.get<std::string>("unknown").value_or("<No such parameter>");
    int dbPort = cfg.get<int>("db.port").value_or(0);
    std::string wrongNestedValue = cfg.get<std::string>("a.b.c").value_or("<unknown>");
    std::string correctNestedValue = cfg.get<std::string>("a.b.c.d").value_or("<unknown>");

    Binder binder(cfg);
    const AppConfig appConfig = binder.bind<AppConfig>();

    // Print the configuration values to standard output.
    std::cout << "port = " << port << "\n";
    std::cout << "debug = " << std::boolalpha << debug << "\n";
    std::cout << "app_name = " << appName << "\n";
    std::cout << "timeout = " << timeout << "\n";
    std::cout << "unknown = " << unknownParameter << "\n";
    std::cout << "db.port = " << dbPort << "\n";
    std::cout << "a.b.c.d = " << correctNestedValue << "\n";
    std::cout << "a.b.c = " << wrongNestedValue << "\n";

    std::cout << "AppConfig.db.host = " << appConfig.db.host << "\n";
    std::cout << "AppConfig.db.port = " << appConfig.db.port << "\n";
    std::cout << "AppConfig.debug = " << std::boolalpha << appConfig.debug << "\n";
    return 0;
}
