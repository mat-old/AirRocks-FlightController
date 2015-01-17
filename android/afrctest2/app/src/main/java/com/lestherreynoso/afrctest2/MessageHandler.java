package com.lestherreynoso.afrctest2;

import android.net.Network;
import android.os.Bundle;
import android.os.Message;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

/**
 * Created by lesther on 1/9/2015.
 */
public class MessageHandler {
    JSONObject jsonMessage;
    Server server;

    public MessageHandler() {
        server = NetworkFragment.getServer();
    }

    public void sendMessage(String name, String value, String action){
        server.send(createJson(name, value, action).toString());
    }

    public void sendMessage(String name, int value, String action){
        server.send(createJson(name, value, action).toString());
    }

    public void sendMessage(String name, JSONArray value, String action){
        server.send(createJson(name, value, action).toString());
    }

    public void sendMessage(String action){
        server.send(createJson(action).toString());
    }

    private JSONObject createJson(String name, String value, String action) {
        JSONObject json = new JSONObject();
        try {
            json.put("name", name);
            json.put("value", value);
            json.put("action", action);
        } catch (JSONException e) {
            e.printStackTrace();
        }
        return json;
    }

    private JSONObject createJson(String name, JSONArray value, String action) {
        JSONObject json = new JSONObject();
        try {
            json.put("name", name);
            json.put("value", value);
            json.put("action", action);
        } catch (JSONException e) {
            e.printStackTrace();
        }
        return json;
    }

    private JSONObject createJson(String name, int value, String action) {
        JSONObject json = new JSONObject();
        try {
            json.put("name", name);
            json.put("value", value);
            json.put("action", action);
        } catch (JSONException e) {
            e.printStackTrace();
        }
        return json;
    }

    private JSONObject createJson(String action){
        JSONObject json = new JSONObject();
        try {
            json.put("action", action);
        } catch (JSONException e) {
            e.printStackTrace();
        }
        return json;
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
                    MainActivity.updateUI(R.id.throttleValue, value, "TextView", "set");
                    MainActivity.updateUI(R.id.throttle, value, "ProgressBar", "set");
                    break;
                case "MotorA":
                    MainActivity.updateUI(R.id.motorAValue, value, "TextView", "set");
                    MainActivity.updateUI(R.id.motorA, value, "ProgressBar", "set");
                    break;
                case "MotorB":
                    MainActivity.updateUI(R.id.motorBValue, value, "TextView", "set");
                    MainActivity.updateUI(R.id.motorB, value, "ProgressBar", "set");
                    break;
                case "MotorC":
                    MainActivity.updateUI(R.id.motorCValue, value, "TextView", "set");
                    MainActivity.updateUI(R.id.motorC, value, "ProgressBar", "set");
                    break;
                case "MotorD":
                    MainActivity.updateUI(R.id.motorDValue, value, "TextView", "set");
                    MainActivity.updateUI(R.id.motorD, value, "ProgressBar", "set");
                    break;
                case "Pitch":
                    MainActivity.updateUI(R.id.pitchValue, value, "TextView", "set");
                    if (Integer.parseInt(value) > 0){

                        MainActivity.updateUI(R.id.posPitch, value, "ProgressBar", "set");
                        MainActivity.updateUI(R.id.negPitch, "0", "ProgressBar", "set");

                    }else if(Integer.parseInt(value) < 0){

                        MainActivity.updateUI(R.id.posPitch, "0", "ProgressBar", "set");
                        MainActivity.updateUI(R.id.negPitch, value, "ProgressBar", "set");

                    }else if (Integer.parseInt(value) == 0){

                        MainActivity.updateUI(R.id.posPitch, "0", "ProgressBar", "set");
                        MainActivity.updateUI(R.id.negPitch, "0", "ProgressBar", "set");

                    }
                    break;
                case "Roll":
                    MainActivity.updateUI(R.id.rollValue, value, "TextView", "set");
                    if (Integer.parseInt(value) > 0){

                        MainActivity.updateUI(R.id.posRoll, value, "ProgressBar", "set");
                        MainActivity.updateUI(R.id.negRoll, "0", "ProgressBar", "set");

                    }else if(Integer.parseInt(value) < 0){

                        MainActivity.updateUI(R.id.posRoll, "0", "ProgressBar", "set");
                        MainActivity.updateUI(R.id.negRoll, value, "ProgressBar", "set");

                    }else if (Integer.parseInt(value) == 0){

                        MainActivity.updateUI(R.id.posRoll, "0", "ProgressBar", "set");
                        MainActivity.updateUI(R.id.negRoll, "0", "ProgressBar", "set");

                    }
                    break;
                case "Yaw":
                    MainActivity.updateUI(R.id.yawValue, value, "TextView", "set");
                    if (Integer.parseInt(value) > 0){

                        MainActivity.updateUI(R.id.posYaw, value, "ProgressBar", "set");
                        MainActivity.updateUI(R.id.negYaw, "0", "ProgressBar", "set");

                    }else if(Integer.parseInt(value) < 0){

                        MainActivity.updateUI(R.id.posYaw, "0", "ProgressBar", "set");
                        MainActivity.updateUI(R.id.negYaw, value, "ProgressBar", "set");

                    }else if (Integer.parseInt(value) == 0){

                        MainActivity.updateUI(R.id.posYaw, "0", "ProgressBar", "set");
                        MainActivity.updateUI(R.id.negYaw, "0", "ProgressBar", "set");

                    }
                    break;
                default:
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

    
}
