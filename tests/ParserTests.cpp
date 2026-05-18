#include "../src/Config.h"
#include "../src/ConfigParser.h"
#include "../src/Node.h"
#include "../src/Binder.h"
#include "../src/ConfigView.h"
#include <gtest/gtest.h>

class ConfigParserTest : public ::testing::Test {
protected:
    void SetUp() override {}

    void TearDown() override {}

    public:
        std::string testConfigText = 
            "port=8080\n"
            "debug=true\n"
            "app_name=ConfigParser\n"
            "timeout=1.5\n"
            "db.host = localhost\n"
            "db.port = 5432\n"
            "a.b.c.d = nested_value\n";
};

TEST_F(ConfigParserTest, BindingAppConfig) {
    ConfigParser parser;
    Node root = parser.parseConfig(testConfigText);

    Binder binder(root);
    AppConfig cfg = binder.bind<AppConfig>();

    EXPECT_EQ(cfg.db.port, 5432);
    EXPECT_EQ(cfg.debug, true);
    EXPECT_EQ(cfg.db.host, "localhost");
}

TEST_F(ConfigParserTest, ConfigViewTest) {
    ConfigParser parser;
    Node root = parser.parseConfig(testConfigText);

    ConfigView view(root);

    auto port = view.get<int>("port");
    auto debug = view.get<bool>("debug");
    auto appName = view.get<std::string>("app_name");
    auto timeout = view.get<double>("timeout");
    auto dbHost = view.get<std::string>("db.host");
    auto dbPort = view.get<int>("db.port");
    auto nestedValue = view.get<std::string>("a.b.c.d");

    EXPECT_TRUE(port.has_value());
    EXPECT_TRUE(debug.has_value());
    EXPECT_TRUE(appName.has_value());
    EXPECT_TRUE(timeout.has_value());
    EXPECT_TRUE(dbHost.has_value());
    EXPECT_TRUE(dbPort.has_value());
    EXPECT_TRUE(nestedValue.has_value());

    EXPECT_EQ(port.value(), 8080);
    EXPECT_EQ(debug.value(), true);
    EXPECT_EQ(appName.value(), "ConfigParser");
    EXPECT_EQ(timeout.value(), 1.5);
    EXPECT_EQ(dbHost.value(), "localhost");
    EXPECT_EQ(dbPort.value(), 5432);
    EXPECT_EQ(nestedValue.value(), "nested_value");
}

