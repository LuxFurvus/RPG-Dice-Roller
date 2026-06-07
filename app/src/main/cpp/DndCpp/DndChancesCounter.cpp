
#include "DndChancesCounter.h"

#include <sstream>

std::array<int, 3> DndChancesCounter::GetMinAvgMaxValues(
    const std::vector<RollPair>& InRollData,
    const int InRollModifier)
{
    if (InRollData.empty())
    {
        return {};
    }

    int MinValue = InRollModifier;
    int MaxValue = InRollModifier;

    for (const auto& [DiceNum, SideNum] : InRollData)
    {
        MinValue += DiceNum;
        MaxValue += DiceNum * SideNum;
    }

    const int AvgValue = (MinValue + MaxValue) / 2;

    return { MinValue, AvgValue, MaxValue };
}

std::string DndChancesCounter::GetMinAvgMaxValuesString(
    const std::vector<RollPair>& InRollData,
    const int InRollModifier)
{
    if (InRollData.empty())
    {
        return std::to_string(InRollModifier);
    }

    const auto [MinValue, AvgValue, MaxValue]
        = GetMinAvgMaxValues(
            InRollData, InRollModifier);

    std::ostringstream ResultStream;
    ResultStream
        << "Min: " << MinValue << "\n\n"
        << "Avg: " << AvgValue << "\n\n"
        << "Max: " << MaxValue;
    return ResultStream.str();
}
