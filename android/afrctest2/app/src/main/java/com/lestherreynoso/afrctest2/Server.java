package com.lestherreynoso.afrctest2;

import android.content.Context;
import android.os.AsyncTask;
import android.os.Handler;
import android.os.Message;
import android.util.Log;

import org.json.JSONObject;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.UnknownHostException;

/**
* Created by Kpable on 12/23/2014.
*/
public class Server {
    DatagramSocket recvDatagramSocket;
    DatagramSocket sendDatagramSocket;
    Handler updateUIHandler;
    Thread receiveThread;
//    Thread sendThread;
//    Thread commThread;
//    SendRunnable sendRunnable = new SendRunnable();
    Boolean sendNotStarted = true;
    String sendMsg = "";
    MessageHandler messageHandler = new MessageHandler();
    public static final int SERVER_PORT = 5000;
    Context context;
//    Network.DebugManager debugManager;
    Handler networkDebugHandler;
    InetAddress ipAddress;


    public void start(Handler ndHandler, String ipadstring) {
        updateUIHandler = new Handler();
        this.receiveThread = new Thread(new ReceiveRunnable());
        this.networkDebugHandler = ndHandler;
//        this.context = context;
//        this.sendThread = new Thread(new SendRunnable());
//        sendThread.start();
        receiveThread.start();
        if (ipadstring.isEmpty()){
            ipadstring = "192.168.42.1";
        }
        else{
            ipadstring = "192.168.0.6";
        }
        try {
            ipAddress = InetAddress.getByName(ipadstring);
        } catch (UnknownHostException e) {
            Message errorMsg = new Message();
            errorMsg.obj = "failed to connect to "+ipadstring;
            ndHandler.handleMessage(errorMsg);
            e.printStackTrace();
        }

    }

    public void stop(){
        receiveThread.interrupt();
//        sendThread.interrupt();
    }

    public void send(String message) {
//         sendRunnable.send(msg);
        sendMsg = message;
        new SendMessage().execute(sendMsg);
//        if(!sendMsg.isEmpty()){
//            byte[] sendData = sendMsg.getBytes();
//            DatagramPacket sendPacket = new DatagramPacket(sendData, sendData.length, ipAddress, 5001);
//            if (ipAddress != null) {
//                try {
//                    datagramSocket = new DatagramSocket(5001);
//                    datagramSocket.send(sendPacket);
//                    String sen = sendMsg;
//                    Log.d("Sent ", sen.trim());
//                    Message msg = networkDebugHandler.obtainMessage();
//                    msg.obj = "sent:" + sen;
//                    networkDebugHandler.handleMessage(msg);
//                } catch (IOException e) {
//                    e.printStackTrace();
//                }finally{
////                    datagramSocket.close();
//                }
//            }
//        }
    }

    class ReceiveRunnable implements  Runnable {
        @Override
        public void run() {
            byte[] receivedData = new byte[1024];
            DatagramPacket receivedPacket = new DatagramPacket(receivedData, receivedData.length);
            try {
                recvDatagramSocket = new DatagramSocket(SERVER_PORT);
            } catch (IOException e) {
                e.printStackTrace();
            }

            while (!Thread.currentThread().isInterrupted()){
//              while(true){
                try{
                    receivedPacket.setData(new byte[1024]);
                    recvDatagramSocket.receive(receivedPacket);
//                    ipAddress = receivedPacket.getAddress();
                    String stringMessage = new String(receivedPacket.getData());
                    stringMessage = stringMessage.trim();
                    messageHandler.read(stringMessage);
//                    Log.d("Received ",sen.trim());
                    Message msg = networkDebugHandler.obtainMessage();
                    msg.obj = "Received: " +stringMessage;
                    networkDebugHandler.handleMessage(msg);

                }catch (IOException e){
                    e.printStackTrace();
                }

            }
            recvDatagramSocket.close();
        }
    }
    public  class SendMessage extends AsyncTask<String, Void, Void>{

        @Override
        protected Void doInBackground(String... params) {
            if(!params[0].isEmpty()){
                byte[] sendData = params[0].getBytes();
                DatagramPacket sendPacket = new DatagramPacket(sendData, sendData.length, ipAddress, 5001);
                if (ipAddress != null) {
                    try {
                        sendDatagramSocket = new DatagramSocket(5001);
                        sendDatagramSocket.send(sendPacket);
                        String sen = params[0];

                        Log.d("Sent ", sen.trim());
                        Message msg = networkDebugHandler.obtainMessage();
                        msg.obj = "sent:" + sen;
                        networkDebugHandler.handleMessage(msg);
                    } catch (IOException e) {
                        e.printStackTrace();
                    }finally{
                    sendDatagramSocket.close();
                    }
                }
            }
            return null;
        }
    }
//    class SendRunnable implements  Runnable {
////        String sendMsg = "testing";
//
////        public void send(String msg){
////            this.sendMsg = msg.toUpperCase();
////        }
//        @Override
//        public void run() {
//            while(!sendMsg.isEmpty() && !Thread.currentThread().isInterrupted()){
////            while(true)
//                            byte[] sendData = sendMsg.getBytes();
//                            byte[] ip = {64,-88,42,1};
//                            try {
//                                ipAddress = InetAddress.getByName("192.168.42.1");
//                            } catch (UnknownHostException e) {
//                                e.printStackTrace();
//                            }
//                            DatagramPacket sendPacket = new DatagramPacket(sendData, sendData.length, ipAddress, 5001);
//                            if (ipAddress != null) {
//                                try {
//                                    datagramSocket = new DatagramSocket(5001);
//                                    datagramSocket.send(sendPacket);
//                                    String sen = sendMsg;
//                                    Log.d("Sent ", sen.trim());
//                                    Message msg = networkDebugHandler.obtainMessage();
//                                    msg.obj = "sent:" + sen;
//                                    networkDebugHandler.handleMessage(msg);
//                                } catch (IOException e) {
//                                    e.printStackTrace();
//                    }
//                }
//            }
//        }
//    }

}
