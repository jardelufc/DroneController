package com.daniel.joystickbt;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.os.CountDownTimer;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.app.Activity;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnTouchListener;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.RelativeLayout;
import android.widget.SeekBar;
import android.widget.TextView;
import android.widget.Toast;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.UUID;

public class Main extends Activity {
    //SeekBar seekBar;
    RelativeLayout layout_joystick;
    ImageView image_joystick, image_border;
    Button buttonBT, buttonUP, buttonDOWN, buttonHOVER;
    TextView textView1, textView2, textView3, textView4, textView5, textViewInt, textViewBin;
    int dir, dis, buf, data;
    String pw;
    boolean conexao;
    private static String MAC = null;
    private static final int solicitaAtivaBT = 1;
    private static final int solicitaConexaoBT = 2;
    ConnectedThread connectedThread;
    BluetoothAdapter mbt;
    BluetoothDevice meuDevice;
    BluetoothSocket meuSocket;
    UUID MEU_UUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
    JoystickBT js;

    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.lay);
        dir = dis = 0;
        pw = "00";
        conexao = false;
        buttonBT = (Button)findViewById(R.id.buttonBT);
        buttonUP = (Button)findViewById(R.id.buttonUp);
        buttonDOWN = (Button)findViewById(R.id.buttonDown);
        buttonHOVER = (Button)findViewById(R.id.buttonHover);
        buttonBT.setText("Conectar");
        mbt = BluetoothAdapter.getDefaultAdapter();
        meuDevice = null;
        meuSocket = null;
        textViewBin = (TextView)findViewById(R.id.textViewBin);
        textViewBin.setText("Bin: 00-000000");
        textViewInt = (TextView)findViewById(R.id.textViewInt);

        btLiga();
        botaoBT();
        botoesPwr();
        jstck();

        new CountDownTimer(3600000, 100){
            public void onTick(long millisUntilFinished){
                String temp = (pw + bin(dir, dis));
                buf = data;
                data = Integer.parseInt(temp,2);
                textViewInt.setText("Val_BT: "+Integer.toString(data));
                if(conexao){
                        if (buf != data) connectedThread.enviar((byte)data);
                    }
                pw = "00";
            }

            public void onFinish(){

            }
        }.start();
        //skbr();
    }

    public void botoesPwr(){
        buttonUP.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                textViewBin.setText("Bin: 10-000000");
                pw = "10";
                dis = dir = 0;

                /*String temp = "10000000";
                int x = Integer.parseInt(temp,2);
                textViewInt.setText("Val_BT: "+Integer.toString(x));

                if(conexao){
                    connectedThread.enviar(x);
                }*/

            }
        });

        buttonUP.setOnLongClickListener(new View.OnLongClickListener() {
            @Override
            public boolean onLongClick(View view) {
                return false;
            }
        });

        buttonDOWN.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                textViewBin.setText("Bin: 01-000000");
                pw = "01";
                dis = dir = 0;

                /*String temp = "01000000";
                int x = Integer.parseInt(temp,2);
                textViewInt.setText("Val_BT: "+Integer.toString(x));

                if(conexao){
                    connectedThread.enviar(x);
                }*/

            }
        });

        buttonDOWN.setOnLongClickListener(new View.OnLongClickListener() {
            @Override
            public boolean onLongClick(View view) {
                return false;
            }
        });

        buttonHOVER.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                textViewBin.setText("Bin: 11-000000");
                pw = "11";
                dis = dir = 0;

                /*String temp = "11000000";
                int x = Integer.parseInt(temp,2);
                textViewInt.setText("Val_BT: "+Integer.toString(x));

                if(conexao){
                    connectedThread.enviar(x);
                }*/

            }
        });

    }

    public void botaoBT(){
        buttonBT.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (conexao){
                    //desconecta
                    try{
                        meuSocket.close();
                        conexao = false;
                        buttonBT.setText("Conectar");
                        Toast.makeText(getApplicationContext(),"Bluetooth desconectado.",Toast.LENGTH_LONG).show();
                    }catch(IOException erro){
                        //conexao = true;
                        Toast.makeText(getApplicationContext(),"Ocorreu um erro: " + erro,Toast.LENGTH_LONG).show();
                    }
                } else{
                    //conecta
                    Intent abreLista = new Intent(Main.this,ListaDispositivos.class);
                    startActivityForResult(abreLista,solicitaConexaoBT);
                }
            }
        });
    }
