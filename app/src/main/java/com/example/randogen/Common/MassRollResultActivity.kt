package com.example.randogen

import android.content.Context
import android.content.Intent
import android.os.Bundle
import android.view.View
import androidx.core.content.edit
import com.example.randogen.WoD.WodNativeBridge
import com.example.randogen.databinding.MassRollResultBinding

object MassRollResultContract
{
    private const val ExtraMode: String = "MassRollResult.Mode"

    private const val ModeWod: String = "Wod"
    private const val ModeRollAndKeep: String = "RollAndKeep"
    private const val ModeDnd: String = "Dnd"

    private const val ExtraDiceCount: String = "MassRollResult.DiceCount"
    private const val ExtraDifficulty: String = "MassRollResult.Difficulty"
    private const val ExtraWithCancel: String = "MassRollResult.WithCancel"
    private const val ExtraWithTenReroll: String = "MassRollResult.WithTenReroll"
    private const val ExtraModifier: String = "MassRollResult.Modifier"

    private const val ExtraRollNum: String = "MassRollResult.RollNum"
    private const val ExtraKeepNum: String = "MassRollResult.KeepNum"
    private const val ExtraBonus: String = "MassRollResult.Bonus"
    private const val ExtraExplodeTens: String = "MassRollResult.ExplodeTens"
    private const val ExtraExplodeOnes: String = "MassRollResult.ExplodeOnes"

    private const val ExtraDiceCounts: String = "MassRollResult.DiceCounts"
    private const val ExtraSideNumbers: String = "MassRollResult.SideNumbers"

    fun CreateWodIntent(
        ContextObj: Context,
        DiceCount: Int,
        Difficulty: Int,
        WithCancel: Boolean,
        WithTenReroll: Boolean,
        Modifier: Int): Intent
    {
        return Intent(ContextObj, MassRollResultActivity::class.java).apply {
            putExtra(ExtraMode, ModeWod)
            putExtra(ExtraDiceCount, DiceCount)
            putExtra(ExtraDifficulty, Difficulty)
            putExtra(ExtraWithCancel, WithCancel)
            putExtra(ExtraWithTenReroll, WithTenReroll)
            putExtra(ExtraModifier, Modifier)
        }
    }

    fun CreateRollAndKeepIntent(
        ContextObj: Context,
        RollNum: Int,
        KeepNum: Int,
        Bonus: Int,
        ExplodeTens: Boolean,
        ExplodeOnes: Boolean): Intent
    {
        return Intent(ContextObj, MassRollResultActivity::class.java).apply {
            putExtra(ExtraMode, ModeRollAndKeep)
            putExtra(ExtraRollNum, RollNum)
            putExtra(ExtraKeepNum, KeepNum)
            putExtra(ExtraBonus, Bonus)
            putExtra(ExtraExplodeTens, ExplodeTens)
            putExtra(ExtraExplodeOnes, ExplodeOnes)
        }
    }

    fun CreateDndIntent(
        ContextObj: Context,
        DiceCounts: IntArray,
        SideNumbers: IntArray,
        Modifier: Int): Intent
    {
        return Intent(ContextObj, MassRollResultActivity::class.java).apply {
            putExtra(ExtraMode, ModeDnd)
            putExtra(ExtraDiceCounts, DiceCounts)
            putExtra(ExtraSideNumbers, SideNumbers)
            putExtra(ExtraModifier, Modifier)
        }
    }

    fun GetMode(
        IntentObj: Intent): String?
    {
        return IntentObj.getStringExtra(ExtraMode)
    }

    fun IsWodMode(
        IntentObj: Intent): Boolean
    {
        return GetMode(IntentObj) == ModeWod
    }

    fun IsRollAndKeepMode(
        IntentObj: Intent): Boolean
    {
        return GetMode(IntentObj) == ModeRollAndKeep
    }

    fun IsDndMode(
        IntentObj: Intent): Boolean
    {
        return GetMode(IntentObj) == ModeDnd
    }

    fun GetDiceCount(
        IntentObj: Intent): Int
    {
        return IntentObj.getIntExtra(ExtraDiceCount, 4)
    }

    fun GetDifficulty(
        IntentObj: Intent): Int
    {
        return IntentObj.getIntExtra(ExtraDifficulty, 6)
    }

    fun GetWithCancel(
        IntentObj: Intent): Boolean
    {
        return IntentObj.getBooleanExtra(ExtraWithCancel, true)
    }

    fun GetWithTenReroll(
        IntentObj: Intent): Boolean
    {
        return IntentObj.getBooleanExtra(ExtraWithTenReroll, false)
    }

    fun GetModifier(
        IntentObj: Intent): Int
    {
        return IntentObj.getIntExtra(ExtraModifier, 0)
    }

    fun GetRollNum(
        IntentObj: Intent): Int
    {
        return IntentObj.getIntExtra(ExtraRollNum, 2)
    }

