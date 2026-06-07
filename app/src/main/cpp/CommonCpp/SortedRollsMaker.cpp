
#include "SortedRollsMaker.h"

#include <random>
#include <vector>
#include <algorithm>

std::vector<int> SortedRollsMaker::GetSortedRoll(
    const int InDiceNumber,
    const int InSideNumber)
{
    if (InDiceNumber < 0 || InSideNumber < 2)
    {
        return {};
    }

    static thread_local std::mt19937 Generator(std::random_device{}());
    std::uniform_int_distribution<int> Distribution(1, InSideNumber);

    std::vector<int> Results;
    Results.reserve(InDiceNumber);

    for (int Index = 0; Index < InDiceNumber; ++Index)
    {
        Results.emplace_back(Distribution(Generator));
    }

    std::sort(Results.begin(), Results.end());

    return Results;
}
