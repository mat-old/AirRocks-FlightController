package com.lestherreynoso.airrocksflightcontroller;

import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.widget.EditText;


/**
 * A simple {@link Fragment} subclass.
 * Activities that contain this fragment must implement the
 * {@link RelayFragment.OnFragmentInteractionListener} interface
 * to handle interaction events.
 * Use the {@link RelayFragment#newInstance} factory method to
 * create an instance of this fragment.
 */
public class RelayFragment extends Fragment {
    // TODO: Rename parameter arguments, choose names that match
    // the fragment initialization parameters, e.g. ARG_ITEM_NUMBER
    private static final String ARG_PARAM1 = "param1";
    private static final String ARG_PARAM2 = "param2";
    WebView webView;

    // TODO: Rename and change types of parameters
    private String mParam1;
    private String mParam2;

    /**
     * Use this factory method to create a new instance of
     * this fragment using the provided parameters.
     *
     * @param param1 Parameter 1.
     * @param param2 Parameter 2.
     * @return A new instance of fragment RelayFragment.
     */
    // TODO: Rename and change types and number of parameters
    public static RelayFragment newInstance(String param1, String param2) {
        RelayFragment fragment = new RelayFragment();
        Bundle args = new Bundle();
        args.putString(ARG_PARAM1, param1);
        args.putString(ARG_PARAM2, param2);
        fragment.setArguments(args);
        return fragment;
    }

    public RelayFragment() {
        // Required empty public constructor
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        return inflater.inflate(R.layout.fragment_relay, container, false);
    }

    @Override
    public void onViewCreated(View view, @Nullable Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);
        webView = (WebView) view.findViewById(R.id.relayWebView);
        WebSettings webSettings = webView.getSettings();
        webSettings.setJavaScriptEnabled(true);

        String relayIp =((EditText)view.findViewById(R.id.relayIpEditText)).getText().toString();
//        String relayIp = "";
        if(relayIp.isEmpty() || relayIp.equals(null)) {
            webView.loadUrl("http://192.168.0.6:8888/tuner");
        }
        else{
            webView.loadUrl("http://" + relayIp +":8888/tuner");
        }
//        webView.loadUrl("https://www.google.com");
    }


}
