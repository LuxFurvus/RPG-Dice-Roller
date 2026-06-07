package com.example.randogen.WoD

import android.widget.CheckBox
import com.example.randogen.InputLimits
import com.example.randogen.NumberStepperController
import com.example.randogen.databinding.NumberStepperBinding

class WodInputPanelController(
    private val DiceNumberStepperBinding: NumberStepperBinding,
    private val DifficultyStepperBinding: NumberStepperBinding,
    private val ModifierStepperBinding: NumberStepperBinding,
    private val CancelCheckbox: CheckBox,
    private val TenRerollCheckbox: CheckBox
)
{
    private var OnInputChangedCallback: (() -> Unit)? = null

    private val DiceNumberStepperController = NumberStepperController(
        StepperBinding = DiceNumberStepperBinding,
        GetMinValue = { InputLimits.MinDiceNumber },
        GetMaxValue = { InputLimits.MaxDiceNumber },
        OnValueChanged = { OnInputChangedCallback?.invoke() }
    )

    private val DifficultyStepperController = NumberStepperController(
        StepperBinding = DifficultyStepperBinding,
        GetMinValue = { InputLimits.MinDifficulty },
        GetMaxValue = { InputLimits.MaxDifficulty },
        OnValueChanged = { OnInputChangedCallback?.invoke() }
    )

    private val ModifierStepperController = NumberStepperController(
        StepperBinding = ModifierStepperBinding,
        GetMinValue = { InputLimits.MinBonusNum },
        GetMaxValue = { InputLimits.MaxBonusNum },
        OnValueChanged = { OnInputChangedCallback?.invoke() }
    )

    fun SetManualInputEnabled(
        IsEnabled: Boolean)
    {
        DiceNumberStepperController.SetManualInputEnabled(IsEnabled)
        DifficultyStepperController.SetManualInputEnabled(IsEnabled)
        ModifierStepperController.SetManualInputEnabled(IsEnabled)
    }

    fun Bind(
        OnInputChanged: () -> Unit)
    {
        OnInputChangedCallback = OnInputChanged

        DiceNumberStepperController.Bind()
        DifficultyStepperController.Bind()
        ModifierStepperController.Bind()

        CancelCheckbox.setOnCheckedChangeListener { _, _ ->
            OnInputChanged()
        }

        TenRerollCheckbox.setOnCheckedChangeListener { _, _ ->
            OnInputChanged()
        }
    }

    fun ClampAll()
    {
        DiceNumberStepperController.Clamp()
        DifficultyStepperController.Clamp()
        ModifierStepperController.Clamp()
    }

    fun GetDiceNumberValue(): Int
    {
        return DiceNumberStepperController.GetValue()
    }

    fun GetDifficultyValue(): Int
    {
        return DifficultyStepperController.GetValue()
    }

    fun GetModifierValue(): Int
    {
        return ModifierStepperController.GetValue()
    }

    fun IsCancelEnabled(): Boolean
    {
        return CancelCheckbox.isChecked
    }

    fun IsTenRerollEnabled(): Boolean
    {
        return TenRerollCheckbox.isChecked
    }

    fun RestoreState(
        DiceNumberValue: Int,
        DifficultyValue: Int,
        ModifierValue: Int,
        WithCancelValue: Boolean,
        WithTenReroll: Boolean)
    {
        DiceNumberStepperController.SetValue(DiceNumberValue)
        DifficultyStepperController.SetValue(DifficultyValue)
        ModifierStepperController.SetValue(ModifierValue)

        CancelCheckbox.isChecked = WithCancelValue
        TenRerollCheckbox.isChecked = WithTenReroll

        DiceNumberStepperController.Clamp()
        DifficultyStepperController.Clamp()
        ModifierStepperController.Clamp()
    }
}