/*
    public void skbr(){
        textViewPwr = (TextView)findViewById(R.id.textViewPwr);
        seekBar = (SeekBar)findViewById(R.id.seekBar);
        seekBar.setMax(200);
        textViewPwr.setText("PWR: " + (pwr/2));

        seekBar.setOnSeekBarChangeListener(
                new SeekBar.OnSeekBarChangeListener() {
                    @Override
                    public void onProgressChanged(SeekBar seekBar, int progress, boolean b) {
                        pwr = progress;
                        float x = pwr;
                        textViewPwr.setText("PWR: " + (x/2));
                        textViewBin.setText("Bin: " + bin(dir, dis, pwr));
                    }

                    @Override
                    public void onStartTrackingTouch(SeekBar seekBar) {

                    }

                    @Override
                    public void onStopTrackingTouch(SeekBar seekBar) {
                        float x = pwr;
                        textViewPwr.setText("PWR: " + (x/2));
                        textViewBin.setText("Bin: " + bin(dir, dis, pwr));
                    }
                }
        );
    }*/

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        switch(requestCode){
            case solicitaAtivaBT:
                if(resultCode==Activity.RESULT_OK){
                    Toast.makeText(getApplicationContext(),"O Bluetooth foi Ativado",Toast.LENGTH_LONG).show();
                }else {
                    Toast.makeText(getApplicationContext(),"O Bluetooth não foi Ativado",Toast.LENGTH_LONG).show();

                }
                break;
            case solicitaConexaoBT:
                if(resultCode == Activity.RESULT_OK){
                    MAC = data.getExtras().getString(ListaDispositivos.ENDERECO_MAC);
                    //Toast.makeText(getApplicationContext(),"MAC Final: "+MAC ,Toast.LENGTH_LONG).show();
                    meuDevice = mbt.getRemoteDevice(MAC);
                    try{
                    //  Treta de conexão!!! [RFCOMM] -------------------------------------------------------------
                        meuSocket = meuDevice.createRfcommSocketToServiceRecord(MEU_UUID);
                        meuSocket.connect();
                        conexao = true;
                        connectedThread = new ConnectedThread(meuSocket);
                        connectedThread.start();
                        buttonBT.setText("Desconectar");
                        Toast.makeText(getApplicationContext(),"Você foi conectado com: "+MAC ,Toast.LENGTH_LONG).show();

                    }catch(IOException erro){
                        conexao = false;
                        Toast.makeText(getApplicationContext(),"Ocorreu um erro: " + erro ,Toast.LENGTH_LONG).show();
                    }
                }else{
                    Toast.makeText(getApplicationContext(),"Falha ao obter Endereço",Toast.LENGTH_LONG).show();
                }
        }
    }

    public void btLiga (){
        if (mbt == null){
            Toast.makeText(getApplicationContext(),"Dispositivo não possui Bluetooth",Toast.LENGTH_LONG).show();
        }
        else if (!mbt.isEnabled()){
            Intent ativaBluetooth = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(ativaBluetooth,solicitaAtivaBT);
        }
    }

    public void btDesliga (){
        if (mbt == null){
            Toast.makeText(getApplicationContext(),"Dispositivo não possui Bluetooth",Toast.LENGTH_LONG).show();
        }
        else if (mbt.isEnabled()){
            mbt.disable();
        }
    }

     public void jstck(){
        textView1 = (TextView)findViewById(R.id.textView1);
        textView2 = (TextView)findViewById(R.id.textView2);
        textView3 = (TextView)findViewById(R.id.textView3);
        textView4 = (TextView)findViewById(R.id.textView4);
        textView5 = (TextView)findViewById(R.id.textView5);

        layout_joystick = (RelativeLayout)findViewById(R.id.layout_joystick);

        js = new JoystickBT(getApplicationContext(), layout_joystick, R.drawable.image_button);
        js.setStickSize(150, 150);
        js.setLayoutSize(500, 500);
        js.setLayoutAlpha(150);
        js.setStickAlpha(100);
        js.setOffset(90);
        js.setMinimumDistance(25);

        layout_joystick.setOnTouchListener(new OnTouchListener() {
            public boolean onTouch(View arg0, MotionEvent arg1) {
                js.drawStick(arg1);
                if(arg1.getAction() == MotionEvent.ACTION_DOWN
                        || arg1.getAction() == MotionEvent.ACTION_MOVE) {
                    pw = "00";
                    textView1.setText("X : " + String.valueOf(js.getX()));
                    textView2.setText("Y : " + String.valueOf(js.getY()));
                    textView3.setText("Angle : " + String.valueOf(js.getAngle()));
                    dis = js.getDis();
                    textView4.setText("Dis : "/* + String.valueOf(js.getDistance()) */+String.valueOf(dis));
                    textViewBin.setText("Bin: " + pw + "-" + bin(dir, dis));

                    int direction = js.get8Direction();
                    if(direction == JoystickBT.STICK_UP) {
                        dir = 0;
                        textView5.setText("Direction : ↑ "+String.valueOf(dir));
                    } else if(direction == JoystickBT.STICK_UPRIGHT) {
                        dir = 1;
                        textView5.setText("Direction : ↑→ "+String.valueOf(dir));
                    } else if(direction == JoystickBT.STICK_RIGHT) {
                        dir = 2;
                        textView5.setText("Direction : → "+String.valueOf(dir));
                    } else if(direction == JoystickBT.STICK_DOWNRIGHT) {
                        dir = 3;
                        textView5.setText("Direction : ↓→ "+String.valueOf(dir));
                    } else if(direction == JoystickBT.STICK_DOWN) {
                        dir = 4;
                        textView5.setText("Direction : ↓ "+String.valueOf(dir));
                    } else if(direction == JoystickBT.STICK_DOWNLEFT) {
                        dir = 5;
                        textView5.setText("Direction : ←↓ "+String.valueOf(dir));
                    } else if(direction == JoystickBT.STICK_LEFT) {
                        dir = 6;
                        textView5.setText("Direction : ← "+String.valueOf(dir));
                    } else if(direction == JoystickBT.STICK_UPLEFT) {
                        dir = 7;
                        textView5.setText("Direction : ←↑ "+String.valueOf(dir));
                    } else if(direction == JoystickBT.STICK_NONE) {
                        dir = 0;
                        textView5.setText("Direction : C "+String.valueOf(dir));
                    }
                } else if(arg1.getAction() == MotionEvent.ACTION_UP) {
                    dir = dis = 0;
                    pw = "00";
                    textView1.setText("X :");
                    textView2.setText("Y :");
                    textView3.setText("Angle :");
                    textView4.setText("Dis :");
                    textView5.setText("Direction :");
                    textViewBin.setText("Bin: " + pw + "-" + bin(dir, dis));
                }
                return true;
            }
        });
    }

    public String bin (int dir, int dis){
        int size;
        String bin = "";
        String dr, ds, zero;
        zero = "0";
        dr = Integer.toBinaryString(dir);
        ds = Integer.toBinaryString(dis);

        size = ds.length();
        for (int i = 0 ; (3-size)>i ; i++){
            bin = bin + zero;
        }

        bin = bin + ds;
        size = dr.length();

        for (int i = 0 ; (3-size)>i ; i++){
            bin = bin + zero;
        }

        bin = bin + dr;

        return bin;
    }

    private class ConnectedThread extends Thread {
        private final InputStream mmInStream;
        private final OutputStream mmOutStream;

        public ConnectedThread(BluetoothSocket socket) {
            InputStream tmpIn = null;
            OutputStream tmpOut = null;

            // Get the input and output streams, using temp objects because
            // member streams are final
            try {
                tmpIn = socket.getInputStream();
                tmpOut = socket.getOutputStream();
            } catch (IOException e) { }

            mmInStream = tmpIn;
            mmOutStream = tmpOut;
        }

        public void run() {
            byte[] buffer = new byte[1024];  // buffer store for the stream
            int bytes; // bytes returned from read()

            // Keep listening to the InputStream until an exception occurs
            /*while (true) {
                try {
                    // Read from the InputStream
                    bytes = mmInStream.read(buffer);
                    // Send the obtained bytes to the UI activity
                    //mHandler.obtainMessage(MESSAGE_READ, bytes, -1, buffer)
                    //        .sendToTarget();
                } catch (IOException e) {
                    break;
                }
            }*/
        }

        /* Call this from the main activity to send data to the remote device */
        public void enviar(int valor) {
            try {
                mmOutStream.write((byte)valor);
            } catch (IOException e) { }
        }

        /* Call this from the main activity to shutdown the connection */
        public void cancel() {
            try {
                meuSocket.close();
            } catch (IOException e) { }
        }
    }
}