package com.lestherreynoso.afrctest2;

import android.os.Bundle;
import android.preference.PreferenceFragment;

/**
 * Created by Kpable on 1/9/2015.
 */
public class SettingsFragment extends PreferenceFragment {
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        addPreferencesFromResource(R.xml.preferences);
    }
}
