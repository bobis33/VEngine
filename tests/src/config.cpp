#include <gtest/gtest.h>

#include "VEngine/Utils/Parser.hpp"

ven::Config conf;

TEST(FUNCTION_MAP_OPT_LONG, width)
{
    ven::FUNCTION_MAP_OPT_LONG.at("width")(conf, "800");
    EXPECT_EQ(conf.window.width, 800);
}

TEST(FUNCTION_MAP_OPT_LONG, height)
{
    ven::FUNCTION_MAP_OPT_LONG.at("height")(conf, "900");
    EXPECT_EQ(conf.window.height, 900);
}

TEST(FUNCTION_MAP_OPT_LONG, fullscreen)
{
    ven::FUNCTION_MAP_OPT_LONG.at("fullscreen")(conf, "");
    EXPECT_EQ(conf.window.fullscreen, true);
}

TEST(FUNCTION_MAP_OPT_LONG, fov)
{
    ven::FUNCTION_MAP_OPT_LONG.at("fov")(conf, "80");
    EXPECT_EQ(conf.camera.fov, 80.0F);
}

TEST(FUNCTION_MAP_OPT_LONG, moveSpeed)
{
    ven::FUNCTION_MAP_OPT_LONG.at("mspeed")(conf, "2");
    EXPECT_EQ(conf.camera.move_speed, 2.0F);
}

TEST(FUNCTION_MAP_OPT_LONG, lookSpeed)
{
    ven::FUNCTION_MAP_OPT_LONG.at("lspeed")(conf, "5");
    EXPECT_EQ(conf.camera.look_speed, 5.0F);
}

TEST(FUNCTION_MAP_OPT_LONG, far)
{
    ven::FUNCTION_MAP_OPT_LONG.at("far")(conf, "33");
    EXPECT_EQ(conf.camera.far, 33.0F);
}

TEST(FUNCTION_MAP_OPT_LONG, near)
{
    ven::FUNCTION_MAP_OPT_LONG.at("near")(conf, "55");
    EXPECT_EQ(conf.camera.near, 55.0F);
}
