package com.lestherreynoso.airrocksflightcontroller;

import android.os.Bundle;
import android.os.Handler;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
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
    static ARFCCopter copter;
    TextView throttleValue;
    static View diagnosticsFragmentView;
    Button startBtn, stopBtn, takeOffBtn, landBtn, rotateCWBtn, rotateCCWBtn, pathSend, pathClear;
    MessageHandler messageHandler;
    private ShapeDrawing shape;

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

        messageHandler = new MessageHandler();
//        messageHandler = NetworkFragment.getMessageHandler();
        LinearLayout testll = (LinearLayout) view.findViewById(R.id.testll);
        copter = new ARFCCopter(getActivity().getApplicationContext(), 75, 75);
        MainActivity.setMcopter(copter);

        testll.addView(copter);

        LinearLayout inputll = (LinearLayout) view.findViewById(R.id.inputll);
        shape = new ShapeDrawing(getActivity().getApplicationContext());
        inputll.addView(shape);



        startBtn = (Button)view.findViewById(R.id.startBtn);
        startBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                messageHandler.sendMessage("start");
            }
        });
        stopBtn = (Button)view.findViewById(R.id.stopBtn);
        stopBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                messageHandler.sendMessage("stop");
            }
        });
        takeOffBtn = (Button)view.findViewById(R.id.takeOffBtn);
        takeOffBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                messageHandler.sendMessage("takeOff");
            }
        });
        landBtn = (Button)view.findViewById(R.id.landBtn);
        landBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                messageHandler.sendMessage("land");
            }
        });
        rotateCWBtn = (Button)view.findViewById(R.id.rotateCWBtn);
        rotateCWBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                messageHandler.sendMessage("rotateCW");
            }
        });
        rotateCCWBtn = (Button)view.findViewById(R.id.rotateCCWBtn);
        rotateCCWBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                messageHandler.sendMessage("rotateCCW");
            }
        });
        pathSend = (Button)view.findViewById(R.id.pathSend);
        pathSend.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                shape.sendPath();
            }
        });
        pathClear = (Button)view.findViewById(R.id.pathClear);
        pathClear.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                shape.clearPath();
            }
        });


//        testll.addView(new Circle(getActivity().getApplicationContext(), 50, 50, 25));
    }

    public void setText(int viewId, String value){
        ((TextView)diagnosticsFragmentView.findViewById(viewId)).setText(value);
    }

    public static ARFCCopter getCopter(){
        return copter;
    }
}

