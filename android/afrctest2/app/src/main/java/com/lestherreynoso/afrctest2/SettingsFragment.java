package com.lestherreynoso.afrctest2;

import android.content.SharedPreferences;
import android.os.Bundle;
import android.preference.Preference;
import android.preference.PreferenceFragment;
import android.preference.PreferenceManager;

/**
 * Created by Kpable on 1/9/2015.
 */
public class SettingsFragment extends PreferenceFragment implements SharedPreferences.OnSharedPreferenceChangeListener {
    SharedPreferences sharedprefs;
    SharedPreferences.Editor editor;
    private Preference routerSSIDPreference;
    private Preference routerPasswordPreference;
    private Preference ipAddressPreference;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        addPreferencesFromResource(R.xml.preferences);

        sharedprefs = PreferenceManager.getDefaultSharedPreferences(getActivity());
        sharedprefs.registerOnSharedPreferenceChangeListener(this);
        editor = sharedprefs.edit();

        routerSSIDPreference = (Preference) findPreference("routerSSIDPreferenceKey");
        routerSSIDPreference.setSummary(sharedprefs.getString("routerSSIDPreferenceKey", ""));
        routerPasswordPreference = (Preference) findPreference("routerPasswordPreferenceKey");
        routerPasswordPreference.setSummary(sharedprefs.getString("routerPasswordPreferenceKey", ""));
        ipAddressPreference = (Preference) findPreference("ipEditTextPreferenceKey");
        ipAddressPreference.setSummary(sharedprefs.getString("ipEditTextPreferenceKey", ""));

    }

    @Override
    public void onSharedPreferenceChanged(SharedPreferences sharedPreferences, String key) {
        if(key.equals("routerSSIDPreferenceKey")){
            routerSSIDPreference.setSummary(sharedPreferences.getString(key, "Not yet set"));
        }
        if(key.equals("routerPasswordPreferenceKey")){
            routerPasswordPreference.setSummary(sharedPreferences.getString(key, "Not yet set"));
        }
        if(key.equals("ipEditTextPreferenceKey")){
            ipAddressPreference.setSummary(sharedPreferences.getString(key, "Not yet set"));
        }
    }
}
