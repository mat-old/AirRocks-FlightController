package com.lestherreynoso.afrctest2;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Matrix;
import android.graphics.Paint;
import android.graphics.Rect;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

import java.lang.reflect.Array;
import java.util.ArrayList;
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
//    private float cwArmStartX;
//    private float cwArmStartY;
//    private float cwArmStopX;
//    private float cwArmStopY;
//    private float ccwArmStartX;
//    private float ccwArmStartY;
//    private float ccwArmStopX;
//    private float ccwArmStopY;
    private float[] cwArm, ccwArm;

//    private float motorAx;
//    private float motorAy;
//    private float motorBx;
//    private float motorBy;
//    private float motorCx;
//    private float motorCy;
//    private float motorDx;
//    private float motorDy;
    private float[] motorA, motorB, motorC, motorD;
    private float[] bladeA, bladeB, bladeC, bladeD;

    private Matrix rotateBladeA, rotateBladeB, rotateBladeC, rotateBladeD;
    private float angle;

    private float[] bladeTipA, bladeTipB, bladeTipC, bladeTipD;

//    private  float bladeLineXStart;
//    private  float bladeLineYStart;
//    private  float bladeLineXStop;
//    private  float bladeLineYStop;

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
        radius = 6;
        centerWidth = 15;
        centerHeight = 15;

//        cwArmStartX = mainX - centerWidth;
//        cwArmStartY = mainY - centerHeight;
//        cwArmStopX = mainX + (2 * centerWidth);
//        cwArmStopY = mainY + (2 * centerHeight);
        cwArm = new float[]{mainX - centerWidth, mainY - centerHeight, mainX + (2 * centerWidth), mainY + (2 * centerHeight)};
        ccwArm = new float[]{mainX - centerWidth,mainY + (2 * centerHeight), mainX + (2 * centerWidth), mainY - centerHeight};

//        motorAx = cwArmStartX;
//        motorAy = cwArmStartY;
        motorA = new float[] {cwArm[0], cwArm[1]};
//        motorA = cwArm;
//        motorBy = ccwArmStartY;
        motorB = new float[] {ccwArm[0], ccwArm[1]};
//        motorCx = ccwArmStopX;
//        motorCy = ccwArmStopY;
        motorC = new float[] {ccwArm[2], ccwArm[3]};
//        motorDx = cwArmStopX;
//        motorDy = cwArmStopY;
        motorD = new float[] {cwArm[2], cwArm[3]};

        bladeTipA = new float[] {cwArm[0], cwArm[1]};
        bladeTipB = new float[] {ccwArm[0], ccwArm[1]};
        bladeTipC = new float[] {ccwArm[2], ccwArm[3]};
        bladeTipD = new float[] {cwArm[2], cwArm[3]};

        movingRight = true;

        bladeA = new float[] {motorA[0], motorA[1] - (2 * radius), motorA[0], motorA[1] + (2 * radius)};
        bladeB = new float[] {motorB[0], motorB[1] - (2 * radius), motorB[0], motorB[1] + (2 * radius)};
        bladeC = new float[] {motorC[0], motorC[1] - (2 * radius), motorC[0], motorC[1] + (2 * radius)};
        bladeD = new float[] {motorD[0], motorD[1] - (2 * radius), motorD[0], motorD[1] + (2 * radius)};
        rotateBladeA = new Matrix();
        rotateBladeB = new Matrix();
        rotateBladeC = new Matrix();
        rotateBladeD = new Matrix();
        angle = 1;
//        rotateBladeA.setRotate(angle, motorA[0], motorA[1]);
//        rotateBladeA.mapPoints(bladeA);


