
#include "../app/src/main/cpp/CommonCpp/CommonMathLib.h"

#include "gtest/gtest.h"

#include <cmath>

namespace
{
void ExpectLongDoubleNear(
    const long double Actual,
    const long double Expected,
    const long double Epsilon = 0.0000000001L)
{
    const long double Difference = std::fabs(Actual - Expected);

    EXPECT_LE(Difference, Epsilon);
}

TEST(CommonMathLibTests, BinomialCoefficient_ReturnsZero_WhenKIsNegative)
{
    const long double Result = CommonMathLib::BinomialCoefficient(5, -1);

    EXPECT_EQ(Result, 0.0L);
}

TEST(CommonMathLibTests, BinomialCoefficient_ReturnsZero_WhenKIsGreaterThanN)
{
    const long double Result = CommonMathLib::BinomialCoefficient(5, 6);

    EXPECT_EQ(Result, 0.0L);
}

TEST(CommonMathLibTests, BinomialCoefficient_ReturnsZero_WhenNIsNegative)
{
    const long double Result = CommonMathLib::BinomialCoefficient(-5, 0);

    EXPECT_EQ(Result, 0.0L);
}

TEST(CommonMathLibTests, BinomialCoefficient_ReturnsOne_WhenNAndKAreZero)
{
    const long double Result = CommonMathLib::BinomialCoefficient(0, 0);

    EXPECT_EQ(Result, 1.0L);
}

TEST(CommonMathLibTests, BinomialCoefficient_ReturnsOne_WhenKIsZero)
{
    const long double Result = CommonMathLib::BinomialCoefficient(10, 0);

    EXPECT_EQ(Result, 1.0L);
}

TEST(CommonMathLibTests, BinomialCoefficient_ReturnsOne_WhenKEqualsN)
{
    const long double Result = CommonMathLib::BinomialCoefficient(10, 10);

    EXPECT_EQ(Result, 1.0L);
}

TEST(CommonMathLibTests, BinomialCoefficient_ReturnsN_WhenKIsOne)
{
    const long double Result = CommonMathLib::BinomialCoefficient(12, 1);

    EXPECT_EQ(Result, 12.0L);
}

TEST(CommonMathLibTests, BinomialCoefficient_ReturnsN_WhenKIsNMinusOne)
{
    const long double Result = CommonMathLib::BinomialCoefficient(12, 11);

    EXPECT_EQ(Result, 12.0L);
}

TEST(CommonMathLibTests, BinomialCoefficient_ReturnsKnownValue_ForFiveChooseTwo)
{
    const long double Result = CommonMathLib::BinomialCoefficient(5, 2);

    EXPECT_EQ(Result, 10.0L);
}

TEST(CommonMathLibTests, BinomialCoefficient_ReturnsKnownValue_ForSixChooseThree)
{
    const long double Result = CommonMathLib::BinomialCoefficient(6, 3);

    EXPECT_EQ(Result, 20.0L);
}

TEST(CommonMathLibTests, BinomialCoefficient_ReturnsKnownValue_ForTenChooseThree)
{
    const long double Result = CommonMathLib::BinomialCoefficient(10, 3);

    EXPECT_EQ(Result, 120.0L);
}

TEST(CommonMathLibTests, BinomialCoefficient_ReturnsKnownValue_ForTwentyChooseTen)
{
    const long double Result = CommonMathLib::BinomialCoefficient(20, 10);

    EXPECT_EQ(Result, 184756.0L);
}

TEST(CommonMathLibTests, BinomialCoefficient_ReturnsKnownValue_ForThirtyChooseFifteen)
{
    const long double Result = CommonMathLib::BinomialCoefficient(30, 15);

    EXPECT_EQ(Result, 155117520.0L);
}

TEST(CommonMathLibTests, BinomialCoefficient_ReturnsSymmetricValue_ForKAndNMinusK)
{
    constexpr int N = 25;
    constexpr int K = 7;

    const long double LeftResult = CommonMathLib::BinomialCoefficient(N, K);
    const long double RightResult = CommonMathLib::BinomialCoefficient(N, N - K);

    EXPECT_EQ(LeftResult, RightResult);
}

TEST(CommonMathLibTests, BinomialCoefficient_HandlesLargeValidInput)
{
    const long double Result = CommonMathLib::BinomialCoefficient(50, 25);

    ExpectLongDoubleNear(Result, 126410606437752.0L);
}

TEST(CommonMathLibTests, BinomialCoefficient_ReturnsPositiveValue_ForLargeValidInput)
{
    const long double Result = CommonMathLib::BinomialCoefficient(60, 30);

    EXPECT_GT(Result, 0.0L);
}

TEST(CommonMathLibTests, BinomialCoefficient_ResultIncreasesWithinSmallValidRange)
{
    const long double LowerResult = CommonMathLib::BinomialCoefficient(8, 2);
    const long double HigherResult = CommonMathLib::BinomialCoefficient(8, 3);

    EXPECT_LT(LowerResult, HigherResult);
}
}  // namespace