    fun GetKeepNum(
        IntentObj: Intent): Int
    {
        return IntentObj.getIntExtra(ExtraKeepNum, 1)
    }

    fun GetBonus(
        IntentObj: Intent): Int
    {
        return IntentObj.getIntExtra(ExtraBonus, 0)
    }

    fun GetExplodeTens(
        IntentObj: Intent): Boolean
    {
        return IntentObj.getBooleanExtra(ExtraExplodeTens, true)
    }

    fun GetExplodeOnes(
        IntentObj: Intent): Boolean
    {
        return IntentObj.getBooleanExtra(ExtraExplodeOnes, false)
    }

    fun GetDiceCounts(
        IntentObj: Intent): IntArray
    {
        return IntentObj.getIntArrayExtra(ExtraDiceCounts) ?: intArrayOf(1)
    }

    fun GetSideNumbers(
        IntentObj: Intent): IntArray
    {
        return IntentObj.getIntArrayExtra(ExtraSideNumbers) ?: intArrayOf(4)
    }
}

class MassRollResultActivity : BaseActivity()
{
    private companion object
    {
        const val PreferencesName: String = "MassRollResultState"
        const val DefaultMassRollCount: Int = 5
        const val SavedResultsTextKey: String = "MassRollResult.ResultsText"
        const val SavedRollCountKey: String = "MassRollResult.RollCount"
    }

    private lateinit var BindingObj: MassRollResultBinding
    private lateinit var RollCountStepper: NumberStepperController

    override fun onCreate(
        SavedInstanceState: Bundle?)
    {
        super.onCreate(SavedInstanceState)

        BindingObj = MassRollResultBinding.inflate(layoutInflater)
        setContentView(BindingObj.root)

        BindingObj.headerMassRollResult.textHeaderTitle.text =
            getString(R.string.title_mass_roll_result)

        BindingObj.headerMassRollResult.buttonHeaderNavigation.setOnClickListener {
            finish()
        }

        RollCountStepper = NumberStepperController(
            StepperBinding = BindingObj.includeMassRollCountStepper,
            GetMinValue = { InputLimits.MinDiceNumber },
            GetMaxValue = { InputLimits.MaxDiceNumber },
            OnValueChanged = {}
        )

        RollCountStepper.SetValue(
            GetRestoredRollCount(SavedInstanceState)
        )
        RollCountStepper.Bind()
        RestoreResultsText(SavedInstanceState)
        ApplyManualInputSettingForScreen()
    }

    override fun onStop()
    {
        SavePersistentState()

        super.onStop()
    }

    override fun onSaveInstanceState(
        OutState: Bundle)
    {
        OutState.putString(
            SavedResultsTextKey,
            BindingObj.textMassRollResults.text?.toString().orEmpty()
        )

        OutState.putInt(
            SavedRollCountKey,
            RollCountStepper.GetValue()
        )

        SavePersistentState()

        super.onSaveInstanceState(OutState)
    }

    override fun OnRollButtonClicked()
    {
        RunMassRolls()
    }

    override fun ApplyManualInputSettingForScreen()
    {
        if (!::RollCountStepper.isInitialized)
        {
            return
        }

        RollCountStepper.SetManualInputEnabled(
            IsManualInputEnabled()
        )
    }

    private fun RunMassRolls()
    {
        RollCountStepper.Clamp()

        val RollCount: Int =
            RollCountStepper.GetValue()

        val ResultLines: List<String> =
            (1..RollCount).mapNotNull {
                BuildMassRollLine()
            }

        BindingObj.textMassRollResults.text =
            if (ResultLines.isEmpty())
            {
                getString(R.string.data_empty)
            }
            else
            {
                ResultLines.joinToString("\n\n")
            }

        BindingObj.scrollMassRollResults.post {
            BindingObj.scrollMassRollResults.fullScroll(View.FOCUS_DOWN)
        }
    }

    private fun BuildMassRollLine(): String?
    {
        val IntentObj: Intent = intent

        return when
        {
            MassRollResultContract.IsWodMode(IntentObj) ->
                BuildWodMassRollLine(IntentObj)

            MassRollResultContract.IsRollAndKeepMode(IntentObj) ->
                BuildRollAndKeepMassRollLine(IntentObj)

            MassRollResultContract.IsDndMode(IntentObj) ->
                BuildDndMassRollLine(IntentObj)

            else -> null
        }
    }

    private fun BuildWodMassRollLine(
        IntentObj: Intent): String?
    {
        val Result: RollResults =
            WodNativeBridge.GetRollResultsJNI(
                MassRollResultContract.GetDiceCount(IntentObj),
                MassRollResultContract.GetDifficulty(IntentObj),
                MassRollResultContract.GetWithCancel(IntentObj),
                MassRollResultContract.GetWithTenReroll(IntentObj),
                MassRollResultContract.GetModifier(IntentObj),
                RollResults::class.java
            )
                ?: return null

        return FormatMassRollLine(
            DiceUiHelper.GetRollSequenceText(Result.GetRollSequence()),
            DiceUiHelper.GetSuccessNumText(
                Result.GetSuccessNum(),
                MassRollResultContract.GetWithCancel(IntentObj)
            )
        )
    }

