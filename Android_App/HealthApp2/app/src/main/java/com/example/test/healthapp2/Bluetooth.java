package com.example.test.healthapp2;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;
import android.widget.Toast;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.Set;
import java.util.UUID;

public class Bluetooth extends AppCompatActivity {

    BluetoothSocket mmSocket;
    BluetoothDevice mmDevice = null;

    boolean workDone = false;
    UUID uuid = UUID.fromString("00001101-0000-1000-8000-00805f9b34fb"); //Standard SerialPortService ID
    private BluetoothAdapter BA;
    private Set<BluetoothDevice>pairedDevices;
    ListView lv;

    public void sendBtMsg(String msg2send){
        try {
//            mmSocket = mmDevice.createRfcommSocketToServiceRecord(uuid);
            if (!mmSocket.isConnected()){
                mmSocket.connect();
            }
            String msg = msg2send;
            OutputStream mmOutputStream = mmSocket.getOutputStream();
            mmOutputStream.write(msg.getBytes());
        } catch (IOException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_bluetooth);

        BA = BluetoothAdapter.getDefaultAdapter();
        lv = (ListView)findViewById(R.id.listView);

        final Button connectButton = (Button) findViewById(R.id.button);
        final Button disconnectButton = (Button) findViewById(R.id.disconnect);

        pairedDevices = BA.getBondedDevices();
        final ArrayList List = new ArrayList();
        final ArrayAdapter<String> adapter = new ArrayAdapter<>(this, android.R.layout.simple_list_item_1,List);

        final BluetoothAdapter mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();

        final class workerThread implements Runnable {
            private String btMsg;
            public workerThread(String msg) {
                btMsg = msg;
            }
            public void run()
            {
                sendBtMsg(btMsg);
                try {
                    if (workDone == true){
                        mmSocket.close();
                    }
                }
                catch (IOException e) {
                    // TODO Auto-generated catch block
                    e.printStackTrace();
                }
            }
        }

        //start connect button handler
        connectButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                (new Thread(new workerThread("Connected and sent string data"))).start();
            }
        });
        //end connect button handler

        //start disconnect button handler
        disconnectButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                (new Thread(new workerThread("disconnected"))).start();
            }
        });
        //end disconnect button handler

        if(!mBluetoothAdapter.isEnabled())
        {
            Intent enableBluetooth = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableBluetooth, 0);
        }
        pairedDevices = mBluetoothAdapter.getBondedDevices();

        //show paired devices on list view
        for(BluetoothDevice device : pairedDevices) List.add(device.getName());
        lv.setAdapter(adapter);

        //listview on item selected listener
        lv.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                for(BluetoothDevice device1 : pairedDevices){
                    String s1 = adapter.getItem(position).toString();
                    if(device1.getName().equals(s1)){
                        mmDevice = device1;
                        try {
                            mmSocket = mmDevice.createRfcommSocketToServiceRecord(uuid);
                            if (!mmSocket.isConnected()){
                                mmSocket.connect();
                            }
                        } catch (IOException e) {
                            // TODO Auto-generated catch block
                            e.printStackTrace();
                        }
                        Toast.makeText(Bluetooth.this, adapter.getItem(position).toString(), Toast.LENGTH_SHORT).show();
                    }
                }
            }
        });
    }
}
