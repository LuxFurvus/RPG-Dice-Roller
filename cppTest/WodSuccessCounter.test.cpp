
#include "../app/src/main/cpp/WodCpp/WodSuccessCounter.h"

#include <algorithm>
#include <vector>

#include "gtest/gtest.h"

namespace
{

bool IsValidDieValue(const int RollValue)
{
    return RollValue >= 1
        && RollValue <= WodSuccessCounter::GlobalSideNumber;
}

int CountValue(
    const std::vector<int>& Rolls,
    const int Value)
{
    return static_cast<int>(std::ranges::count(Rolls, Value));
}

int CountSuccesses(
    const std::vector<int>& Rolls,
    const int Difficulty)
{
    return static_cast<int>(std::ranges::count_if(
        Rolls,
        [Difficulty](const int RollValue) -> bool
        {
            return RollValue >= Difficulty;
        }));
}

std::vector<int> GetInitialRolls(
    const std::vector<int>& FinalRolls,
    const int DiceCount)
{
    const int SafeDiceCount = std::max(DiceCount, 0);

    const std::size_t InitialRollCount = std::min(
        static_cast<std::size_t>(SafeDiceCount), FinalRolls.size());

    return std::vector<int>(
        FinalRolls.begin(), FinalRolls.begin() + static_cast<std::ptrdiff_t>(InitialRollCount));
}

int CalculateExpectedSuccessCount(
    const std::vector<int>& FinalRolls,
    const int DiceCount,
    const int Difficulty,
    const bool WithCancel,
    const int Modifier)
{
    const std::vector<int> InitialRolls = GetInitialRolls(
        FinalRolls, DiceCount);

    int SuccessCount = CountSuccesses(FinalRolls, Difficulty);

    const int InitialOneCount = CountValue(InitialRolls, 1);

    const bool HasSuccessBeforeCancel =
        SuccessCount > 0 || Modifier > 0;

    if (WithCancel)
    {
        SuccessCount -= InitialOneCount;
    }

    SuccessCount += Modifier;

    if (WithCancel
        && !HasSuccessBeforeCancel
        && InitialOneCount > 0
        && SuccessCount <= 0)
    {
        return -InitialOneCount;
    }

    return std::max(SuccessCount, 0);
}

void ExpectAllRollsAreValid(const std::vector<int>& Rolls)
{
    EXPECT_TRUE(std::ranges::all_of(
        Rolls, IsValidDieValue));
}

void ExpectSuccessCountMatchesReturnedRolls(
    const int DiceCount,
    const int Difficulty,
    const bool WithCancel,
    const bool WithTenReroll,
    const int Modifier)
{
    const std::pair<std::vector<int>, int> Result =
        WodSuccessCounter::GetSuccessesNum(DiceCount, Difficulty, WithCancel, WithTenReroll, Modifier);

    const std::vector<int>& Rolls = Result.first;
    const int SuccessCount = Result.second;

    const int ExpectedSuccessCount = CalculateExpectedSuccessCount(
        Rolls, DiceCount, Difficulty, WithCancel, Modifier);

    EXPECT_EQ(ExpectedSuccessCount, SuccessCount);
}

TEST(WodSuccessCounterTests_GetSuccessesNum, ReturnsRequestedRollCount_WhenTenRerollIsDisabled)
{
    constexpr int DiceCount = 5;

    const std::pair<std::vector<int>, int> Result =
        WodSuccessCounter::GetSuccessesNum(DiceCount, 6, false, false, 0);

    EXPECT_EQ(DiceCount, static_cast<int>(Result.first.size()));
}

TEST(WodSuccessCounterTests_GetSuccessesNum, ReturnsValidD10Values)
{
    const std::pair<std::vector<int>, int> Result =
        WodSuccessCounter::GetSuccessesNum(10, 6, true, true, 0);

    ExpectAllRollsAreValid(Result.first);
}

TEST(WodSuccessCounterTests_GetSuccessesNum, ReturnsSortedInitialRolls_WhenTenRerollIsDisabled)
{
    const std::pair<std::vector<int>, int> Result =
        WodSuccessCounter::GetSuccessesNum(8, 6, false, false, 0);

    EXPECT_TRUE(std::ranges::is_sorted(Result.first));
}

TEST(WodSuccessCounterTests_GetSuccessesNum, CalculatesSuccesses_WithoutCancelAndWithoutTenReroll)
{
    ExpectSuccessCountMatchesReturnedRolls(
        6, 6, false, false, 0);
}

TEST(WodSuccessCounterTests_GetSuccessesNum, CalculatesSuccesses_WithCancelAndWithoutTenReroll)
{
    ExpectSuccessCountMatchesReturnedRolls(
        6, 6, true, false, 0);
}

TEST(WodSuccessCounterTests_GetSuccessesNum, AppliesPositiveModifier)
{
    ExpectSuccessCountMatchesReturnedRolls(
        4, 8, false, false, 2);
}

TEST(WodSuccessCounterTests_GetSuccessesNum, AppliesNegativeModifier)
{
    ExpectSuccessCountMatchesReturnedRolls(
        4, 6, false, false, -2);
}

TEST(WodSuccessCounterTests_GetSuccessesNum, ClampsNegativeResultToZero_WhenCancelIsDisabled)
{
    const std::pair<std::vector<int>, int> Result =
        WodSuccessCounter::GetSuccessesNum(3, 6, false, false, -100);

    EXPECT_EQ(0, Result.second);
}

TEST(WodSuccessCounterTests_GetSuccessesNum, DoesNotReturnBotch_WhenModifierIsPositive)
{
    const std::pair<std::vector<int>, int> Result =
        WodSuccessCounter::GetSuccessesNum(10, 10, true, false, 1);

    EXPECT_GE(Result.second, 0);
}

TEST(WodSuccessCounterTests_GetSuccessesNum, CalculatesSuccesses_WithTenRerollWithoutCancel)
{
    ExpectSuccessCountMatchesReturnedRolls(
        10, 6, false, true, 0);
}

TEST(WodSuccessCounterTests_GetSuccessesNum, CalculatesSuccesses_WithTenRerollAndCancel)
{
    ExpectSuccessCountMatchesReturnedRolls(
        10, 6, true, true, 0);
}

TEST(WodSuccessCounterTests_GetSuccessesNum, AddsRerollValuesOnlyForInitialTens)
{
    constexpr int DiceCount = 20;

    const std::pair<std::vector<int>, int> Result =
        WodSuccessCounter::GetSuccessesNum(DiceCount, 6, false, true, 0);

    const std::vector<int> InitialRolls = GetInitialRolls(
        Result.first, DiceCount);

    const int InitialTenCount = CountValue(InitialRolls, 10);

    const int MinExpectedRollCount = DiceCount + InitialTenCount;

    const int MaxExpectedRollCount =
        DiceCount
        + InitialTenCount * (WodSuccessCounter::MaxTenReroll - 1);

    EXPECT_GE(static_cast<int>(Result.first.size()), MinExpectedRollCount);
    EXPECT_LE(static_cast<int>(Result.first.size()), MaxExpectedRollCount);
}

TEST(WodSuccessCounterTests_GetSuccessesNum, DoesNotAddRerolls_WhenTenRerollIsDisabled)
{
    constexpr int DiceCount = 20;

    const std::pair<std::vector<int>, int> Result =
        WodSuccessCounter::GetSuccessesNum(DiceCount, 6, false, false, 0);

    EXPECT_EQ(DiceCount, static_cast<int>(Result.first.size()));
}

TEST(WodSuccessCounterTests_GetSuccessesNum, DoesNotThrow_ForRepresentativeInputs)
{
    EXPECT_NO_THROW(WodSuccessCounter::GetSuccessesNum(
        1, 6, false, false, 0));

    EXPECT_NO_THROW(WodSuccessCounter::GetSuccessesNum(
        5, 6, true, false, 0));

    EXPECT_NO_THROW(WodSuccessCounter::GetSuccessesNum(
        10, 6, true, true, 1));

    EXPECT_NO_THROW(WodSuccessCounter::GetSuccessesNum(
        10, 10, true, true, -1));
}

}
