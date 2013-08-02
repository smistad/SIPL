#include "Tests.hpp"

// Include all the tests here
#include "TypesTests.cpp"

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
