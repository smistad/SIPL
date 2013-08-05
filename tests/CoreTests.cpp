#include "Tests.hpp"

TEST(CoreTests, Fill) {
    Image<float> * image = new Image<float>(3, 3);
    image->fill(-1.0f);

    const float * data = image->getData();
    for(int i = 0; i < 9; i++) {
        ASSERT_EQ(-1.0f, data[i]);
    }

    Volume<int> * volume = new Volume<int>(3,3,3);
    volume->fill(10);

    const int * data2 = volume->getData();
    for(int i = 0; i < 27; i++) {
        ASSERT_EQ(10, data2[i]);
    }
}

TEST(CoreTests, ImageOutOfBounds) {
    Image<char> * image = new Image<char>(10,10);
    // Getters
    EXPECT_THROW(image->get(-1), OutOfBoundsException);
    EXPECT_THROW(image->get(10*10), OutOfBoundsException);
    EXPECT_THROW(image->get(-1, 4), OutOfBoundsException);
    EXPECT_THROW(image->get(-1,-10), OutOfBoundsException);
    EXPECT_THROW(image->get(10, 0), OutOfBoundsException);
    EXPECT_THROW(image->get(2, 200), OutOfBoundsException);
    EXPECT_THROW(image->get(int2(-1, 4)), OutOfBoundsException);
    EXPECT_THROW(image->get(int2(0, 200)), OutOfBoundsException);
    EXPECT_THROW(image->get(Region(11, 11)), OutOfBoundsException);

    // Setters
    EXPECT_THROW(image->set(-1, 1), OutOfBoundsException);
    EXPECT_THROW(image->set(10*10, 1), OutOfBoundsException);
    EXPECT_THROW(image->set(-1, 4, 1), OutOfBoundsException);
    EXPECT_THROW(image->set(-1,-10, 1), OutOfBoundsException);
    EXPECT_THROW(image->set(10, 0, 1), OutOfBoundsException);
    EXPECT_THROW(image->set(2, 200, 1), OutOfBoundsException);
    EXPECT_THROW(image->set(int2(-1, 4), 1), OutOfBoundsException);
    EXPECT_THROW(image->set(int2(0, 200), 1), OutOfBoundsException);
    EXPECT_THROW(image->set(Region(11, 11), 1), OutOfBoundsException);
}

TEST(CoreTests, VolumeOutOfBounds) {
    Volume<char> * image = new Volume<char>(10,10, 10);

    // Getters
    EXPECT_THROW(image->get(-1), OutOfBoundsException);
    EXPECT_THROW(image->get(10*10*10), OutOfBoundsException);
    EXPECT_THROW(image->get(-4, 4, 2), OutOfBoundsException);
    EXPECT_THROW(image->get(-1,-10, 0), OutOfBoundsException);
    EXPECT_THROW(image->get(10, 0, 0), OutOfBoundsException);
    EXPECT_THROW(image->get(2, 1, -1), OutOfBoundsException);
    EXPECT_THROW(image->get(int3(-1, 4, 0)), OutOfBoundsException);
    EXPECT_THROW(image->get(int3(0, 200, 2)), OutOfBoundsException);
    EXPECT_THROW(image->get(Region(11, 11, 1)), OutOfBoundsException);

    // Setters
    EXPECT_THROW(image->set(-1,1), OutOfBoundsException);
    EXPECT_THROW(image->set(10*10*10,1), OutOfBoundsException);
    EXPECT_THROW(image->set(-4, 4, 2,1), OutOfBoundsException);
    EXPECT_THROW(image->set(-1,-10, 0,1), OutOfBoundsException);
    EXPECT_THROW(image->set(10, 0, 0,1), OutOfBoundsException);
    EXPECT_THROW(image->set(2, 1, -1,1), OutOfBoundsException);
    EXPECT_THROW(image->set(int3(-1, 4, 0),1), OutOfBoundsException);
    EXPECT_THROW(image->set(int3(0, 200, 2),1), OutOfBoundsException);
    EXPECT_THROW(image->set(Region(11, 11, 1),1), OutOfBoundsException);
}

TEST(CoreTests, ImageSpacingSetAndGet) {
    Image<char> * image = new Image<char>(1,1);
    image->setSpacing(float3(0.5, 0.1, 0.2));
    float3 spacing = image->getSpacing();
    EXPECT_FLOAT_EQ(0.5, spacing.x);
    EXPECT_FLOAT_EQ(0.1, spacing.y);
    EXPECT_FLOAT_EQ(0.2, spacing.z);
}

TEST(CoreTests, VolumeSpacingSetAndGet) {
    Volume<char> * image = new Volume<char>(1,1,1);
    image->setSpacing(float3(0.5, 0.1, 0.2));
    float3 spacing = image->getSpacing();
    EXPECT_FLOAT_EQ(0.5, spacing.x);
    EXPECT_FLOAT_EQ(0.1, spacing.y);
    EXPECT_FLOAT_EQ(0.2, spacing.z);

}
