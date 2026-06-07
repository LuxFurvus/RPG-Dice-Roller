#include "../app/src/main/cpp/RollAndKeepCpp/RollAndKeepRulesLib.h"
#include "../app/src/main/cpp/RollAndKeepCpp/RollAndKeepChanceCounter.h"

#include <algorithm>
#include <numeric>
#include <tuple>
#include <vector>

#include "gtest/gtest.h"

namespace
{

constexpr int SideNumber = 10;

bool IsValidNonExplodedDieValue(const int RollValue)
{
    return RollValue >= 1 && RollValue <= SideNumber;
}

bool IsValidExplodedDieValue(const int RollValue)
{
    return RollValue >= 1;
}

int GetExpectedKeptSum(
    const std::vector<int>& Rolls,
    const int KeepNum,
    const int Bonus)
{
    if (Rolls.empty() || KeepNum <= 0)
    {
        return Bonus;
    }

    const int CorrectedKeepNum =
        std::min(KeepNum, static_cast<int>(Rolls.size()));

    return std::accumulate(
        Rolls.end() - CorrectedKeepNum, Rolls.end(), Bonus);
}

void ExpectRollsAreSortedAscending(const std::vector<int>& Rolls)
{
    EXPECT_TRUE(std::ranges::is_sorted(Rolls));
}

void ExpectAllRollsAreValidWithoutExplosions(const std::vector<int>& Rolls)
{
    EXPECT_TRUE(std::ranges::all_of(
        Rolls, IsValidNonExplodedDieValue));
}

void ExpectAllRollsAreValidWithExplosions(const std::vector<int>& Rolls)
{
    EXPECT_TRUE(std::ranges::all_of(
        Rolls, IsValidExplodedDieValue));
}

void ExpectRollAndKeepResultMatchesReturnedRolls(
    const int RollNum,
    const int KeepNum,
    const int Bonus,
    const bool ExplodeTens,
    const bool ExplodeOnes)
{
    const FValueParamsRnk NormalizedParams =
        RollAndKeepChanceCounter::ApplyTenDiceRule(
            FValueParamsRnk
            {
                .InRollNum = RollNum,     .InKeepNum = KeepNum,     .InBonus = Bonus
            });

    const std::tuple<std::vector<int>, int> Result =
        RollAndKeepRulesLib::GetRollAndKeepResults(
            RollNum, KeepNum, Bonus, ExplodeTens, ExplodeOnes);

    const std::vector<int>& Rolls = std::get<0>(Result);
    const int Total = std::get<1>(Result);

    const int ExpectedTotal =
        GetExpectedKeptSum(
            Rolls, NormalizedParams.InKeepNum, NormalizedParams.InBonus);

    EXPECT_EQ(ExpectedTotal, Total);
}

TEST(RollAndKeepRulesLibTests, GetRollAndKeepResults_ReturnsEmptyRollsAndBonus_WhenRollNumIsZero)
{
    const std::tuple<std::vector<int>, int> Result =
        RollAndKeepRulesLib::GetRollAndKeepResults(
            0, 0, 5, false, false);

    EXPECT_TRUE(std::get<0>(Result).empty());
    EXPECT_EQ(5, std::get<1>(Result));
}

TEST(RollAndKeepRulesLibTests, GetRollAndKeepResults_ReturnsEmptyRollsAndNormalizedBonus_WhenKeepIsAboveZeroButRollIsZero)
{
    const std::tuple<std::vector<int>, int> Result =
        RollAndKeepRulesLib::GetRollAndKeepResults(
            0, 5, 3, false, false);

    EXPECT_TRUE(std::get<0>(Result).empty());
    EXPECT_EQ(13, std::get<1>(Result));
}

TEST(RollAndKeepRulesLibTests, GetRollAndKeepResults_ClampsNegativeRollAndKeepToZero)
{
    const std::tuple<std::vector<int>, int> Result =
        RollAndKeepRulesLib::GetRollAndKeepResults(
            -3, -2, 7, false, false);

    EXPECT_TRUE(std::get<0>(Result).empty());
    EXPECT_EQ(7, std::get<1>(Result));
}

TEST(RollAndKeepRulesLibTests, GetRollAndKeepResults_ReturnsNormalizedRollCount_ForNormalPool)
{
    constexpr int RollNum = 5;

    const std::tuple<std::vector<int>, int> Result =
        RollAndKeepRulesLib::GetRollAndKeepResults(
            RollNum, 3, 0, false, false);

    EXPECT_EQ(RollNum, static_cast<int>(std::get<0>(Result).size()));
}

TEST(RollAndKeepRulesLibTests, GetRollAndKeepResults_ReturnsSortedRollsAscending)
{
    const std::tuple<std::vector<int>, int> Result =
        RollAndKeepRulesLib::GetRollAndKeepResults(
            8, 4, 0, false, false);

    ExpectRollsAreSortedAscending(std::get<0>(Result));
}

TEST(RollAndKeepRulesLibTests, GetRollAndKeepResults_ReturnsValidD10Values_WhenExplosionsAreDisabled)
{
    const std::tuple<std::vector<int>, int> Result =
        RollAndKeepRulesLib::GetRollAndKeepResults(
            10, 5, 0, false, false);

    ExpectAllRollsAreValidWithoutExplosions(std::get<0>(Result));
}

TEST(RollAndKeepRulesLibTests, GetRollAndKeepResults_CalculatesTotalFromHighestKeptRollsWithoutBonus)
{
    ExpectRollAndKeepResultMatchesReturnedRolls(
        6, 3, 0, false, false);
}

TEST(RollAndKeepRulesLibTests, GetRollAndKeepResults_CalculatesTotalFromHighestKeptRollsWithPositiveBonus)
{
    ExpectRollAndKeepResultMatchesReturnedRolls(
        6, 3, 5, false, false);
}

TEST(RollAndKeepRulesLibTests, GetRollAndKeepResults_CalculatesTotalFromHighestKeptRollsWithNegativeBonus)
{
    ExpectRollAndKeepResultMatchesReturnedRolls(
        6, 3, -5, false, false);
}

TEST(RollAndKeepRulesLibTests, GetRollAndKeepResults_ConvertsKeepAboveRollIntoBonus)
{
    const std::tuple<std::vector<int>, int> Result =
        RollAndKeepRulesLib::GetRollAndKeepResults(
            3, 5, 0, false, false);

    const std::vector<int>& Rolls = std::get<0>(Result);
    const int Total = std::get<1>(Result);

    EXPECT_EQ(3, static_cast<int>(Rolls.size()));
    ExpectRollsAreSortedAscending(Rolls);

    const int ExpectedTotal =
        GetExpectedKeptSum(
            Rolls, 3, 4);

    EXPECT_EQ(ExpectedTotal, Total);
}

TEST(RollAndKeepRulesLibTests, GetRollAndKeepResults_AppliesTenDiceRule_WhenRollNumIsAboveTen)
{
    const std::tuple<std::vector<int>, int> Result =
        RollAndKeepRulesLib::GetRollAndKeepResults(
            12, 5, 0, false, false);

    const std::vector<int>& Rolls = std::get<0>(Result);

    EXPECT_EQ(10, static_cast<int>(Rolls.size()));
    ExpectRollsAreSortedAscending(Rolls);
    ExpectAllRollsAreValidWithoutExplosions(Rolls);

    const int ExpectedTotal =
        GetExpectedKeptSum(
            Rolls, 6, 0);

    EXPECT_EQ(ExpectedTotal, std::get<1>(Result));
}

TEST(RollAndKeepRulesLibTests, GetRollAndKeepResults_AppliesTenDiceRule_WhenRollAndKeepAreAboveTen)
{
    const std::tuple<std::vector<int>, int> Result =
        RollAndKeepRulesLib::GetRollAndKeepResults(
            12, 12, 0, false, false);

    const std::vector<int>& Rolls = std::get<0>(Result);

    EXPECT_EQ(10, static_cast<int>(Rolls.size()));
    ExpectRollsAreSortedAscending(Rolls);
    ExpectAllRollsAreValidWithoutExplosions(Rolls);

    const int ExpectedTotal =
        GetExpectedKeptSum(
            Rolls, 10, 8);

    EXPECT_EQ(ExpectedTotal, std::get<1>(Result));
}

TEST(RollAndKeepRulesLibTests, GetRollAndKeepResults_ReturnsValidValues_WhenExplodeOnesIsEnabled)
{
    const std::tuple<std::vector<int>, int> Result =
        RollAndKeepRulesLib::GetRollAndKeepResults(
            10, 5, 0, false, true);

    const std::vector<int>& Rolls = std::get<0>(Result);

    EXPECT_EQ(10, static_cast<int>(Rolls.size()));
    ExpectRollsAreSortedAscending(Rolls);
    ExpectAllRollsAreValidWithoutExplosions(Rolls);

    ExpectRollAndKeepResultMatchesReturnedRolls(
        10, 5, 0, false, true);
}

TEST(RollAndKeepRulesLibTests, GetRollAndKeepResults_ReturnsValidValues_WhenExplodeTensIsEnabled)
{
    const std::tuple<std::vector<int>, int> Result =
        RollAndKeepRulesLib::GetRollAndKeepResults(
            10, 5, 0, true, false);

    const std::vector<int>& Rolls = std::get<0>(Result);

    EXPECT_EQ(10, static_cast<int>(Rolls.size()));
    ExpectRollsAreSortedAscending(Rolls);
    ExpectAllRollsAreValidWithExplosions(Rolls);

    ExpectRollAndKeepResultMatchesReturnedRolls(
        10, 5, 0, true, false);
}

TEST(RollAndKeepRulesLibTests, GetRollAndKeepResults_ReturnsValidValues_WhenBothExplosionRulesAreEnabled)
{
    const std::tuple<std::vector<int>, int> Result =
        RollAndKeepRulesLib::GetRollAndKeepResults(
            10, 5, 3, true, true);

    const std::vector<int>& Rolls = std::get<0>(Result);

    EXPECT_EQ(10, static_cast<int>(Rolls.size()));
    ExpectRollsAreSortedAscending(Rolls);
    ExpectAllRollsAreValidWithExplosions(Rolls);

    ExpectRollAndKeepResultMatchesReturnedRolls(
        10, 5, 3, true, true);
}

TEST(RollAndKeepRulesLibTests, GetRollAndKeepResults_DoesNotThrow_ForRepresentativeInputs)
{
    EXPECT_NO_THROW(RollAndKeepRulesLib::GetRollAndKeepResults(
        1, 1, 0, false, false));

    EXPECT_NO_THROW(RollAndKeepRulesLib::GetRollAndKeepResults(
        10, 5, 0, true, false));

    EXPECT_NO_THROW(RollAndKeepRulesLib::GetRollAndKeepResults(
        10, 5, 3, true, true));

    EXPECT_NO_THROW(RollAndKeepRulesLib::GetRollAndKeepResults(
        -5, -3, 2, false, true));
}

}