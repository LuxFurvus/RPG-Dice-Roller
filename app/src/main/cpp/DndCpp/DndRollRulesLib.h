
#pragma once

#include <string>
#include <tuple>
#include <vector>

#include "DndRollPair.h"

struct DndRollRulesLib
{
private:

    static std::string FormatRollLine(
        const RollPair& InRollData,
        const std::vector<int>& InRollSequence,
        const int InRollSum);

public:

    static std::tuple<std::string, int> GetDndRollResults(
        const std::vector<RollPair>& InRollData,
        const int InRollModifier);
};
