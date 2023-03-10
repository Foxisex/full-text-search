#include <gtest/gtest.h>
#include <libfts/solver.hpp>

TEST(TestFTS, Test1) {
    const double first_operand = 22.5;
    const double second_operand = 12.6;
    double result = 0;

    result = fts::sum(first_operand, second_operand);

    ASSERT_TRUE(result == first_operand + second_operand);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}