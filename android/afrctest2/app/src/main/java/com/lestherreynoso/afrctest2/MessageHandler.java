package com.lestherreynoso.afrctest2;

import android.os.Handler;
import android.os.Message;
import android.widget.TextView;

import org.json.JSONException;
import org.json.JSONObject;

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
        switch (name) {
            case "Throttle-Torque":
                update(R.id.throttleValue, value.toString());
                break;
            default:
                //no name
                break;
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

    public void update(final int viewId, final String value){
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
        msg.arg1 = viewId;
        msg.obj = value;
        MainActivity.valueHandler.handleMessage(msg);

    }
}
