package com.lestherreynoso.afrctest2;

import android.content.Intent;
import android.os.Handler;
import android.os.Message;
import android.support.v7.app.ActionBarActivity;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentPagerAdapter;
import android.os.Bundle;
import android.support.v4.view.ViewPager;
import android.text.method.ScrollingMovementMethod;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ProgressBar;
import android.widget.TextView;


public class MainActivity extends ActionBarActivity {

    /**
     * The {@link android.support.v4.view.PagerAdapter} that will provide
     * fragments for each of the sections. We use a
     * {@link FragmentPagerAdapter} derivative, which will keep every
     * loaded fragment in memory. If this becomes too memory intensive, it
     * may be best to switch to a
     * {@link android.support.v4.app.FragmentStatePagerAdapter}.
     */
    SectionsPagerAdapter mSectionsPagerAdapter;

    /**
     * The {@link ViewPager} that will host the section contents.
     */
//    ViewPager mViewPager;
    static StoppableSwipeViewPager mViewPager;

    static Handler valueHandler;
    static TextView debugTV;
    static ARFCCopter mcopter;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);


        // Create the adapter that will return a fragment for each of the three
        // primary sections of the activity.
        mSectionsPagerAdapter = new SectionsPagerAdapter(getSupportFragmentManager());

        // Set up the ViewPager with the sections adapter.
        mViewPager = (StoppableSwipeViewPager) findViewById(R.id.pager);
//        mViewPager = (ViewPager) findViewById(R.id.pager);
        mViewPager.setAdapter(mSectionsPagerAdapter);
        debugTV = (TextView)findViewById(R.id.debugTV);
        debugTV.setMovementMethod(new ScrollingMovementMethod());

        valueHandler = new Handler(){

            @Override
            public void handleMessage(Message msg) {
                final int viewId = msg.getData().getInt("viewId");
                final String value = msg.getData().getString("value");
                final String viewType = msg.getData().getString("viewType");
                final String mode = msg.getData().getString("mode");
                final String log = viewId + " " + value + " " + viewType + " " + mode + " " + "\n";
                runOnUiThread(new Runnable() {

                    @Override
                    public void run() {
                        switch(viewType){
                            case "TextView":
                                if(mode == "set"){
                                    ((TextView) findViewById(viewId)).setText(value);
                                }
                                if(mode == "append"){
                                    ((TextView) findViewById(viewId)).append(value + "\n");
                                }

                                break;
                            case "ProgressBar":
                                ((ProgressBar) findViewById(viewId)).setProgress(Integer.parseInt(value));
                            default:
                                break;
                        }
//                        debugTV.append(log);

                    }
                });
                super.handleMessage(msg);
            }
        };

    }

    @Override
    protected void onPause() {
        super.onPause();
        if(mcopter != null) {
            mcopter.setIsRunning(false);
        }
    }

    @Override
    protected void onResume() {
        super.onResume();
        if(mcopter != null){
            mcopter.setIsRunning(true);
        }
    }

    public static void updateUI(final int viewId, final String value, final String viewType, final String mode){

        Message msg = valueHandler.obtainMessage();
        Bundle msgBundle = new Bundle();
        msgBundle.putString("viewType", viewType);
        msgBundle.putInt("viewId", viewId);
        msgBundle.putString("value", value);
        msgBundle.putString("mode", mode);
        msg.setData(msgBundle);

        valueHandler.handleMessage(msg);

    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();
        switch (id){
            case R.id.action_settings:
//            getFragmentManager().beginTransaction().replace(android.R.id.content, new SettingsFragment()).commit();

                Intent i = new Intent(this, SettingsActivity.class);
                startActivity(i);
                break;
            default:
                return true;
        }

        return super.onOptionsItemSelected(item);
    }

    public static void setMcopter(ARFCCopter copter) {
        mcopter = copter;
    }


    /**
     * A {@link FragmentPagerAdapter} that returns a fragment corresponding to
     * one of the sections/tabs/pages.
     */
    public class SectionsPagerAdapter extends FragmentPagerAdapter {

        public SectionsPagerAdapter(FragmentManager fm) {
            super(fm);
        }

        @Override
        public Fragment getItem(int position) {
            // getItem is called to instantiate the fragment for the given page.
            // Return a PlaceholderFragment (defined as a static inner class below).
//            return PlaceholderFragment.newInstance(position + 1);
            switch(position){
                case (0):
//                    return DiagnosticsFragment.newInstance("1", "2");
                    return NetworkFragment.newInstance("1", "2");
                case (1):
//                    return NetworkFragment.newInstance("1", "2");
                    return DiagnosticsFragment.newInstance("1", "2");
                case (2):
//                    return ControlFragment.newInstance("1", "2");
                    return new ControlFragment();
//                case (3):
//                    return RelayFragment.newInstance("1", "2");
//                case (10):
//                    return SettingsFragment.instantiate(getApplicationContext(), "settings");
                default:
                    return PlaceholderFragment.newInstance(position + 1);

            }
        }


        @Override
        public int getCount() {
            // Show 3 total pages.
            return 3;
        }
    }

    /**
     * A placeholder fragment containing a simple view.
     */
    public static class PlaceholderFragment extends Fragment {
        /**
         * The fragment argument representing the section number for this
         * fragment.
         */
        private static final String ARG_SECTION_NUMBER = "section_number";

        /**
         * Returns a new instance of this fragment for the given section
         * number.
         */
        public static PlaceholderFragment newInstance(int sectionNumber) {
            PlaceholderFragment fragment = new PlaceholderFragment();
            Bundle args = new Bundle();
            args.putInt(ARG_SECTION_NUMBER, sectionNumber);
            fragment.setArguments(args);
            return fragment;
        }

        public PlaceholderFragment() {
        }

        @Override
        public View onCreateView(LayoutInflater inflater, ViewGroup container,
                                 Bundle savedInstanceState) {
            View rootView = inflater.inflate(R.layout.fragment_main, container, false);
            return rootView;
        }
    }

}
