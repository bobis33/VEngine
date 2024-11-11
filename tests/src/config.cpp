#include <gtest/gtest.h>

#include "VEngine/Utils/Parser.hpp"

TEST(config_window, width)
{
    ven::Config conf;
    ven::FUNCTION_MAP_OPT_LONG.at("width")(conf, "800");
    EXPECT_EQ(conf.window.width, 800);
}

TEST(config_window, height)
{
    ven::Config conf;
    ven::FUNCTION_MAP_OPT_LONG.at("height")(conf, "900");
    EXPECT_EQ(conf.window.height, 900);
}

TEST(config_window, fullscreen)
{
    ven::Config conf;
    ven::FUNCTION_MAP_OPT_LONG.at("fullscreen")(conf, "");
    EXPECT_EQ(conf.window.fullscreen, true);
}

TEST(config_camera, fov)
{
    ven::Config conf;
    ven::FUNCTION_MAP_OPT_LONG.at("fov")(conf, "80");
    EXPECT_EQ(conf.camera.fov, 80.0F);
}

TEST(config_camera, moveSpeed)
{
    ven::Config conf;
    ven::FUNCTION_MAP_OPT_LONG.at("mspeed")(conf, "2");
    EXPECT_EQ(conf.camera.move_speed, 2.0F);
}

TEST(config_camera, lookSpeed)
{
    ven::Config conf;
    ven::FUNCTION_MAP_OPT_LONG.at("lspeed")(conf, "5");
    EXPECT_EQ(conf.camera.look_speed, 5.0F);
}

TEST(config_camera, far)
{
    ven::Config conf;
    ven::FUNCTION_MAP_OPT_LONG.at("far")(conf, "33");
    EXPECT_EQ(conf.camera.far, 33.0F);
}

TEST(config_camera, near)
{
    ven::Config conf;
    ven::FUNCTION_MAP_OPT_LONG.at("near")(conf, "55");
    EXPECT_EQ(conf.camera.near, 55.0F);
}
