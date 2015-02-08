package com.lestherreynoso.afrctest2;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Matrix;
import android.graphics.Paint;
import android.graphics.Path;
import android.graphics.PathMeasure;
import android.graphics.RectF;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

import java.util.ArrayList;

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

    private float[] cwArm, ccwArm;

    private float[] motorA, motorB, motorC, motorD;
    private float[] bladeA, bladeB, bladeC, bladeD;

    private Matrix rotateBladeA, rotateBladeB, rotateBladeC, rotateBladeD, rotateAll;
    private float angle;
    private float allAngle;

    private RectF centerRect;

    private float[] bladeTipA, bladeTipB, bladeTipC, bladeTipD;



    private Boolean movingRight;

    private Boolean isRunning;
    private boolean isTherePath;
    private ArrayList<int[]> thePoints;

    private Path path;
    private PathMeasure pathMeasure;
    float[] pos, tan;
    float speed, distance;
    private boolean isFollow;


    public ARFCCopter(Context context, float x, float y) {
        super(context);
        this.mainX = x;
        this.mainY = y;
        this.isRunning = true;
        this.isTherePath = false;
        this.isFollow = false;
        getHolder().addCallback(this);
//        this.fThread = new FlyThread(getHolder(), this);

        init();
        update();
    }

    private void init() {
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

        rotateBladeA = new Matrix();
        rotateBladeB = new Matrix();
        rotateBladeC = new Matrix();
        rotateBladeD = new Matrix();
        rotateAll = new Matrix();
        angle = 1;
        allAngle = 1;
        movingRight = true;
        }

    private void update() {

//        centerRect = new RectF(mainX, mainY, mainX + centerWidth, mainY + centerHeight);

        cwArm = new float[]{mainX - centerWidth, mainY - centerHeight, mainX + (2 * centerWidth), mainY + (2 * centerHeight)};
        ccwArm = new float[]{mainX - centerWidth,mainY + (2 * centerHeight), mainX + (2 * centerWidth), mainY - centerHeight};

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

        allAngle +=6;
        if (allAngle >= 360){allAngle = 1;}       //reset angle after full rotation

        rotateBladeA.setRotate(angle, motorA[0], motorA[1]);
        rotateBladeB.setRotate(-angle, motorB[0], motorB[1]);
        rotateBladeC.setRotate(-angle, motorC[0], motorC[1]);
        rotateBladeD.setRotate(angle, motorD[0], motorD[1]);

//        rotateAll.setRotate(allAngle, mainX + centerWidth/2, mainY + centerWidth/2);
//        rotateAll.mapRect(centerRect);

        rotateBladeA.mapPoints(bladeA);
        rotateBladeB.mapPoints(bladeB);
        rotateBladeC.mapPoints(bladeC);
        rotateBladeD.mapPoints(bladeD);


//        centerRect = new RectF(mainX, mainY, mainX + centerWidth, mainY + centerHeight);
    }

    public void moveCopter(){
        if (this.movingRight) {
            this.mainX++;
        } else {
            this.mainX--;
        }

        update();

        if (this.mainX == (getWidth() - 2 * centerWidth)) {
            this.movingRight = false;
        } else if (this.mainX == (centerWidth)) {
            this.movingRight = true;
        }
    }


    public float[] getRelativePosition(float x, float y){
        float mx =  this.getWidth() * (x / 100);
        float my =  this.getHeight() * (y / 100);
        float[] fpos = new float[]{mx, my};

        return fpos;
    }

    public void setPosition(float[] pos){
        this.mainX = pos[0];
        this.mainY = pos[1];
    }

    public void setPath(ArrayList<int[]> intpath){
        this.thePoints = intpath;
        isTherePath = true;
    }


    public void draw(Canvas mainCanvas){
        drawCenter(mainCanvas);
        drawArms(mainCanvas);
        drawMotors(mainCanvas);
        drawBlades(mainCanvas);
        drawBladeTips(mainCanvas);
//        mainCanvas.save();
//        mainCanvas.rotate(allAngle);
//        mainCanvas.restore();
    }

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
        float[] arms = {cwArm[0], cwArm[1], cwArm[2], cwArm[3],
                        ccwArm[0], ccwArm[1], ccwArm[2], ccwArm[3]};
        armsCanvas.drawLines(arms, mainPaint);
    }

    private void drawCenter(Canvas centerCanvas) {
        centerCanvas.drawRect(mainX, mainY, mainX + centerWidth, mainY + centerHeight, mainPaint);
//        centerCanvas.drawRect(centerRect, mainPaint);
    }

    public void clearCopter(Canvas mainCanvas){
        mainCanvas.drawColor(Color.LTGRAY);
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        this.fThread = new FlyThread(getHolder(), this);
        fThread.start();
//        if(!fThread.isAlive()){
//            fThread.start();
//        }
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        fThread.interrupt();
    }

    public void setIsRunning(boolean b) {
        this.isRunning = b;
    }

    public void setFollow(boolean b) {
        this.isFollow = b;
    }

    private class FlyThread extends Thread{
        private SurfaceHolder surfaceHolder;
        private ARFCCopter arfcCopter;

        FlyThread(SurfaceHolder surfaceHolder, ARFCCopter arfcCopter){
            this.surfaceHolder = surfaceHolder;
            this.arfcCopter = arfcCopter;
        }

        public void run(){
            Canvas mainCanvas = null;

//            while(!Thread.currentThread().isInterrupted()){
            while(isRunning){

                if(isTherePath){
                    int n = thePoints.size();
                    for(int i = 0; i < n; i++ ){
                        if(i+1 != n) {
                            setPathBetween(thePoints.get(i), thePoints.get(i + 1));
                            while (distance < pathMeasure.getLength()) {
                                float[] p = getPathPointsBetween();
                                try {

                                    mainCanvas = surfaceHolder.lockCanvas();
                                    synchronized (surfaceHolder) {
                                        arfcCopter.clearCopter(mainCanvas);

                                        arfcCopter.moveCopterThroughPath(p);
                                        arfcCopter.draw(mainCanvas);
                                    }
                                } finally {
                                    if (mainCanvas != null) {
                                        surfaceHolder.unlockCanvasAndPost(mainCanvas);
                                    }
                                }
                            }
                        }
//                        else{
//                            String got = "got";
//                        }
                    }
                    isTherePath = false;
                }else if(isFollow){
                    try {

                        mainCanvas = surfaceHolder.lockCanvas();
                        synchronized (surfaceHolder) {
                            arfcCopter.clearCopter(mainCanvas);
                            arfcCopter.update();
                            arfcCopter.draw(mainCanvas);
                        }
                    } finally {
                        if (mainCanvas != null) {
                            surfaceHolder.unlockCanvasAndPost(mainCanvas);
                        }
                    }
                }else{             //default left and right movement
                    try {

                        mainCanvas = surfaceHolder.lockCanvas();
                        synchronized (surfaceHolder) {
                            arfcCopter.clearCopter(mainCanvas);
                            arfcCopter.moveCopter();
                            arfcCopter.draw(mainCanvas);
                        }
                    } finally {
                        if (mainCanvas != null) {
                            surfaceHolder.unlockCanvasAndPost(mainCanvas);
                        }
                    }
                }
            }
        }
    }

    private float[] getPathPointsBetween() {
        pathMeasure.getPosTan(distance, pos, tan);
        distance += speed;
        return pos;
    }

    private void setPathBetween(int[] start, int[] end) {
        path = new Path();
        path.moveTo(start[0], start[1]);
        path.lineTo(end[0], end[1]);
        pathMeasure = new PathMeasure(path, false);
        speed = pathMeasure.getLength() / 10;
        pos = new float[2];
        tan = new float[2];
        distance = 0;
//        pathMeasure.getPosTan(distance, pos, tan);
//        distance += speed;
    }


    private void moveCopterThroughPath(float[] p) {
//        while(distance < pathMeasure.getLength()){
//            pathMeasure.getPosTan(distance, pos, tan);
//            distance += speed;
//        }
        float[] rPos = getRelativePosition(p[0], p[1]);
        this.mainX = rPos[0];
        this.mainY = rPos[1];
        update();
    }
}
