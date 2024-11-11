#include <gtest/gtest.h>

int add(int a, int b) {
    return a + b;
}

TEST(AddTest, PositiveNumbers)
{
    EXPECT_EQ(add(2, 3), 5);
}

TEST(AddTest, NegativeNumbers)
{
    EXPECT_EQ(add(-1, -1), -2);
}

TEST(AddTest, Zero)
{
    EXPECT_EQ(add(0, 0), 0);
}
