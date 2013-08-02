#include "Tests.hpp"

TEST(TypesTests, DefaultConstructors) {

    float2 v;
    EXPECT_EQ(0.0f, v.x);
    EXPECT_EQ(0.0f, v.y);

    float3 v2;
    EXPECT_EQ(0.0f, v2.x);
    EXPECT_EQ(0.0f, v2.y);
    EXPECT_EQ(0.0f, v2.z);

    int2 v3;
    EXPECT_EQ(0, v3.x);
    EXPECT_EQ(0, v3.y);

    int3 v4;
    EXPECT_EQ(0, v4.x);
    EXPECT_EQ(0, v4.y);
    EXPECT_EQ(0, v4.z);
}

TEST(TypesTests, ValuedConstructors) {
    float2 f2(1.1f, -2.0f);
    EXPECT_EQ(1.1f, f2.x);
    EXPECT_EQ(-2.0f, f2.y);

    float3 f3(1.2f, -2.0f, 4.0f);
    EXPECT_EQ(1.2f, f3.x);
    EXPECT_EQ(-2.0f, f3.y);
    EXPECT_EQ(4.0f, f3.z);

    int2 i2(-2, 3.2f);
    EXPECT_EQ(-2, i2.x);
    EXPECT_EQ(3, i2.y);

    int3 i3(-2, 2000, 0);
    EXPECT_EQ(-2, i3.x);
    EXPECT_EQ(2000, i3.y);
    EXPECT_EQ(0, i3.z);
}
