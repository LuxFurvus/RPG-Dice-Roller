
#include "DndRollButtonTextFormatter.h"

void DndRollButtonTextFormatter::AppendModifier(
    std::ostringstream& ResultStream,
    const bool HasRollData,
    const int InRollModifier)
{
    if (InRollModifier == 0)
    {
        return;
    }

    if (HasRollData && InRollModifier > 0)
    {
        ResultStream << " + ";
    }

    ResultStream << InRollModifier;
}

std::string DndRollButtonTextFormatter::GetRollButtonText(
    const std::vector<RollPair>& InRollData,
    const int InRollModifier)
{
    if (InRollData.empty() && InRollModifier == 0)
    {
        return "";
    }

    std::ostringstream ResultStream;
    bool HasRollData = false;

    for (const auto& [DiceNum, SideNum] : InRollData)
    {
        if (DiceNum <= 0 || SideNum <= 0)
        {
            continue;
        }

        if (HasRollData)
        {
            ResultStream << " + ";
        }

        ResultStream
            << DiceNum
            << 'd'
            << SideNum;

        HasRollData = true;
    }

    AppendModifier(
        ResultStream,
        HasRollData,
        InRollModifier
    );

    return ResultStream.str();
}