    private fun BuildRollAndKeepMassRollLine(
        IntentObj: Intent): String?
    {
        val Result: RollResults =
            RollAndKeepNativeBridge.GetRollAndKeepResultsJNI(
                MassRollResultContract.GetRollNum(IntentObj),
                MassRollResultContract.GetKeepNum(IntentObj),
                MassRollResultContract.GetBonus(IntentObj),
                MassRollResultContract.GetExplodeTens(IntentObj),
                MassRollResultContract.GetExplodeOnes(IntentObj),
                RollResults::class.java
            )
                ?: return null

        return FormatMassRollLine(
            DiceUiHelper.GetRollSequenceText(Result.GetRollSequence()),
            Result.GetRollSum().toString()
        )
    }

    private fun BuildDndMassRollLine(
        IntentObj: Intent): String?
    {
        val Result: DndRollResults =
            DndNativeBridge.GetDndRollResultsJNI(
                MassRollResultContract.GetDiceCounts(IntentObj),
                MassRollResultContract.GetSideNumbers(IntentObj),
                MassRollResultContract.GetModifier(IntentObj),
                DndRollResults::class.java
            )
                ?: return null

        return FormatMassRollLine(
            Result.GetResultText(),
            Result.GetRollSum().toString()
        )
    }

    private fun FormatMassRollLine(
        RollText: String,
        ResultText: String): String
    {
        return "$RollText\n>> $ResultText"
    }

    private fun RestoreResultsText(
        SavedInstanceState: Bundle?)
    {
        val SavedResultsText: String =
            SavedInstanceState
                ?.getString(SavedResultsTextKey)
                ?: GetPreferences()
                    .getString(
                        GetResultsTextPreferenceKey(),
                        null
                    )
                ?: return

        BindingObj.textMassRollResults.text =
            SavedResultsText.ifBlank {
                getString(R.string.data_empty)
            }
    }

    private fun GetRestoredRollCount(
        SavedInstanceState: Bundle?): Int
    {
        return SavedInstanceState
            ?.getInt(SavedRollCountKey)
            ?: GetPreferences()
                .getInt(
                    GetRollCountPreferenceKey(),
                    DefaultMassRollCount
                )
    }

    private fun SavePersistentState()
    {
        if (!::BindingObj.isInitialized || !::RollCountStepper.isInitialized)
        {
            return
        }

        GetPreferences().edit {
            putString(
                GetResultsTextPreferenceKey(),
                BindingObj.textMassRollResults.text?.toString().orEmpty()
            )

            putInt(
                GetRollCountPreferenceKey(),
                RollCountStepper.GetValue()
            )
        }
    }

    private fun GetPreferences() =
        getSharedPreferences(
            PreferencesName,
            Context.MODE_PRIVATE
        )

    private fun GetResultsTextPreferenceKey(): String
    {
        return "${GetPersistentStateKey()}.ResultsText"
    }

    private fun GetRollCountPreferenceKey(): String
    {
        return "${GetPersistentStateKey()}.RollCount"
    }

    private fun GetPersistentStateKey(): String
    {
        val IntentObj: Intent = intent

        if (MassRollResultContract.IsWodMode(IntentObj))
        {
            return listOf(
                "Wod",
                MassRollResultContract.GetDiceCount(IntentObj),
                MassRollResultContract.GetDifficulty(IntentObj),
                MassRollResultContract.GetWithCancel(IntentObj),
                MassRollResultContract.GetWithTenReroll(IntentObj),
                MassRollResultContract.GetModifier(IntentObj)
            ).joinToString("|")
        }

        if (MassRollResultContract.IsRollAndKeepMode(IntentObj))
        {
            return listOf(
                "RollAndKeep",
                MassRollResultContract.GetRollNum(IntentObj),
                MassRollResultContract.GetKeepNum(IntentObj),
                MassRollResultContract.GetBonus(IntentObj),
                MassRollResultContract.GetExplodeTens(IntentObj),
                MassRollResultContract.GetExplodeOnes(IntentObj)
            ).joinToString("|")
        }

        if (MassRollResultContract.IsDndMode(IntentObj))
        {
            return listOf(
                "Dnd",
                MassRollResultContract.GetDiceCounts(IntentObj).joinToString(","),
                MassRollResultContract.GetSideNumbers(IntentObj).joinToString(","),
                MassRollResultContract.GetModifier(IntentObj)
            ).joinToString("|")
        }

        return "Default"
    }
}
