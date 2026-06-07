
#include "../app/src/main/cpp/DndCpp/DndRollRulesLib.h"

#include <algorithm>
#include <numeric>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

#include "gtest/gtest.h"

namespace
{

RollPair MakeRollPair(const int DiceNum, const int SideNum)
{
    return RollPair{.DiceNum = DiceNum, .SideNum = SideNum};
}

std::vector<std::string> SplitLines(const std::string& Text)
{
    std::vector<std::string> Lines;
    std::istringstream Stream(Text);
    std::string Line;

    while (std::getline(Stream, Line))
    {
        Lines.emplace_back(Line);
    }

    return Lines;
}

std::string GetExpectedPrefix(const RollPair& RollData)
{
    std::ostringstream Stream;

    Stream << "* " << RollData.DiceNum << 'd' << RollData.SideNum << ": ";

    return Stream.str();
}

std::string TrimString(std::string Text)
{
    while (!Text.empty() && std::isspace(static_cast<unsigned char>(Text.front())))
    {
        Text.erase(Text.begin());
    }

    while (!Text.empty() && std::isspace(static_cast<unsigned char>(Text.back())))
    {
        Text.pop_back();
    }

    return Text;
}

std::vector<int> ParseRollValuesFromLine(const std::string& Line)
{
    const std::size_t ColonPosition = Line.find(": ");

    if (ColonPosition == std::string::npos)
    {
        return {};
    }

    const std::size_t ValuesStart = ColonPosition + 2;
    const std::size_t EqualsPosition = Line.find(" = ", ValuesStart);

    const std::string ValuesText =
        EqualsPosition == std::string::npos
            ? Line.substr(ValuesStart)
            : Line.substr(ValuesStart, EqualsPosition - ValuesStart);

    std::vector<int> Values;
    std::istringstream Stream(ValuesText);
    std::string Token;

    while (std::getline(Stream, Token, ','))
    {
        Token = TrimString(Token);

        if (!Token.empty())
        {
            Values.emplace_back(std::stoi(Token));
        }
    }

    return Values;
}

int ParseDisplayedSumFromLine(const std::string& Line)
{
    const std::size_t EqualsPosition = Line.find(" = ");

    if (EqualsPosition == std::string::npos)
    {
        return 0;
    }

    return std::stoi(Line.substr(EqualsPosition + 3));
}

int GetRollValuesSumFromResultText(const std::string& ResultText)
{
    int Sum = 0;

    for (const std::string& Line : SplitLines(ResultText))
    {
        const std::vector<int> Values = ParseRollValuesFromLine(Line);
        Sum += std::accumulate(Values.begin(), Values.end(), 0);
    }

    return Sum;
}

void ExpectLineMatchesRollData(const std::string& Line, const RollPair& RollData)
{
    const std::string ExpectedPrefix = GetExpectedPrefix(RollData);

    EXPECT_EQ(0, Line.rfind(ExpectedPrefix, 0));

    const std::vector<int> RollValues = ParseRollValuesFromLine(Line);

    ASSERT_EQ(RollData.DiceNum, static_cast<int>(RollValues.size()));

    EXPECT_TRUE(std::ranges::all_of(RollValues, [RollData](const int RollValue) -> bool
    {
        return RollValue >= 1 && RollValue <= RollData.SideNum;
    }));

    EXPECT_TRUE(std::ranges::is_sorted(RollValues));

    const int RollSum = std::accumulate(RollValues.begin(), RollValues.end(), 0);

    if (RollValues.size() > 1)
    {
        EXPECT_NE(std::string::npos, Line.find(" = "));
        EXPECT_EQ(RollSum, ParseDisplayedSumFromLine(Line));
    }
    else
    {
        EXPECT_EQ(std::string::npos, Line.find(" = "));
    }
}

void ExpectDndRollResultIsValid(const std::vector<RollPair>& RollData, const int RollModifier)
{
    const std::tuple<std::string, int> Result
        = DndRollRulesLib::GetDndRollResults(RollData, RollModifier);

    const std::string& ResultText = std::get<0>(Result);
    const int Total = std::get<1>(Result);

    const std::vector<std::string> Lines = SplitLines(ResultText);

    ASSERT_EQ(RollData.size(), Lines.size());

    for (std::size_t Index = 0; Index < RollData.size(); ++Index)
    {
        ExpectLineMatchesRollData(Lines[Index], RollData[Index]);
    }

    EXPECT_EQ(RollModifier + GetRollValuesSumFromResultText(ResultText), Total);
}

void ExpectDndRollResultMatchesValidRollData(
    const std::vector<RollPair>& InputRollData,
    const std::vector<RollPair>& ExpectedValidRollData,
    const int RollModifier)
{
    const std::tuple<std::string, int> Result
        = DndRollRulesLib::GetDndRollResults(InputRollData, RollModifier);

    const std::string& ResultText = std::get<0>(Result);
    const int Total = std::get<1>(Result);

    const std::vector<std::string> Lines = SplitLines(ResultText);

    ASSERT_EQ(ExpectedValidRollData.size(), Lines.size());

    for (std::size_t Index = 0; Index < ExpectedValidRollData.size(); ++Index)
    {
        ExpectLineMatchesRollData(Lines[Index], ExpectedValidRollData[Index]);
    }

    EXPECT_EQ(RollModifier + GetRollValuesSumFromResultText(ResultText), Total);
}

TEST(DndRollRulesLibTests,
    GetDndRollResults_ReturnsEmptyResult_WhenRollDataIsEmpty)
{
    const std::tuple<std::string, int> Result
        = DndRollRulesLib::GetDndRollResults({}, 0);

    EXPECT_EQ("", std::get<0>(Result));
    EXPECT_EQ(0, std::get<1>(Result));
}

TEST(DndRollRulesLibTests,
    GetDndRollResults_ReturnsEmptyResult_WhenRollDataIsEmptyEvenWithModifier)
{
    const std::tuple<std::string, int> Result
        = DndRollRulesLib::GetDndRollResults({}, 5);

    EXPECT_EQ("", std::get<0>(Result));
    EXPECT_EQ(0, std::get<1>(Result));
}

TEST(DndRollRulesLibTests,
    GetDndRollResults_ReturnsValidSingleD20Result)
{
    ExpectDndRollResultIsValid({MakeRollPair(1, 20)}, 0);
}

TEST(DndRollRulesLibTests,
    GetDndRollResults_ReturnsValidSingleD2Result)
{
    ExpectDndRollResultIsValid({MakeRollPair(1, 2)}, 0);
}

TEST(DndRollRulesLibTests,
    GetDndRollResults_ReturnsValidTwoD6Result)
{
    ExpectDndRollResultIsValid({MakeRollPair(2, 6)}, 0);
}

TEST(DndRollRulesLibTests,
    GetDndRollResults_ReturnsValidThreeD4Result)
{
    ExpectDndRollResultIsValid({MakeRollPair(3, 4)}, 0);
}

TEST(DndRollRulesLibTests,
    GetDndRollResults_ReturnsValidMultipleDiceGroupResult)
{
    ExpectDndRollResultIsValid({MakeRollPair(1, 20),
        MakeRollPair(2, 6), MakeRollPair(3, 4)}, 0);
}

TEST(DndRollRulesLibTests,
    GetDndRollResults_AppliesPositiveModifierToTotal)
{
    ExpectDndRollResultIsValid({MakeRollPair(1, 20), MakeRollPair(2, 6)}, 5);
}

TEST(DndRollRulesLibTests,
    GetDndRollResults_AppliesNegativeModifierToTotal)
{
    ExpectDndRollResultIsValid({MakeRollPair(1, 20), MakeRollPair(2, 6)}, -3);
}

TEST(DndRollRulesLibTests,
    GetDndRollResults_SkipsZeroDiceNum)
{
    ExpectDndRollResultMatchesValidRollData(
        {MakeRollPair(0, 6), MakeRollPair(1, 20)}, {MakeRollPair(1, 20)}, 0);
}

TEST(DndRollRulesLibTests,
    GetDndRollResults_SkipsNegativeDiceNum)
{
    ExpectDndRollResultMatchesValidRollData(
        {MakeRollPair(-2, 6), MakeRollPair(2, 8)}, {MakeRollPair(2, 8)}, 0);
}

TEST(DndRollRulesLibTests,
    GetDndRollResults_SkipsZeroSideNum)
{
    ExpectDndRollResultMatchesValidRollData(
        {MakeRollPair(2, 0), MakeRollPair(1, 12)}, {MakeRollPair(1, 12)}, 0);
}

TEST(DndRollRulesLibTests,
    GetDndRollResults_SkipsNegativeSideNum)
{
    ExpectDndRollResultMatchesValidRollData(
        {MakeRollPair(2, -6), MakeRollPair(1, 10)}, {MakeRollPair(1, 10)}, 0);
}

TEST(DndRollRulesLibTests,
    GetDndRollResults_SkipsSeveralInvalidPairsAndKeepsValidOrder)
{
    ExpectDndRollResultMatchesValidRollData(
        {MakeRollPair(0, 6), MakeRollPair(1, 20), MakeRollPair(-2, 8),
        MakeRollPair(2, 6), MakeRollPair(3, 0)},
        {MakeRollPair(1, 20), MakeRollPair(2, 6)}, 0);
}

TEST(DndRollRulesLibTests,
    GetDndRollResults_DoesNotAddBlankLines_WhenInvalidRollPairsAreSkipped)
{
    const std::tuple<std::string, int> Result
        = DndRollRulesLib::GetDndRollResults(
            {MakeRollPair(0, 6), MakeRollPair(1, 20),
            MakeRollPair(-2, 8), MakeRollPair(2, 6)}, 0);

    const std::string& ResultText = std::get<0>(Result);

    EXPECT_EQ(std::string::npos, ResultText.find("\n\n"));

    const std::vector<std::string> Lines = SplitLines(ResultText);

    ASSERT_EQ(2, static_cast<int>(Lines.size()));

    ExpectLineMatchesRollData(Lines[0], MakeRollPair(1, 20));
    ExpectLineMatchesRollData(Lines[1], MakeRollPair(2, 6));
}

TEST(DndRollRulesLibTests,
    GetDndRollResults_ReturnsEmptyResult_WhenAllRollPairsAreInvalid)
{
    const std::tuple<std::string, int> Result
        = DndRollRulesLib::GetDndRollResults(
            {MakeRollPair(0, 6), MakeRollPair(-1, 8),
            MakeRollPair(2, 0), MakeRollPair(3, -4)}, 5);

    EXPECT_EQ("", std::get<0>(Result));
    EXPECT_EQ(0, std::get<1>(Result));
}

TEST(DndRollRulesLibTests,
    GetDndRollResults_DoesNotThrow_ForRepresentativeInputs)
{
    EXPECT_NO_THROW(DndRollRulesLib::GetDndRollResults(
        {}, 0));
    EXPECT_NO_THROW(DndRollRulesLib::GetDndRollResults(
        {MakeRollPair(1, 20)}, 5));
    EXPECT_NO_THROW(DndRollRulesLib::GetDndRollResults(
        {MakeRollPair(2, 6), MakeRollPair(1, 8)}, -2));
    EXPECT_NO_THROW(DndRollRulesLib::GetDndRollResults(
        {MakeRollPair(0, 6), MakeRollPair(-1, 8),
        MakeRollPair(2, 0)}, 4));
}

}