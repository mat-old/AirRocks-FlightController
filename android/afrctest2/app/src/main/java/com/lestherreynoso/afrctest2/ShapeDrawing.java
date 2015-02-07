package com.lestherreynoso.afrctest2;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Path;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;

import org.json.JSONArray;

import java.lang.reflect.Array;
import java.util.ArrayList;
import java.util.List;
import java.util.Vector;

/**
 * Created by lesther on 1/17/2015.
 */
public class ShapeDrawing extends View {
    Path path;
    Paint paint;
    Paint textPaint;
    Canvas c;
    String coor;
    Float xx, yy;
    ArrayList<float[]> pathCoordinates;
    MessageHandler messageHandler;
    public ShapeDrawing(Context context) {
        super(context);
        pathCoordinates = new ArrayList<>();
        messageHandler = new MessageHandler();
        path = new Path();
        c = new Canvas();
        paint = new Paint();
        paint.setColor(Color.BLUE);
        paint.setStyle(Paint.Style.STROKE);
        paint.setStrokeJoin(Paint.Join.ROUND);
        paint.setStrokeCap(Paint.Cap.ROUND);
        paint.setStrokeWidth(20);
        textPaint = new Paint();
        textPaint.setColor(Color.RED);
        textPaint.setTextSize(20);
        coor="nothingyet";
        xx=1f;
        yy=1f;

    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);
        canvas.drawColor(Color.LTGRAY);

        canvas.drawPath(path, paint);

//        canvas.drawText(coor, xx,yy , textPaint);
    }

    private float mX, mY;
    private static final float TOUCH_TOLERANCE = 4;



    @Override
    public boolean onTouchEvent(MotionEvent event) {
        float x = event.getX();
        float y = event.getY();

        switch (event.getAction()) {
            case MotionEvent.ACTION_DOWN:
                touch_start(x, y);
                MainActivity.mViewPager.setSwipeEnabled(false);
                postInvalidate();
                break;
            case MotionEvent.ACTION_MOVE:
                touch_move(x, y);
                postInvalidate();
                break;
            case MotionEvent.ACTION_UP:
                touch_up();
                MainActivity.mViewPager.setSwipeEnabled(true);
                postInvalidate();
                break;
        }
        return true;
    }

    private void touch_start(float x, float y) {
        path.reset();
        path.moveTo(x, y);
        mX = x;
        mY = y;
        pathCoordinates.clear();
        pathCoordinates.add(new float[]{x,y});

    }
    private void touch_move(float x, float y) {
        float dx = Math.abs(x - mX);
        float dy = Math.abs(y - mY);
        if (dx >= TOUCH_TOLERANCE || dy >= TOUCH_TOLERANCE) {
            path.quadTo(mX, mY, (x + mX)/2, (y + mY)/2);

            mX = x;
            mY = y;
            pathCoordinates.add(new float[]{x,y});
        }
        xx = x;
        yy = y;

    }
    private void touch_up() {
        preparePathForSending(pathCoordinates);
        path.lineTo(mX, mY);
        c.drawPath(path, paint);
//        path.reset();
//        c.drawText("{"+ x + ","+y+"}", x, y, textPaint);

    }

    private void preparePathForSending(ArrayList<float[]> pathCoordinates) {

        ArrayList<int[]> pC = new ArrayList<>();
        for(float[] pair : pathCoordinates){
            int x = Math.round((pair[0] / this.getWidth()) * 100) ;
            int y = Math.round((pair[1] / this.getHeight()) * 100) ;
            pC.add(new int[] {x, y});
//            String temp = "{"+ x + ","+y+"}";
//            c.drawText(temp, pair[0], pair[1], textPaint);
        }

        JSONArray jsonArray = new JSONArray(pC);

        messageHandler.sendMessage("path", jsonArray, "set");
    }
}
