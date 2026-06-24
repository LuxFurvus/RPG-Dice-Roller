#include "WodChanceCounter.h"

#include <cmath>
#include <iomanip>
#include <sstream>

bool WodChanceCounter::IsInputValid(
    const int InDiceCount,
    const int InDifficulty)
{
    return
        InDiceCount >= MinDiceCount &&
        InDiceCount <= MaxDiceCount &&
        InDifficulty >= MinDifficulty &&
        InDifficulty <= GlobalSideNumber;
}

int WodChanceCounter::GetMaxSuccessPerDie(
    const bool InWithTenReroll)
{
    return InWithTenReroll ? MaxTenReroll : 1;
}

std::string WodChanceCounter::ConvertProbabilityEntriesToString(
    const std::vector<std::pair<int, long double>>& InEntries)
{
    if (InEntries.empty())
    {
        return "";
    }

    int MaxDigits = 1;

    for (const std::pair<int, long double>& Entry : InEntries)
    {
        if (Entry.first < 0)
        {
            continue;
        }

        const int CurrentDigits =
            static_cast<int>(std::to_string(Entry.first).length());

        if (CurrentDigits > MaxDigits)
        {
            MaxDigits = CurrentDigits;
        }
    }

    std::ostringstream ResultStream;
    bool IsFirstLine = true;

    for (const std::pair<int, long double>& Entry : InEntries)
    {
        if (!IsFirstLine)
        {
            ResultStream << '\n';
        }

        if (Entry.first < 0)
        {
            ResultStream << std::setw(MaxDigits) << "B";
        }
        else
        {
            ResultStream << std::setw(MaxDigits) << Entry.first;
        }

        ResultStream
            << ": "
            << std::fixed
            << std::setprecision(2)
            << static_cast<double>(Entry.second * 100.0L)
            << '%';

        IsFirstLine = false;
    }

    return ResultStream.str();
}

