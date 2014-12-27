package com.lestherreynoso.afrctest2;

import android.content.Context;
import android.os.Handler;
import android.os.Message;
import android.util.Log;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;

/**
* Created by Kpable on 12/23/2014.
*/
public class Server {
    DatagramSocket datagramSocket;
    Handler updateUIHandler;
    Thread receiveThread;
    Thread commThread;
    public static final int SERVER_PORT = 5005;
    Context context;
//    Network.DebugManager debugManager;
    Handler networkDebugHandler;


    public void start(Handler ndHandler) {
        updateUIHandler = new Handler();
        this.receiveThread = new Thread(new ReceiveRunnable());
        this.networkDebugHandler = ndHandler;
//        this.context = context;

        receiveThread.start();
    }

    public void stop(){
        receiveThread.interrupt();
    }

    class ReceiveRunnable implements  Runnable {
        @Override
        public void run() {
            byte[] receivedData = new byte[10];
            DatagramPacket receivedPacket = new DatagramPacket(receivedData, receivedData.length);
            try {
                datagramSocket = new DatagramSocket(SERVER_PORT);
            } catch (IOException e) {
                e.printStackTrace();
            }

            while (!Thread.currentThread().isInterrupted()){
//              while(true){
                try{
                    receivedPacket.setData(new byte[10]);
                    datagramSocket.receive(receivedPacket);
                    String sen = new String(receivedPacket.getData());
                    Log.d("Received ",sen.trim());
                    Message msg = networkDebugHandler.obtainMessage();
                    msg.obj = sen;
                    networkDebugHandler.handleMessage(msg);

                }catch (IOException e){
                    e.printStackTrace();
                }

            }
            datagramSocket.close();
        }
    }

//    class ReceiveRunnable implements  Runnable {
//        @Override
//        public void run() {
//            byte[] receivedData = new byte[10];
//            DatagramPacket receivedPacket = new DatagramPacket(receivedData, receivedData.length);
//            try {
//                datagramSocket = new DatagramSocket(SERVER_PORT);
//            } catch (IOException e) {
//                e.printStackTrace();
//            }
//
//            while (!Thread.currentThread().isInterrupted()){
////              while(true){
//                try{
//                    receivedPacket.setData(new byte[10]);
//                    datagramSocket.receive(receivedPacket);
//                    String sen = new String(receivedPacket.getData());
//                    Log.d("Received ",sen.trim());
//                    Message msg = networkDebugHandler.obtainMessage();
//                    msg.obj = sen;
//                    networkDebugHandler.handleMessage(msg);
//
//                }catch (IOException e){
//                    e.printStackTrace();
//                }
//
//            }
//            datagramSocket.close();
//        }
//    }

}
