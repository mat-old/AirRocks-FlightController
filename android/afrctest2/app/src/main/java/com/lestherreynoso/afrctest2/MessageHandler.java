package com.lestherreynoso.afrctest2;

import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.widget.TextView;

import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayList;

/**
 * Created by lesther on 1/9/2015.
 */
public class MessageHandler {
    JSONObject jsonMessage;
//    private Handler valueHandler;
    MainActivity mActivity = new MainActivity();
    DiagnosticsFragment diagnosticsFragment = new DiagnosticsFragment();
//    DiagnosticsFragment diagnosticsFragment = (DiagnosticsFragment)getSupportFragmentManager().findFragmentById(R.id.)


    public MessageHandler() {
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
    }

    public void sendMessage(){

    }

    public void read(String message){
        jsonMessage = toJSON(message);
        interpretJSON(jsonMessage);
    }

    private void interpretJSON(JSONObject jsonMessage) {
        String name = "";
        String action = "";
        String value = "";
        try {
            name = (String) jsonMessage.get("name");
        } catch (JSONException e) {
            e.printStackTrace();
        }
        try {
            value = (String) jsonMessage.get("value");
        } catch (JSONException e) {
            e.printStackTrace();
        }
        if (name.isEmpty()){
            try {
                action = (String) jsonMessage.get("action");
            } catch (JSONException e) {
                e.printStackTrace();
            }
            if(!action.isEmpty()) {
                switch (action) {
                    case "Pitch-activate":
                        break;
                    default:
                        break;
                }
            }
        }else{
            switch (name) {
                case "Throttle-Torque":
                    update(R.id.throttleValue, value.toString(), "TextView");
                    update(R.id.throttle, value.toString(), "ProgressBar");
                    break;
                default:
                    //no name
                    break;
            }
        }



    }

    private JSONObject toJSON(String message) {
        JSONObject decodedString = new JSONObject();


        message = message.replaceAll("\\{|\\}|\"", "");
        String[] keyValuePairs = message.split(",");
        for(String kv : keyValuePairs){
            String[] keyValue = kv.split(":");
            try {
                decodedString.put(keyValue[0], keyValue[1]);
            } catch (JSONException e) {
                e.printStackTrace();
            }
        }

        //TODO if invalid json?
        return decodedString;
    }

    public void read(JSONObject jsonMessage){
        jsonMessage = jsonMessage;
    }

    public void update(final int viewId, final String value, final String viewType){
//        ((TextView)mActivity.findViewById(viewId)).setText(value);
//        mActivity.runOnUiThread(new Runnable(){
//
//            @Override
//            public void run() {
//                ((TextView)mActivity.findViewById(viewId)).setText(value);
//            }
//        });
//        diagnosticsFragment.setText(viewId, value);

//        ((TextView)DiagnosticsFragment.diagnosticsFragmentView.findViewById(viewId)).setText(value);
        Message msg = MainActivity.valueHandler.obtainMessage();
        Bundle msgBundle = new Bundle();
        msgBundle.putString("viewType", viewType);
        msgBundle.putInt("viewId", viewId);
        msgBundle.putString("value", value);
        msg.setData(msgBundle);
//        msg.arg1 = viewId;
//        msg.obj = value;
        MainActivity.valueHandler.handleMessage(msg);

    }
}
