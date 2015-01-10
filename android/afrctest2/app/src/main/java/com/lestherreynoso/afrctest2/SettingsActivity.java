package com.lestherreynoso.afrctest2;

import android.content.SharedPreferences;
import android.preference.Preference;
import android.preference.PreferenceFragment;
import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;


public class SettingsActivity extends ActionBarActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        getFragmentManager().beginTransaction()
                .replace(android.R.id.content, new SettingsFragment())
                .commit();
    }


}


//public class SettingsFragment extends PreferenceFragment {
////        Preference overallDelayPicker;
////        SharedPreferences sharedprefs;
////        SharedPreferences.Editor editor;
//
//    @Override
//    public void onCreate(Bundle savedInstanceState) {
//        super.onCreate(savedInstanceState);
//
//        // Load the preferences from an XML resource
//        addPreferencesFromResource(R.xml.preferences);
//
//
//    }
//}

