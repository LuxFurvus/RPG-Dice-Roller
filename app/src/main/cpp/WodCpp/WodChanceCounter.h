
#pragma once

#include <string>
#include <utility>
#include <vector>

class WodChanceCounter
{
private:

    static constexpr int MinDiceCount = 1;
    static constexpr int MaxDiceCount = 999;

    static constexpr int MinDifficulty = 2;
    static constexpr int GlobalSideNumber = 10;

    static constexpr int MaxTenReroll = 10;

    static constexpr long double MinDisplayedProbability = 0.001L;

private:

    static bool IsInputValid(
        const int InDiceCount,
        const int InDifficulty);

    static int GetMaxSuccessPerDie(
        const bool InWithTenReroll);

    static std::string ConvertProbabilityEntriesToString(
        const std::vector<std::pair<int, long double>>& InEntries);

public:

    static std::string GetProbabilityList(
        const int InDiceCount,
        const int InDifficulty,
        const bool InWithCancel,
        const bool InWithTenReroll,
        const int Modifier);
};