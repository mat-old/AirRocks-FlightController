package com.lestherreynoso.afrctest2;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Rect;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

import java.util.List;

/**
 * Created by lesther on 1/7/2015.
 */
public class ARFCCopter extends SurfaceView implements SurfaceHolder.Callback {
    private FlyThread fThread;

    private float mainX;
    private float mainY;
    private Paint cwPaint;
    private Paint ccwPaint;
    private Paint motorPaint;
    private Paint mainPaint;
    private int radius;
    private float centerWidth;
    private float centerHeight;
    private float cwArmStartX;
    private float cwArmStartY;
    private float cwArmStopX;
    private float cwArmStopY;
    private float ccwArmStartX;
    private float ccwArmStartY;
    private float ccwArmStopX;
    private float ccwArmStopY;

    private float motorAx;
    private float motorAy;
    private float motorBx;
    private float motorBy;
    private float motorCx;
    private float motorCy;
    private float motorDx;
    private float motorDy;

    private List<Float> bladeA;

    private Boolean movingRight;

    public ARFCCopter(Context context, float x, float y) {
        super(context);
        this.mainX = x;
        this.mainY = y;

        getHolder().addCallback(this);
        this.fThread = new FlyThread(getHolder(), this);

        cwPaint = new Paint(Paint.ANTI_ALIAS_FLAG);
        cwPaint.setColor(Color.GREEN);
        ccwPaint = new Paint(Paint.ANTI_ALIAS_FLAG);
        ccwPaint.setColor(Color.CYAN);
        motorPaint = new Paint(Paint.ANTI_ALIAS_FLAG);
        motorPaint.setColor(Color.BLACK);
        mainPaint = new Paint(Paint.ANTI_ALIAS_FLAG);
        mainPaint.setColor(Color.BLUE);
        radius = 8;
        centerWidth = 15;
        centerHeight = 15;

        cwArmStartX = mainX - centerWidth;
        cwArmStartY = mainY - centerHeight;
        cwArmStopX = mainX + (2 * centerWidth);
        cwArmStopY = mainY + (2 * centerHeight);
        ccwArmStartX = mainX - centerWidth;
        ccwArmStartY = mainY + (2 * centerHeight);
        ccwArmStopX = mainX + (2 * centerWidth);
        ccwArmStopY = mainY - centerHeight;

        motorAx = cwArmStartX;
        motorAy = cwArmStartY;
        motorBx = ccwArmStartX;
        motorBy = ccwArmStartY;
        motorCx = ccwArmStopX;
        motorCy = ccwArmStopY;
        motorDx = cwArmStopX;
        motorDy = cwArmStopY;

        movingRight = true;

    }
    public void moveCopter(){
        if (this.movingRight) {
            this.mainX++;
        } else {
            this.mainX--;
        }

        cwArmStartX = mainX - centerWidth;
        cwArmStartY = mainY - centerHeight;
        cwArmStopX = mainX + (2 * centerWidth);
        cwArmStopY = mainY + (2 * centerHeight);
        ccwArmStartX = mainX - centerWidth;
        ccwArmStartY = mainY + (2 * centerHeight);
        ccwArmStopX = mainX + (2 * centerWidth);
        ccwArmStopY = mainY - centerHeight;

        motorAx = cwArmStartX;
        motorAy = cwArmStartY;
        motorBx = ccwArmStartX;
        motorBy = ccwArmStartY;
        motorCx = ccwArmStopX;
        motorCy = ccwArmStopY;
        motorDx = cwArmStopX;
        motorDy = cwArmStopY;

        if (this.mainX == (getWidth() - centerWidth)) {
            this.movingRight = false;
        } else if (this.mainX == (0 + centerWidth)) {
            this.movingRight = true;
        }
    }

//    public void draw(Canvas centerCanvas, Canvas armsCanvas, Canvas motorCanvas){
//        drawCenter(centerCanvas);
//        drawArms(armsCanvas);
//        drawMotors(motorCanvas);
////        drawBlades(bladesCanvas);
//    }
//*************************************************************************************

    public void draw(Canvas mainCanvas){
        drawCenter(mainCanvas);
        drawArms(mainCanvas);
        drawMotors(mainCanvas);
    }
//*************************************************************************************

    private void drawBlades(Canvas bladesCanvas) {
//        bladesCanvas.drawLine();
    }

    private void drawMotors(Canvas motorCanvas) {
        motorCanvas.drawCircle(motorAx, motorAy, radius, ccwPaint);
        motorCanvas.drawCircle(motorBx, motorBy, radius, cwPaint);
        motorCanvas.drawCircle(motorCx, motorCy, radius, cwPaint);
        motorCanvas.drawCircle(motorDx, motorDy, radius, ccwPaint);

    }


    private void drawArms(Canvas armsCanvas) {
        float[] arms = {cwArmStartX, cwArmStartY, cwArmStopX, cwArmStopY,
                        ccwArmStartX, ccwArmStartY, ccwArmStopX, ccwArmStopY};
        armsCanvas.drawLines(arms, mainPaint);
    }

    private void drawCenter(Canvas centerCanvas) {
        centerCanvas.drawRect(mainX, mainY, mainX + centerWidth, mainY + centerHeight, mainPaint);
    }

//    public void clearCopter(Canvas centerCanvas, Canvas armsCanvas, Canvas motorCanvas){
//        centerCanvas.drawColor(Color.WHITE);
//        armsCanvas.drawColor(Color.WHITE);
//        motorCanvas.drawColor(Color.WHITE);
//    }

    public void clearCopter(Canvas mainCanvas){
        mainCanvas.drawColor(Color.WHITE);
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        fThread.start();
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {

    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {

    }

    private class FlyThread extends Thread{
        private SurfaceHolder surfaceHolder;
        private ARFCCopter arfcCopter;

        FlyThread(SurfaceHolder surfaceHolder, ARFCCopter arfcCopter){
            this.surfaceHolder = surfaceHolder;
            this.arfcCopter = arfcCopter;
        }

        public void run(){
//            Canvas centerCanvas = null;
//            Canvas armsCanvas = null;
//            Canvas motorCanvas = null;
            Canvas mainCanvas = null;

            while(true){
                try{
//                    centerCanvas = surfaceHolder.lockCanvas();
//                    armsCanvas = surfaceHolder.lockCanvas();
//                    motorCanvas = surfaceHolder.lockCanvas();
                    mainCanvas = surfaceHolder.lockCanvas();
                    synchronized (surfaceHolder){
                        arfcCopter.clearCopter(mainCanvas);
//                        arfcCopter.clearCopter(centerCanvas, armsCanvas, motorCanvas);
                        arfcCopter.moveCopter();
//                        arfcCopter.draw(centerCanvas, armsCanvas, motorCanvas);
                        arfcCopter.draw(mainCanvas);
                    }


                }finally {
                    if (mainCanvas != null){
                        surfaceHolder.unlockCanvasAndPost(mainCanvas);
                    }
//                    if (centerCanvas != null){
//                        surfaceHolder.unlockCanvasAndPost(centerCanvas);
//                    }
//                    if (armsCanvas != null){
//                        surfaceHolder.unlockCanvasAndPost(armsCanvas);
//                    }
//                    if (motorCanvas != null){
//                        surfaceHolder.unlockCanvasAndPost(motorCanvas);
//                    }
                }
            }
        }
    }
}
