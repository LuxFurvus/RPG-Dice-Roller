package com.example.randogen

import android.content.Context
import androidx.core.content.edit

object AppKeepScreenOnSettings
{
    private const val PreferencesName: String = "AppKeepScreenOnSettings"
    private const val KeepScreenOnEnabledKey: String = "KeepScreenOnEnabled"
    private const val DefaultKeepScreenOnEnabled: Boolean = true

    fun IsKeepScreenOnEnabled(
        ContextObj: Context): Boolean
    {
        return ContextObj
            .getSharedPreferences(
                PreferencesName,
                Context.MODE_PRIVATE
            )
            .getBoolean(
                KeepScreenOnEnabledKey,
                DefaultKeepScreenOnEnabled
            )
    }

    fun SetKeepScreenOnEnabled(
        ContextObj: Context,
        IsEnabled: Boolean)
    {
        ContextObj
            .getSharedPreferences(
                PreferencesName,
                Context.MODE_PRIVATE
            )
            .edit {
                putBoolean(
                    KeepScreenOnEnabledKey,
                    IsEnabled
                )
            }
    }
}
