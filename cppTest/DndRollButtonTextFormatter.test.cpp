#include "../app/src/main/cpp/DndCpp/DndRollButtonTextFormatter.h"

#include <string>
#include <vector>

#include "gtest/gtest.h"

namespace
{

RollPair MakeRollPair(const int DiceNum, const int SideNum)
{
    return RollPair {.DiceNum = DiceNum, .SideNum = SideNum};
}

void ExpectRollButtonText(
    const std::string& Expected,
    const std::vector<RollPair>& RollData,
    const int RollModifier)
{
    const std::string Result = DndRollButtonTextFormatter::GetRollButtonText(RollData, RollModifier);

    EXPECT_EQ(Expected, Result);
}

TEST(DndRollButtonTextFormatterTests,
    GetRollButtonText_ReturnsEmptyString_WhenRollDataIsEmptyAndModifierIsZero)
{
    ExpectRollButtonText(
        "", {}, 0);
}

TEST(DndRollButtonTextFormatterTests,
    GetRollButtonText_ReturnsPositiveModifierOnly_WhenRollDataIsEmpty)
{
    ExpectRollButtonText(
        "5", {}, 5);
}

TEST(DndRollButtonTextFormatterTests,
    GetRollButtonText_ReturnsNegativeModifierOnly_WhenRollDataIsEmpty)
{
    ExpectRollButtonText(
        "-3", {}, -3);
}

TEST(DndRollButtonTextFormatterTests,
    GetRollButtonText_ReturnsSingleDiceExpressionWithoutModifier)
{
    ExpectRollButtonText(
        "1d20", {MakeRollPair(1, 20)}, 0);
}

TEST(DndRollButtonTextFormatterTests,
    GetRollButtonText_ReturnsSingleDiceExpressionWithPositiveModifier)
{
    ExpectRollButtonText(
        "1d20 + 5", {MakeRollPair(1, 20)}, 5);
}

TEST(DndRollButtonTextFormatterTests,
    GetRollButtonText_ReturnsSingleDiceExpressionWithNegativeModifier)
{
    ExpectRollButtonText(
        "1d20-2", {MakeRollPair(1, 20)}, -2);
}

TEST(DndRollButtonTextFormatterTests,
    GetRollButtonText_ReturnsMultipleDiceExpressionsWithoutModifier)
{
    ExpectRollButtonText(
        "1d20 + 2d6", {MakeRollPair(1, 20), MakeRollPair(2, 6)}, 0);
}

TEST(DndRollButtonTextFormatterTests,
    GetRollButtonText_ReturnsMultipleDiceExpressionsWithPositiveModifier)
{
    ExpectRollButtonText(
        "1d20 + 2d6 + 3", {MakeRollPair(1, 20), MakeRollPair(2, 6)}, 3);
}

TEST(DndRollButtonTextFormatterTests,
    GetRollButtonText_ReturnsMultipleDiceExpressionsWithNegativeModifier)
{
    ExpectRollButtonText(
        "1d20 + 2d6-4", {MakeRollPair(1, 20), MakeRollPair(2, 6)}, -4);
}

TEST(DndRollButtonTextFormatterTests,
    GetRollButtonText_SkipsZeroDiceNum)
{
    ExpectRollButtonText(
        "1d20", {MakeRollPair(0, 6), MakeRollPair(1, 20)}, 0);
}

TEST(DndRollButtonTextFormatterTests,
    GetRollButtonText_SkipsNegativeDiceNum)
{
    ExpectRollButtonText(
        "2d8", {MakeRollPair(-1, 6), MakeRollPair(2, 8)}, 0);
}

TEST(DndRollButtonTextFormatterTests,
    GetRollButtonText_SkipsZeroSideNum)
{
    ExpectRollButtonText(
        "3d4", {MakeRollPair(1, 0), MakeRollPair(3, 4)}, 0);
}

TEST(DndRollButtonTextFormatterTests,
    GetRollButtonText_SkipsNegativeSideNum)
{
    ExpectRollButtonText(
        "1d12", {MakeRollPair(2, -6), MakeRollPair(1, 12)}, 0);
}

TEST(DndRollButtonTextFormatterTests,
    GetRollButtonText_SkipsSeveralInvalidPairsAndKeepsValidSeparator)
{
    ExpectRollButtonText(
        "1d20 + 2d6", {MakeRollPair(0, 6), MakeRollPair(1, 20), MakeRollPair(-2, 8), MakeRollPair(2, 6), MakeRollPair(3, 0)}, 0);
}

TEST(DndRollButtonTextFormatterTests,
    GetRollButtonText_ReturnsEmptyString_WhenAllRollPairsAreInvalidAndModifierIsZero)
{
    ExpectRollButtonText(
        "", {MakeRollPair(0, 6), MakeRollPair(-1, 8), MakeRollPair(2, 0), MakeRollPair(3, -4)}, 0);
}

TEST(DndRollButtonTextFormatterTests,
    GetRollButtonText_ReturnsPositiveModifierOnly_WhenAllRollPairsAreInvalid)
{
    ExpectRollButtonText(
        "7", {MakeRollPair(0, 6), MakeRollPair(-1, 8), MakeRollPair(2, 0)}, 7);
}

TEST(DndRollButtonTextFormatterTests,
    GetRollButtonText_ReturnsNegativeModifierOnly_WhenAllRollPairsAreInvalid)
{
    ExpectRollButtonText(
        "-7", {MakeRollPair(0, 6), MakeRollPair(-1, 8), MakeRollPair(2, 0)}, -7);
}

TEST(DndRollButtonTextFormatterTests,
    GetRollButtonText_KeepsOriginalValidRollOrder)
{
    ExpectRollButtonText(
        "3d4 + 1d20 + 2d6", {MakeRollPair(3, 4), MakeRollPair(1, 20), MakeRollPair(2, 6)}, 0);
}

TEST(DndRollButtonTextFormatterTests,
    GetRollButtonText_DoesNotThrow_ForRepresentativeInputs)
{
    EXPECT_NO_THROW(DndRollButtonTextFormatter::GetRollButtonText(
        {}, 0));

    EXPECT_NO_THROW(DndRollButtonTextFormatter::GetRollButtonText(
        {MakeRollPair(1, 20)}, 5));

    EXPECT_NO_THROW(DndRollButtonTextFormatter::GetRollButtonText(
        {MakeRollPair(1, 20), MakeRollPair(2, 6)}, -3));

    EXPECT_NO_THROW(DndRollButtonTextFormatter::GetRollButtonText(
        {MakeRollPair(0, 6), MakeRollPair(-1, 8), MakeRollPair(2, 0)}, 4));
}

}