//        bladeLineXStart;
//        bladeLineYStart;
//        bladeLineXStop;
//        bladeLineYStop;

    }
    public void moveCopter(){
        if (this.movingRight) {
            this.mainX++;
        } else {
            this.mainX--;
        }

//        cwArmStartX = mainX - centerWidth;
//        cwArmStartY = mainY - centerHeight;
//        cwArmStopX = mainX + (2 * centerWidth);
//        cwArmStopY = mainY + (2 * centerHeight);
//        ccwArmStartX = mainX - centerWidth;
//        ccwArmStartY = mainY + (2 * centerHeight);
//        ccwArmStopX = mainX + (2 * centerWidth);
//        ccwArmStopY = mainY - centerHeight;

        cwArm = new float[]{mainX - centerWidth, mainY - centerHeight, mainX + (2 * centerWidth), mainY + (2 * centerHeight)};
        ccwArm = new float[]{mainX - centerWidth,mainY + (2 * centerHeight), mainX + (2 * centerWidth), mainY - centerHeight};


//        motorAx = cwArmStartX;
//        motorAy = cwArmStartY;
//        motorBx = ccwArmStartX;
//        motorBy = ccwArmStartY;
//        motorCx = ccwArmStopX;
//        motorCy = ccwArmStopY;
//        motorDx = cwArmStopX;
//        motorDy = cwArmStopY;

        motorA = new float[] {cwArm[0], cwArm[1]};
        motorB = new float[] {ccwArm[0], ccwArm[1]};
        motorC = new float[] {ccwArm[2], ccwArm[3]};
        motorD = new float[] {cwArm[2], cwArm[3]};

        bladeTipA = new float[] {cwArm[0], cwArm[1]};
        bladeTipB = new float[] {ccwArm[0], ccwArm[1]};
        bladeTipC = new float[] {ccwArm[2], ccwArm[3]};
        bladeTipD = new float[] {cwArm[2], cwArm[3]};

        bladeA = new float[] {motorA[0], motorA[1] - (2 * radius), motorA[0], motorA[1] + (2 * radius)};
        bladeB = new float[] {motorB[0], motorB[1] - (2 * radius), motorB[0], motorB[1] + (2 * radius)};
        bladeC = new float[] {motorC[0], motorC[1] - (2 * radius), motorC[0], motorC[1] + (2 * radius)};
        bladeD = new float[] {motorD[0], motorD[1] - (2 * radius), motorD[0], motorD[1] + (2 * radius)};

        angle +=24;
        if (angle >= 360){angle = 1;}       //reset angle after full rotation

        rotateBladeA.setRotate(angle, motorA[0], motorA[1]);
        rotateBladeB.setRotate(-angle, motorB[0], motorB[1]);
        rotateBladeC.setRotate(-angle, motorC[0], motorC[1]);
        rotateBladeD.setRotate(angle, motorD[0], motorD[1]);

        rotateBladeA.mapPoints(bladeA);
        rotateBladeB.mapPoints(bladeB);
        rotateBladeC.mapPoints(bladeC);
        rotateBladeD.mapPoints(bladeD);

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
        drawBlades(mainCanvas);
        drawBladeTips(mainCanvas);
    }
    //*************************************************************************************
    private void drawBladeTips(Canvas bladeTipCanvas) {
        bladeTipCanvas.drawCircle(bladeTipA[0], bladeTipA[1], radius/3, motorPaint);
        bladeTipCanvas.drawCircle(bladeTipB[0], bladeTipB[1], radius/3, motorPaint);
        bladeTipCanvas.drawCircle(bladeTipC[0], bladeTipC[1], radius/3, motorPaint);
        bladeTipCanvas.drawCircle(bladeTipD[0], bladeTipD[1], radius/3, motorPaint);
    }

    private void drawBlades(Canvas bladesCanvas) {
        bladesCanvas.drawLine(bladeA[0], bladeA[1], bladeA[2], bladeA[3], mainPaint);
        bladesCanvas.drawLine(bladeB[0], bladeB[1], bladeB[2], bladeB[3], mainPaint);
        bladesCanvas.drawLine(bladeC[0], bladeC[1], bladeC[2], bladeC[3], mainPaint);
        bladesCanvas.drawLine(bladeD[0], bladeD[1], bladeD[2], bladeD[3], mainPaint);

    }

    private void drawMotors(Canvas motorCanvas) {
        motorCanvas.drawCircle(motorA[0], motorA[1], radius, ccwPaint);
        motorCanvas.drawCircle(motorB[0], motorB[1], radius, cwPaint);
        motorCanvas.drawCircle(motorC[0], motorC[1], radius, cwPaint);
        motorCanvas.drawCircle(motorD[0], motorD[1], radius, ccwPaint);

    }


    private void drawArms(Canvas armsCanvas) {
//        float[] arms = {cwArmStartX, cwArmStartY, cwArmStopX, cwArmStopY,
//                        ccwArmStartX, ccwArmStartY, ccwArmStopX, ccwArmStopY};
        float[] arms = {cwArm[0], cwArm[1], cwArm[2], cwArm[3],
                        ccwArm[0], ccwArm[1], ccwArm[2], ccwArm[3]};
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
        mainCanvas.drawColor(Color.LTGRAY);
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
