package com.example.randogen

import android.os.Bundle
import com.example.randogen.databinding.MassRollResultBinding

class MassRollResultActivity : BaseActivity()
{
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

        RollCountStepper.SetValue(1)
        RollCountStepper.Bind()
        ApplyManualInputSettingForScreen()
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
}
