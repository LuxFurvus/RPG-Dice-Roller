
#include "../app/src/main/cpp/WodCpp/WodChanceCounter.h"

#include <string>

#include "gtest/gtest.h"

namespace
{
void ExpectProbabilityList(
    const std::string& Expected,
    const int DiceCount,
    const int Difficulty,
    const bool WithCancel,
    const bool WithTenReroll,
    const int Modifier)
{
    const std::string Result
        = WodChanceCounter::GetProbabilityList(
            DiceCount, Difficulty, WithCancel,
            WithTenReroll, Modifier);

    EXPECT_EQ(Expected, Result);
}

TEST(WodChanceCounterTests, GetProbabilityList_Returns_EmptyString_WhenDiceCountIsNegative)
{
    ExpectProbabilityList("",
        -1, 6, false, false, 0);
}

TEST(WodChanceCounterTests, GetProbabilityList_Returns_EmptyString_WhenDiceCountIsZero)
{
    ExpectProbabilityList("",
        0, 6, false, false, 0);
}

TEST(WodChanceCounterTests, GetProbabilityList_Returns_EmptyString_WhenDiceCountIsTooLarge)
{
    ExpectProbabilityList("",
        1000, 6, false, false, 0);
}

TEST(WodChanceCounterTests, GetProbabilityList_Returns_EmptyString_WhenDifficultyIsTooSmall)
{
    ExpectProbabilityList("",
        3, 1, false, false, 0);
}

TEST(WodChanceCounterTests, GetProbabilityList_Returns_EmptyString_WhenDifficultyIsTooLarge)
{
    ExpectProbabilityList("",
        3, 11, false, false, 0);
}

TEST(WodChanceCounterTests, GetProbabilityList_Returns_SingleSuccessLineForSingleDieWithoutCancel)
{
    ExpectProbabilityList("1: 50.00%",
        1, 6, false, false, 0);
}

TEST(WodChanceCounterTests, GetProbabilityList_Returns_BotchAndSuccessLinesForSingleDieWithCancel)
{
    ExpectProbabilityList("B: 10.00%\n1: 50.00%",
        1, 6, true, false, 0);
}

TEST(WodChanceCounterTests, GetProbabilityList_Returns_CumulativeSuccessLines_ForThreeDiceWithoutCancel)
{
    ExpectProbabilityList("1: 87.50%\n2: 50.00%\n3: 12.50%",
        3, 6, false, false, 0);
}

TEST(WodChanceCounterTests, GetProbabilityList_Returns_BotchAndCumulativeSuccessLines_ForThreeDiceWithCancel)
{
    ExpectProbabilityList("B: 6.10%\n1: 74.00%\n2: 42.50%\n3: 12.50%",
        3, 6, true, false, 0);
}

TEST(WodChanceCounterTests, GetProbabilityList_Returns_LowerSuccessChance_WhenCancelsCanRemoveSuccesses)
{
    ExpectProbabilityList("1: 19.00%\n2: 1.00%",
        2, 10, false, false, 0);

    ExpectProbabilityList("B: 17.00%\n1: 17.00%\n2: 1.00%",
        2, 10, true, false, 0);
}

TEST(WodChanceCounterTests, GetProbabilityList_DoesNotPrintBotch_WhenModifierIsPositive)
{
    ExpectProbabilityList("1: 92.40%\n2: 74.00%\n3: 42.50%\n4: 12.50%",
        3, 6, true, false, 1);
}

TEST(WodChanceCounterTests, GetProbabilityList_AppliesPositiveModifierToDisplayedSuccessTargets)
{
    ExpectProbabilityList("1: 100.00%\n2: 100.00%\n3: 50.00%",
        1, 6, false, false, 2);
}

TEST(WodChanceCounterTests, GetProbabilityList_SupportsTenReroll_ForSingleDieAtDifficultyTen)
{
    ExpectProbabilityList("1: 10.00%\n2: 1.00%\n3: 0.10%",
        1, 10, false, true, 0);
}

TEST(WodChanceCounterTests, GetProbabilityList_SupportsTenRerollWithCancel_ForRepresentativeInput)
{
    ExpectProbabilityList("B: 6.10%\n1: 75.22%\n2: 46.45%\n3: 18.66%\n4: 4.28%\n5: 0.77%\n6: 0.12%",
        3, 6, true, true, 0);
}

TEST(WodChanceCounterTests, GetProbabilityList_DoesNotThrow_ForRepresentativeInputs)
{
    EXPECT_NO_THROW(WodChanceCounter::GetProbabilityList(3, 6, false, false, 0));

    EXPECT_NO_THROW(WodChanceCounter::GetProbabilityList(3, 6, true, false, 0));

    EXPECT_NO_THROW(WodChanceCounter::GetProbabilityList(3, 6, true, true, 1));

    EXPECT_NO_THROW(WodChanceCounter::GetProbabilityList(-1, 6, true, true, 0));
}
}
