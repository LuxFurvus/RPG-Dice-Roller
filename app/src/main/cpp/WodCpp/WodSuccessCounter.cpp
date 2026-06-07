
#include "WodSuccessCounter.h"

#include <algorithm>

#include "../CommonCpp/SortedRollsMaker.h"


int WodSuccessCounter::RollSingleDie()
{
    const std::vector<int> SortedRoll
        = SortedRollsMaker::GetSortedRoll(
            1, GlobalSideNumber);

    return SortedRoll.empty()? 0 : SortedRoll[0];
}

std::pair<std::vector<int>, int> WodSuccessCounter::GetTenRerollResults(
    const int InDifficulty)
{
    int AdditionalSuccessCount = 0;
    int ConsecutiveTenCount = 1; // initial 10 is already present

    std::vector<int> NewRerollValues;

    while (ConsecutiveTenCount < MaxTenReroll)
    {
        const int RerollValue = RollSingleDie();

        if (RerollValue >= InDifficulty)
        {
            ++AdditionalSuccessCount;
        }

        NewRerollValues.emplace_back(RerollValue);

        if (RerollValue != 10)
        {
            break;
        }

        ++ConsecutiveTenCount;
    }

    return std::make_pair(NewRerollValues, AdditionalSuccessCount);
}

std::pair<std::vector<int>, int> WodSuccessCounter::GetSuccessesNum(
    const int InDiceCount,
    const int InDifficulty,
    const bool InWithCancel,
    const bool InWithTenReroll,
    const int InModifier)
{
    const std::vector<int> InitialSortedRoll
        = SortedRollsMaker::GetSortedRoll(
            InDiceCount, GlobalSideNumber);

    const auto DifficultyChecker =
        [InDifficulty](const int RollValue) -> bool
        {
            return RollValue >= InDifficulty;
        };

    int SuccessCount =
        static_cast<int>(std::count_if(
            InitialSortedRoll.begin(), InitialSortedRoll.end(), DifficultyChecker));

    const int InitialOneCount =
        static_cast<int>(std::count(
            InitialSortedRoll.begin(), InitialSortedRoll.end(), 1));

    std::vector<int> FinalSortedRoll = InitialSortedRoll;

    if (InWithTenReroll)
    {
        const int InitialTenCount =
            static_cast<int>(std::count(
                InitialSortedRoll.begin(), InitialSortedRoll.end(), 10));

        for (int TenIndex = 0; TenIndex < InitialTenCount; ++TenIndex)
        {
            const auto& [NewRerollValues, AdditionalSuccesses] =
                GetTenRerollResults(InDifficulty);

            SuccessCount += AdditionalSuccesses;

            FinalSortedRoll.insert(
                FinalSortedRoll.end(),
                NewRerollValues.begin(),
                NewRerollValues.end());
        }
    }

    int FinalSuccessCount = SuccessCount;

    if (InWithCancel)
    {
        FinalSuccessCount -= InitialOneCount;
    }

    FinalSuccessCount += InModifier;

    const bool HasAutoSuccesses = InModifier > 0;

    if (InWithCancel
        && !HasAutoSuccesses
        && InitialOneCount > 0
        && FinalSuccessCount <= 0)
    {
        return std::make_pair(FinalSortedRoll, -InitialOneCount);
    }

    FinalSuccessCount = std::max(FinalSuccessCount, 0);

    return std::make_pair(FinalSortedRoll, FinalSuccessCount);
}
