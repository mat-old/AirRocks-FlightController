package com.lestherreynoso.airrocksflightcontroller;

import android.content.Context;
import android.support.v4.view.ViewPager;
import android.util.AttributeSet;
import android.view.MotionEvent;

/**
 * Created by lesther on 1/17/2015.
 */
public class StoppableSwipeViewPager extends ViewPager {

    Boolean swipeEnabled;

    public StoppableSwipeViewPager(Context context, AttributeSet attrs) {
        super(context, attrs);
        this.swipeEnabled = true;
    }

    @Override
    public boolean onInterceptTouchEvent(MotionEvent ev) {
        if(this.swipeEnabled){
            return super.onInterceptTouchEvent(ev);
        }

        return false;
    }

    @Override
    public boolean onTouchEvent(MotionEvent ev) {
        if(this.swipeEnabled){
            return super.onTouchEvent(ev);
        }

        return false;
    }

    public void setSwipeEnabled(Boolean enabled){
        this.swipeEnabled = enabled;
    }
}
