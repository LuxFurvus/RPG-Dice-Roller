package com.example.randogen.WoD

import com.example.randogen.RollResults

object WodNativeBridge
{
    init
    {
        System.loadLibrary("randogen")
    }

    external fun GetRollResultsJNI(
        DiceCount: Int,
        DifficultyValue: Int,
        WithCancel: Boolean,
        InWithTenReroll: Boolean,
        InModifier: Int,
        ResultClass: Class<RollResults>
    ): RollResults?

    external fun GetSuccessChancesJNI(
        DiceCount: Int,
        DifficultyValue: Int,
        WithCancel: Boolean,
        InWithTenReroll: Boolean,
        InModifier: Int
    ): String

}