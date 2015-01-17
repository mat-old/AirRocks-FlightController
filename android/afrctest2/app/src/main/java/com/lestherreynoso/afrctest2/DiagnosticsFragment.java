package com.lestherreynoso.afrctest2;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Rect;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.view.ViewGroup;
import android.widget.LinearLayout;
import android.widget.TextView;


/**
* A simple {@link Fragment} subclass.
* Activities that contain this fragment must implement the
* {@link DiagnosticsFragment.OnFragmentInteractionListener} interface
* to handle interaction events.
* Use the {@link DiagnosticsFragment#newInstance} factory method to
* create an instance of this fragment.
*/
public class DiagnosticsFragment extends Fragment {


    private Handler valueHandler;
    MainActivity mActivity = new MainActivity();
    TextView throttleValue;
    static View diagnosticsFragmentView;

    // TODO: Rename and change types and number of parameters
    public static DiagnosticsFragment newInstance(String param1, String param2) {
        DiagnosticsFragment fragment = new DiagnosticsFragment();
        return fragment;
    }

    public DiagnosticsFragment() {
        // Required empty public constructor
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        diagnosticsFragmentView = inflater.inflate(R.layout.fragment_diagnostics, container, false);
        return diagnosticsFragmentView;
    }

    @Override
    public void onViewCreated(View view, @Nullable Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);


        LinearLayout testll = (LinearLayout) view.findViewById(R.id.testll);
//       testll.addView(new ARFCCopter(getActivity().getApplicationContext(), 75, 75));
//        testll.addView(new Circle(getActivity().getApplicationContext(), 50, 50, 25));
    }

    public void setText(int viewId, String value){
        ((TextView)diagnosticsFragmentView.findViewById(viewId)).setText(value);
    }
}

