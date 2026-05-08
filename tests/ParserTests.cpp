#include "../src/Config.h"
#include "../src/ConfigParser.h"
#include "../src/Node.h"
#include "../src/Binder.h"
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

