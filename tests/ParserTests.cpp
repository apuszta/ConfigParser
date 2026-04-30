#include "../src/Config.h"
#include "../src/ConfigParser.h"
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

TEST_F(ConfigParserTest, BasicDataParsing) {
    ConfigParser parser;
    Config cfg = parser.parseConfig(testConfigText);
    EXPECT_EQ(cfg.get<int>("port"), 8080);
    EXPECT_EQ(cfg.get<bool>("debug"), true);
    EXPECT_EQ(cfg.get<std::string>("app_name"), "ConfigParser");
    EXPECT_EQ(cfg.get<double>("timeout"), 1.5);
}

TEST_F(ConfigParserTest, NestedKeyParsing) {
    ConfigParser parser;
    Config cfg = parser.parseConfig(testConfigText);
    EXPECT_EQ(cfg.get<std::string>("db.host"), "localhost");
    EXPECT_EQ(cfg.get<int>("db.port"), 5432);
    EXPECT_EQ(cfg.get<std::string>("a.b.c.d"), "nested_value");
    EXPECT_FALSE(cfg.get<std::string>("a.b.c").has_value());
}
