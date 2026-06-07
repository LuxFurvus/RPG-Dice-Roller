#include "../app/src/main/cpp/RollAndKeepCpp/RollAndKeepChanceCounter.h"

#include "gtest/gtest.h"

#include <string>

namespace
{
void ExpectProbabilityList(
    const std::string& Expected,
    const int RollNum,
    const int KeepNum,
    const int Bonus,
    const bool ExplodeTens,
    const bool ExplodeOnes)
{
    const std::string Result
        = RollAndKeepChanceCounter::GetProbabilityList(
            RollNum, KeepNum, Bonus, ExplodeTens, ExplodeOnes);

    EXPECT_EQ(Expected, Result);
}

TEST(RollAndKeepChanceCounterLibTests,
    GetProbabilityListReturnsExpectedTextForSingleDieWithoutExplosions)
{
    ExpectProbabilityList(
        " 5: 60.00%\n10: 10.00%\n15: 00.00%",
        1, 1, 0, false, false);
}

TEST(RollAndKeepChanceCounterLibTests,
    GetProbabilityListReturnsExpectedTextForSingleDieWithExplodingTens)
{
    ExpectProbabilityList(
        " 5: 60.00%\n10: 10.00%\n15: 6.00%\n20: 1.00%\n25: 0.60%\n30: 0.10%\n35: 0.06%\n40: 0.01%\n45: <0.01%",
        1, 1, 0, true, false);
}

TEST(RollAndKeepChanceCounterLibTests,
    GetProbabilityListReturnsExpectedTextForSingleDieWithExplodingOnes)
{
    ExpectProbabilityList(
        " 5: 66.00%\n10: 11.00%\n15: 00.00%",
        1, 1, 0, false, true);
}

TEST(RollAndKeepChanceCounterLibTests,
    GetProbabilityListReturnsExpectedTextForSingleDieWithExplodingTensAndOnes)
{
    ExpectProbabilityList(
        " 5: 66.00%\n10: 11.00%\n15: 6.60%\n20: 1.10%\n25: 0.66%\n30: 0.11%\n35: 0.07%\n40: 0.01%\n45: <0.01%",
        1, 1, 0, true, true);
}

TEST(RollAndKeepChanceCounterLibTests,
    GetProbabilityListReturnsExpectedTextForSingleDieWithPositiveBonus)
{
    ExpectProbabilityList(
        " 5: 100.0%\n10: 50.00%\n15: 00.00%",
        1, 1, 4, false, false);
}

TEST(RollAndKeepChanceCounterLibTests,
    GetProbabilityListReturnsExpectedTextWhenKeepNumExceedsRollNum)
{
    ExpectProbabilityList(
        " 5: 80.00%\n10: 30.00%\n15: 00.00%",
        1, 2, 0, false, false);
}

TEST(RollAndKeepChanceCounterLibTests,
    GetProbabilityListReturnsExpectedTextWhenRollNumIsZero)
{
    ExpectProbabilityList(
        " 5: 00.00%",
        0, 1, 0, false, false);
}

TEST(RollAndKeepChanceCounterLibTests,
    GetProbabilityListReturnsExpectedTextWhenKeepNumIsZero)
{
    ExpectProbabilityList(
        " 5: 00.00%",
        1, 0, 0, false, false);
}

TEST(RollAndKeepChanceCounterLibTests,
    GetProbabilityListDoesNotThrowForRepresentativeInputs)
{
    EXPECT_NO_THROW(RollAndKeepChanceCounter::GetProbabilityList(1, 1, 0, false, false));

    EXPECT_NO_THROW(RollAndKeepChanceCounter::GetProbabilityList(1, 1, 0, true, false));

    EXPECT_NO_THROW(RollAndKeepChanceCounter::GetProbabilityList(1, 1, 0, false, true));

    EXPECT_NO_THROW(RollAndKeepChanceCounter::GetProbabilityList(1, 1, 0, true, true));
}
}