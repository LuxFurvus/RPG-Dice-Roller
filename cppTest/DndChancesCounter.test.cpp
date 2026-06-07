#include "../app/src/main/cpp/DndCpp/DndChancesCounter.h"

#include <string>
#include <vector>

#include "gtest/gtest.h"

namespace
{

RollPair MakeRollPair(
    const int DiceNum,
    const int SideNum)
{
    return RollPair
    {
        .DiceNum = DiceNum,
        .SideNum = SideNum
    };
}

void ExpectMinAvgMaxString(
    const std::string& Expected,
    const std::vector<RollPair>& RollData,
    const int RollModifier)
{
    const std::string Result =
        DndChancesCounter::GetMinAvgMaxValuesString(
            RollData,     RollModifier);

    EXPECT_EQ(Expected, Result);
}

TEST(DndChancesCounterTests, GetMinAvgMaxValuesString_ReturnsModifierOnly_WhenRollDataIsEmpty)
{
    ExpectMinAvgMaxString(
        "0", {}, 0);
}

TEST(DndChancesCounterTests, GetMinAvgMaxValuesString_ReturnsPositiveModifierOnly_WhenRollDataIsEmpty)
{
    ExpectMinAvgMaxString(
        "5", {}, 5);
}

TEST(DndChancesCounterTests, GetMinAvgMaxValuesString_ReturnsNegativeModifierOnly_WhenRollDataIsEmpty)
{
    ExpectMinAvgMaxString(
        "-3", {}, -3);
}

TEST(DndChancesCounterTests, GetMinAvgMaxValuesString_ReturnsMinAvgMax_ForOneD6WithoutModifier)
{
    ExpectMinAvgMaxString(
        "Min: 1\n\n" "Avg: 3\n\n" "Max: 6", {
            MakeRollPair(1, 6)
        }, 0);
}

TEST(DndChancesCounterTests, GetMinAvgMaxValuesString_ReturnsMinAvgMax_ForOneD20WithoutModifier)
{
    ExpectMinAvgMaxString(
        "Min: 1\n\n" "Avg: 10\n\n" "Max: 20", {
            MakeRollPair(1, 20)
        }, 0);
}

TEST(DndChancesCounterTests, GetMinAvgMaxValuesString_ReturnsMinAvgMax_ForOneD20WithPositiveModifier)
{
    ExpectMinAvgMaxString(
        "Min: 6\n\n" "Avg: 15\n\n" "Max: 25", {
            MakeRollPair(1, 20)
        }, 5);
}

TEST(DndChancesCounterTests, GetMinAvgMaxValuesString_ReturnsMinAvgMax_ForOneD20WithNegativeModifier)
{
    ExpectMinAvgMaxString(
        "Min: -1\n\n" "Avg: 8\n\n" "Max: 18", {
            MakeRollPair(1, 20)
        }, -2);
}

TEST(DndChancesCounterTests, GetMinAvgMaxValuesString_ReturnsMinAvgMax_ForTwoD6WithoutModifier)
{
    ExpectMinAvgMaxString(
        "Min: 2\n\n" "Avg: 7\n\n" "Max: 12", {
            MakeRollPair(2, 6)
        }, 0);
}

TEST(DndChancesCounterTests, GetMinAvgMaxValuesString_ReturnsMinAvgMax_ForTwoD6WithPositiveModifier)
{
    ExpectMinAvgMaxString(
        "Min: 5\n\n" "Avg: 10\n\n" "Max: 15", {
            MakeRollPair(2, 6)
        }, 3);
}

TEST(DndChancesCounterTests, GetMinAvgMaxValuesString_ReturnsMinAvgMax_ForMultipleDiceGroupsWithoutModifier)
{
    ExpectMinAvgMaxString(
        "Min: 3\n\n" "Avg: 11\n\n" "Max: 20", {
            MakeRollPair(1, 8),     MakeRollPair(2, 6)
        }, 0);
}

TEST(DndChancesCounterTests, GetMinAvgMaxValuesString_ReturnsMinAvgMax_ForMultipleDiceGroupsWithPositiveModifier)
{
    ExpectMinAvgMaxString(
        "Min: 7\n\n" "Avg: 15\n\n" "Max: 24", {
            MakeRollPair(1, 8),     MakeRollPair(2, 6)
        }, 4);
}

TEST(DndChancesCounterTests, GetMinAvgMaxValuesString_ReturnsMinAvgMax_ForMultipleDiceGroupsWithNegativeModifier)
{
    ExpectMinAvgMaxString(
        "Min: 0\n\n" "Avg: 8\n\n" "Max: 17", {
            MakeRollPair(1, 8),     MakeRollPair(2, 6)
        }, -3);
}

    TEST(DndChancesCounterTests, GetMinAvgMaxValuesString_UsesIntegerMidpointOfMinAndMax)
{
    ExpectMinAvgMaxString(
        "Min: 1\n\n"
        "Avg: 4\n\n"
        "Max: 7",
        {
            MakeRollPair(1, 7)
        },
        0);
}

    TEST(DndChancesCounterTests, GetMinAvgMaxValuesString_HandlesSeveralCommonDndDamageDice)
{
    ExpectMinAvgMaxString(
        "Min: 6\n\n"
        "Avg: 22\n\n"
        "Max: 38",
        {
            MakeRollPair(1, 4),
            MakeRollPair(2, 8),
            MakeRollPair(3, 6)
        },
        0);
}

TEST(DndChancesCounterTests, GetMinAvgMaxValuesString_HandlesZeroDiceGroup)
{
    ExpectMinAvgMaxString(
        "Min: 2\n\n" "Avg: 2\n\n" "Max: 2", {
            MakeRollPair(0, 6)
        }, 2);
}

TEST(DndChancesCounterTests, GetMinAvgMaxValuesString_DoesNotThrow_ForRepresentativeInputs)
{
    EXPECT_NO_THROW(DndChancesCounter::GetMinAvgMaxValuesString(
        {}, 0));

    EXPECT_NO_THROW(DndChancesCounter::GetMinAvgMaxValuesString(
        {
            MakeRollPair(1, 20)
        }, 5));

    EXPECT_NO_THROW(DndChancesCounter::GetMinAvgMaxValuesString(
        {
            MakeRollPair(2, 6), MakeRollPair(1, 8)
        }, -2));
}

}