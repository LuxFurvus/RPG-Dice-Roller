
#pragma once

#include <array>
#include <vector>
#include <string>

#include "DndRollPair.h"

class DndChancesCounter
{

private:

    static std::array<int, 3> GetMinAvgMaxValues(
        const std::vector<RollPair>& InRollData,
        const int InRollModifier);

public:

    static std::string GetMinAvgMaxValuesString(
        const std::vector<RollPair>& InRollData,
        const int InRollModifier);
};
