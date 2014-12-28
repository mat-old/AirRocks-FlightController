package com.lestherreynoso.afrctest2;

import android.content.Context;
import android.os.Handler;
import android.os.Message;
import android.util.Log;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;

/**
* Created by Kpable on 12/23/2014.
*/
public class Server {
    DatagramSocket datagramSocket;
    Handler updateUIHandler;
    Thread receiveThread;
    Thread sendThread;
    Thread commThread;
    SendRunnable sendRunnable = new SendRunnable();
    Boolean sendNotStarted = true;

    public static final int SERVER_PORT = 5000;
    Context context;
//    Network.DebugManager debugManager;
    Handler networkDebugHandler;
    InetAddress ipAddress;


    public void start(Handler ndHandler) {
        updateUIHandler = new Handler();
        this.receiveThread = new Thread(new ReceiveRunnable());
        this.sendThread = new Thread(new SendRunnable());
        this.networkDebugHandler = ndHandler;
//        this.context = context;

        receiveThread.start();


    }

    public void stop(){
        receiveThread.interrupt();
        sendThread.interrupt();
    }

    public void send(String msg) {
        sendRunnable.send(msg);

        if (sendNotStarted){
            sendThread.start();
            sendNotStarted = false;
        }
    }

    class ReceiveRunnable implements  Runnable {
        @Override
        public void run() {
            byte[] receivedData = new byte[1024];
            DatagramPacket receivedPacket = new DatagramPacket(receivedData, receivedData.length);
            try {
                datagramSocket = new DatagramSocket(SERVER_PORT);
            } catch (IOException e) {
                e.printStackTrace();
            }

            while (!Thread.currentThread().isInterrupted()){
//              while(true){
                try{
                    receivedPacket.setData(new byte[1024]);
                    datagramSocket.receive(receivedPacket);
                    ipAddress = receivedPacket.getAddress();
                    String sen = new String(receivedPacket.getData());
                    Log.d("Received ",sen.trim());
                    Message msg = networkDebugHandler.obtainMessage();
                    msg.obj = "Received: " +sen;
                    networkDebugHandler.handleMessage(msg);

                }catch (IOException e){
                    e.printStackTrace();
                }

            }
            datagramSocket.close();
        }
    }

    class SendRunnable implements  Runnable {
        String sendMsg = "testing";

        public void send(String msg){
            this.sendMsg = msg.toUpperCase();
        }
        @Override
        public void run() {
            while(!sendMsg.isEmpty() && !Thread.currentThread().isInterrupted()){
//            while(true)
                byte[] sendData = sendMsg.getBytes();
                byte[] ip = {64,-88,42,1};
                DatagramPacket sendPacket = new DatagramPacket(sendData, sendData.length, ipAddress, 5001);
                try {
                    datagramSocket = new DatagramSocket(5001);
                    datagramSocket.send(sendPacket);
                    String sen = sendMsg;
                    Log.d("Sent ",sen.trim());
                    Message msg = networkDebugHandler.obtainMessage();
                    msg.obj = "sent:"+sen;
                    networkDebugHandler.handleMessage(msg);
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }

}
