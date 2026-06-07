
#pragma once

#include <sstream>
#include <string>
#include <vector>

#include "DndRollPair.h"

class DndRollButtonTextFormatter
{
private:

    static void AppendModifier(
        std::ostringstream& ResultStream,
        const bool HasRollData,
        const int InRollModifier);

public:

    static std::string GetRollButtonText(
        const std::vector<RollPair>& InRollData,
        const int InRollModifier);
};
