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
//        SurfaceView surfaceView = (SurfaceView) view.findViewById(R.id.surfaceView);
//        Rect testrect = new Rect();
//        testrect.set(0, 0, surfaceView.getMeasuredWidth(), surfaceView.getMeasuredHeight());
//        Paint blue = new Paint();
//        blue.setColor(Color.BLUE);
//        blue.setStyle(Paint.Style.STROKE);
//        Canvas c = new Canvas();
//        c.drawRect(testrect, blue);
//        surfaceView.draw(c);
//        throttleValue = (TextView) view.findViewById(R.id.throttleValue);
//        throttleValue.setText("test");
//        valueHandler = new Handler(){
//
//            @Override
//            public void handleMessage(Message msg) {
//                final int viewId = msg.arg1;
//                final String value = (String) msg.obj;
//                mActivity.runOnUiThread(new Runnable(){
//
//                    @Override
//                    public void run() {
//                        ((TextView)mActivity.findViewById(viewId)).setText(value);
//                    }
//                });
//                super.handleMessage(msg);
//            }
//        };

        LinearLayout testll = (LinearLayout) view.findViewById(R.id.testll);
//       testll.addView(new ARFCCopter(getActivity().getApplicationContext(), 75, 75));
//        testll.addView(new Circle(getActivity().getApplicationContext(), 50, 50, 25));
    }

    public void setText(int viewId, String value){
        ((TextView)diagnosticsFragmentView.findViewById(viewId)).setText(value);
    }
}

//class Circle extends SurfaceView implements SurfaceHolder.Callback {
//
//    private float x;
//    private float y;
//    private int radius;
//    private Paint paint;
//    private MainThread thread;
//    private boolean movingRight = true;
//
//    public Circle(Context context, float x, float y, int radius) {
//        super(context);
//
//        this.x = x;
//        this.y = y;
//        this.radius = radius;
//
//        // Tell the SurfaceHolder ( -> getHolder() ) to receive SurfaceHolder
//        // callbacks
//        getHolder().addCallback(this);
//        this.thread = new MainThread(getHolder(), this);
//
//        paint = new Paint(Paint.ANTI_ALIAS_FLAG);
//        paint.setColor(Color.BLUE);
//    }
//
//    // Specify wheater to move right or left
//    public void moveCircle() {
//
//        if (this.movingRight) {
//            this.x++;
//        } else {
//            this.x--;
//        }
//
//        if (this.x == (getWidth() - this.radius)) {
//            this.movingRight = false;
//        } else if (this.x == (0 + this.radius)) {
//            this.movingRight = true;
//        }
//
//    }
//
//    public void draw(Canvas canvas) {
//        canvas.drawCircle(this.x, this.y, this.radius, this.paint);
//
//    }
//
//    public void clearCircle(Canvas canvas) {
//        canvas.drawColor(Color.WHITE);
//    }
//
//    @Override
//    public void surfaceChanged(SurfaceHolder arg0, int arg1, int arg2, int arg3) {
//        // TODO Auto-generated method stub
//    }
//
//    @Override
//    public void surfaceCreated(SurfaceHolder arg0) {
//        thread.start();
//    }
//
//    @Override
//    public void surfaceDestroyed(SurfaceHolder arg0) {
//        // TODO Auto-generated method stub
//    }
//}
//
//class MainThread extends Thread {
//
//    private SurfaceHolder surfaceHolder;
//    private Circle circle;
//
//    public MainThread(SurfaceHolder surfaceHolder, Circle circle) {
//        this.surfaceHolder = surfaceHolder;
//        this.circle = circle;
//    }
//
//    public void run() {
//        Canvas canvas = null;
//
//        while (true) {
//
//            try {
//                canvas = surfaceHolder.lockCanvas();
//                synchronized (surfaceHolder) {
//                    circle.clearCircle(canvas);
//                    circle.moveCircle();
//                    circle.draw(canvas);
//                }
//            } finally {
//                if (canvas != null) {
//                    surfaceHolder.unlockCanvasAndPost(canvas);
//                }
//            }
//
//        }
//    }
//}