std::string WodChanceCounter::GetProbabilityList(
    const int InDiceCount,
    const int InDifficulty,
    const bool InWithCancel,
    const bool InWithTenReroll,
    const int InModifier)
{
    if (!IsInputValid(InDiceCount, InDifficulty))
    {
        return "";
    }

    struct SingleDieEntry
    {
        SingleDieEntry(
            const int InNetValue,
            const bool InHasOne,
            const long double InProbability)
            : NetValue(InNetValue),
            HasOne(InHasOne),
            Probability(InProbability)
        {
        }

        int NetValue;
        bool HasOne;
        long double Probability;
    };

    std::vector<SingleDieEntry> SingleDieDistribution;

    const long double OneFaceProbability =
        1.0L / static_cast<long double>(GlobalSideNumber);

    if (InWithCancel)
    {
        SingleDieDistribution.emplace_back(
            -1,
            true,
            OneFaceProbability);
    }
    else
    {
        SingleDieDistribution.emplace_back(
            0,
            false,
            OneFaceProbability);
    }

    const int InitialFailureFaceCount = InDifficulty - 2;

    if (InitialFailureFaceCount > 0)
    {
        SingleDieDistribution.emplace_back(
            0,
            false,
            static_cast<long double>(InitialFailureFaceCount) *
                OneFaceProbability);
    }

    if (!InWithTenReroll)
    {
        const int InitialSuccessFaceCount =
            GlobalSideNumber - InDifficulty + 1;

        if (InitialSuccessFaceCount > 0)
        {
            SingleDieDistribution.emplace_back(
                1,
                false,
                static_cast<long double>(InitialSuccessFaceCount) *
                    OneFaceProbability);
        }
    }
    else
    {
        const int InitialNonTenSuccessFaceCount =
            GlobalSideNumber - InDifficulty;

        if (InitialNonTenSuccessFaceCount > 0)
        {
            SingleDieDistribution.emplace_back(
                1,
                false,
                static_cast<long double>(InitialNonTenSuccessFaceCount) *
                    OneFaceProbability);
        }

        long double ChainProbability = OneFaceProbability;

        const long double TerminalFailureProbability =
            static_cast<long double>(InDifficulty - 1) /
            static_cast<long double>(GlobalSideNumber);

        const long double TerminalSuccessProbability =
            static_cast<long double>(GlobalSideNumber - InDifficulty) /
            static_cast<long double>(GlobalSideNumber);

        for (int TenCount = 1; TenCount < MaxTenReroll; ++TenCount)
        {
            SingleDieDistribution.emplace_back(
                TenCount,
                false,
                ChainProbability * TerminalFailureProbability);

            SingleDieDistribution.emplace_back(
                TenCount + 1,
                false,
                ChainProbability * TerminalSuccessProbability);

            ChainProbability *= OneFaceProbability;
        }

        SingleDieDistribution.emplace_back(
            MaxTenReroll,
            false,
            ChainProbability);
    }

    const int MinDieNet = InWithCancel ? -1 : 0;
    const int MaxDieNet = GetMaxSuccessPerDie(InWithTenReroll);

    std::vector<long double> CurrentNoOneDistribution(1, 1.0L);
    std::vector<long double> CurrentHasOneDistribution(1, 0.0L);

    int CurrentMinNet = 0;
    int CurrentMaxNet = 0;

    for (int DiceIndex = 0; DiceIndex < InDiceCount; ++DiceIndex)
    {
        const int NextMinNet = CurrentMinNet + MinDieNet;
        const int NextMaxNet = CurrentMaxNet + MaxDieNet;

        std::vector<long double> NextNoOneDistribution(
            NextMaxNet - NextMinNet + 1,
            0.0L);

        std::vector<long double> NextHasOneDistribution(
            NextMaxNet - NextMinNet + 1,
            0.0L);

        for (int CurrentNet = CurrentMinNet; CurrentNet <= CurrentMaxNet; ++CurrentNet)
        {
            const int CurrentIndex = CurrentNet - CurrentMinNet;

            const long double CurrentNoOneProbability =
                CurrentNoOneDistribution[CurrentIndex];

            if (CurrentNoOneProbability > 0.0L)
            {
                for (const SingleDieEntry& Entry : SingleDieDistribution)
                {
                    const int NextNet = CurrentNet + Entry.NetValue;
                    const int NextIndex = NextNet - NextMinNet;

                    if (Entry.HasOne)
                    {
                        NextHasOneDistribution[NextIndex] +=
                            CurrentNoOneProbability * Entry.Probability;

                        continue;
                    }

                    NextNoOneDistribution[NextIndex] +=
                        CurrentNoOneProbability * Entry.Probability;
                }
            }

            const long double CurrentHasOneProbability =
                CurrentHasOneDistribution[CurrentIndex];

            if (CurrentHasOneProbability <= 0.0L)
            {
                continue;
            }

            for (const SingleDieEntry& Entry : SingleDieDistribution)
            {
                const int NextNet = CurrentNet + Entry.NetValue;
                const int NextIndex = NextNet - NextMinNet;

                NextHasOneDistribution[NextIndex] +=
                    CurrentHasOneProbability * Entry.Probability;
            }
        }

        CurrentNoOneDistribution.swap(NextNoOneDistribution);
        CurrentHasOneDistribution.swap(NextHasOneDistribution);

        CurrentMinNet = NextMinNet;
        CurrentMaxNet = NextMaxNet;
    }

    const bool CanBotch = InWithCancel && InModifier <= 0;

    std::vector<std::pair<int, long double>> Entries;

    if (CanBotch)
    {
        const long double NoSuccessFaceProbability =
            static_cast<long double>(InDifficulty - 1) /
            static_cast<long double>(GlobalSideNumber);

        const long double NoSuccessAndNoOneFaceProbability =
            static_cast<long double>(InDifficulty - 2) /
            static_cast<long double>(GlobalSideNumber);

        const long double BotchProbability =
            std::pow(NoSuccessFaceProbability, InDiceCount) -
            std::pow(NoSuccessAndNoOneFaceProbability, InDiceCount);

        Entries.emplace_back(-1, BotchProbability);
    }

    for (int TargetSuccessNum = 1;; ++TargetSuccessNum)
    {
        long double SuccessChanceValue = 0.0L;

        for (int NetValue = CurrentMinNet; NetValue <= CurrentMaxNet; ++NetValue)
        {
            const int ModifiedNetValue = NetValue + InModifier;

            if (ModifiedNetValue < TargetSuccessNum)
            {
                continue;
            }

            const int DistributionIndex = NetValue - CurrentMinNet;

            SuccessChanceValue +=
                CurrentNoOneDistribution[DistributionIndex];

            SuccessChanceValue +=
                CurrentHasOneDistribution[DistributionIndex];
        }

        if (SuccessChanceValue <= MinDisplayedProbability)
        {
            break;
        }

        Entries.emplace_back(TargetSuccessNum, SuccessChanceValue);
    }

    return ConvertProbabilityEntriesToString(Entries);
}
