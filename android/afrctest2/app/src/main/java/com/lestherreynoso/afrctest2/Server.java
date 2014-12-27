package com.lestherreynoso.afrctest2;

import android.content.Context;
import android.os.Handler;
import android.os.Message;
import android.util.Log;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.ServerSocket;
import java.net.Socket;

/**
* Created by Kpable on 12/23/2014.
*/
public class Server {
    DatagramSocket datagramSocket;
    Handler updateUIHandler;
    Thread serverThread;
    Thread commThread;
    public static final int SERVER_PORT = 5005;
    Context context;
//    Network.DebugManager debugManager;
    Handler networkDebugHandler;


    public void start(Handler ndHandler) {
        updateUIHandler = new Handler();
        this.serverThread = new Thread(new ServerRun());
        this.networkDebugHandler = ndHandler;
//        this.context = context;

        serverThread.start();
    }

    class ServerRun implements  Runnable {
        @Override
        public void run() {
            byte[] receivedData = new byte[10];
            DatagramPacket receivedPacket = new DatagramPacket(receivedData, receivedData.length);
            try {
                datagramSocket = new DatagramSocket(SERVER_PORT);
            } catch (IOException e) {
                e.printStackTrace();
            }

//            while (!Thread.currentThread().isInterrupted()){
              while(true){
                try{
                    datagramSocket.receive(receivedPacket);
                    String sen = new String(receivedPacket.getData());
                    Log.d("Received ",sen.trim());
                    Message msg = networkDebugHandler.obtainMessage();
                    msg.obj = sen;
//                    networkDebugHandler.handleMessage(msg);
//                    Network.mhandler.post(new updateUI("socket accepted.."+sen, context));
//                    CommT commRunnable = new CommT(datagramSocket);
//                    new Thread(commRunnable).start();

                }catch (IOException e){
                    e.printStackTrace();
                }
//                datagramSocket.close();
            }
        }
    }
//    class CommT implements Runnable {
//        DatagramSocket clientSocket;
//        BufferedReader input;
//
//        public CommT(DatagramSocket clientSocket){
//            this.clientSocket = clientSocket;
////            try {
////               // this.input = new BufferedReader(new InputStreamReader(this.clientSocket));
////            } catch (IOException e) {
////                e.printStackTrace();
////            }
//        }
//
//        @Override
//        public void run() {
//            while (!Thread.currentThread().isInterrupted()){
//                try{
//                    String read = input.readLine();
//                    updateUIHandler.post(new updateUI(read));
//                }catch (IOException e){
//                    e.printStackTrace();
//                }
//            }
//        }
//    }


    private class updateUI implements Runnable {
        String msg;
        Context  context;
        public updateUI(String read, Context context) {
            this.msg = read; this.context = context;
        }

        @Override
        public void run() {
//         context.networkDebug.append("Received message: "+msg+"\n");

        }
    }
}
