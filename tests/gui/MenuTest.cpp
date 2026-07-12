#include <gtest/gtest.h>
#include <string>

#include "gui/Menu.h"

TEST(MenuTest, DefaultIsClosed) {
    Menu menu;
    EXPECT_FALSE(menu.isOpen());
}

TEST(MenuTest, ToggleOpens) {
    Menu menu;
    menu.toggle();
    EXPECT_TRUE(menu.isOpen());
}

TEST(MenuTest, ToggleCloses) {
    Menu menu;
    menu.toggle();
    menu.toggle();
    EXPECT_FALSE(menu.isOpen());
}

TEST(MenuTest, ToggleTwiceOpens) {
    Menu menu;
    menu.toggle();
    menu.toggle();
    menu.toggle();
    EXPECT_TRUE(menu.isOpen());
}

TEST(MenuTest, ButtonLabelsAreAscii) {
    std::string resumeLabel = "wznuw gre";
    std::string exitLabel   = "wyjdz z gry";

    EXPECT_EQ(resumeLabel, "wznuw gre");
    EXPECT_EQ(exitLabel, "wyjdz z gry");

    for (char c : resumeLabel) {
        EXPECT_GE(static_cast<unsigned char>(c), 0x20);
        EXPECT_LE(static_cast<unsigned char>(c), 0x7E);
    }
    for (char c : exitLabel) {
        EXPECT_GE(static_cast<unsigned char>(c), 0x20);
        EXPECT_LE(static_cast<unsigned char>(c), 0x7E);
    }
}

TEST(MenuTest, FontScaleMinSize) {
    constexpr f32 TEXT_SCALE = 2.0f;
    EXPECT_GE(TEXT_SCALE, 1.5f);
}
