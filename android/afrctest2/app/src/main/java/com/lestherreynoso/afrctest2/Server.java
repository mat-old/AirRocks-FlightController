package com.lestherreynoso.afrctest2;

import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;


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
    String sendMsg = "";
    MessageHandler messageHandler = new MessageHandler();
    public static final int SERVER_PORT = 5000;
    InetAddress ipAddress;
    Boolean serverStarted = false;


    public void start(String ipadstring) {
        updateUIHandler = new Handler();
        this.receiveThread = new Thread(new ReceiveRunnable());
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
            MainActivity.updateUI(R.id.debugTextView, "Failed to connect to " + ipadstring, "TextView", "append");
            e.printStackTrace();
        }
        serverStarted = true;
    }

    public void stop(){
        receiveThread.interrupt();
        serverStarted = false;
    }

    public void send(String message) {

        sendMsg = message;
        if (serverStarted){
            new SendMessage().execute(sendMsg);
        }else{
            MainActivity.debugTV.append("Server is not started.\n Unable to send: "+message+"\n");
        }

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
                try{
                    receivedPacket.setData(new byte[1024]);
                    recvDatagramSocket.receive(receivedPacket);

                    String recievedStringMessage = new String(receivedPacket.getData());
                    recievedStringMessage = recievedStringMessage.trim();
                    messageHandler.read(recievedStringMessage);

//                    MainActivity.updateUI(R.id.debugTextView, "Recieved: "+ recievedStringMessage, "TextView", "append");
                    MainActivity.debugTV.append("Recieved: "+ recievedStringMessage);

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
                        String sentStringMessage = params[0];
                        sentStringMessage = sentStringMessage.trim();

//                        MainActivity.updateUI(R.id.debugTV, "Sent: " + sentStringMessage, "TextView", "append");
                        MainActivity.debugTV.append("Sent: " + sentStringMessage);

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
}
