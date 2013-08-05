#include "Tests.hpp"

TEST(CoreTests, Fill) {
    Image<float> * image = new Image<float>(10, 10);
    image->fill(-1.0f);

    const float * data = image->getData();
    for(int i = 0; i < 100; i++) {
        ASSERT_EQ(-1.0f, data[i]);
    }

    Volume<int> * volume = new Volume<int>(5,5,5);
    volume->fill(10);

    const int * data2 = volume->getData();
    for(int i = 0; i < 100; i++) {
        ASSERT_EQ(10, data2[i]);
    }
}
