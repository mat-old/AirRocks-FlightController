package com.lestherreynoso.afrctest2;

import android.os.Handler;

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

    public void start() {
        updateUIHandler = new Handler();
        this.serverThread = new Thread(new ServerRun());
        serverThread.start();
    }

    class ServerRun implements  Runnable {
        @Override
        public void run() {
            byte[] data = new byte[5];
            DatagramPacket datagramPacket = new DatagramPacket(data, data.length);
            try {
                datagramSocket = new DatagramSocket(SERVER_PORT);

            } catch (IOException e) {
                e.printStackTrace();
            }

            while (!Thread.currentThread().isInterrupted()){
                try{
                    datagramSocket.receive(datagramPacket);
                    String sen = new String(datagramPacket.getData());

                    Network.mhandler.post(new updateUI("socket accepted.."+sen));
//                    CommT commRunnable = new CommT(datagramSocket);
//                    new Thread(commRunnable).start();

                }catch (IOException e){
                    e.printStackTrace();
                }
                datagramSocket.close();
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
        public updateUI(String read) {
            this.msg = read;
        }

        @Override
        public void run() {
//         networkDebug.append("Received message: "+msg+"\n");

        }
    }
}
