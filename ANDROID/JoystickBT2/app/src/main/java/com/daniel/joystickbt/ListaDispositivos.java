package com.daniel.joystickbt;

import android.app.ListActivity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Intent;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import java.util.Set;

/**
 * Created by Daniel on 01/12/2017.
 */

public class ListaDispositivos extends ListActivity{
    private BluetoothAdapter mbta2 = null;
    static String ENDERECO_MAC = null;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        ArrayAdapter<String> ArrayBluetooth = new ArrayAdapter<String>(this,android.R.layout.simple_list_item_1);
        mbta2 = BluetoothAdapter.getDefaultAdapter();
        Set<BluetoothDevice> dispositivosPareados = mbta2.getBondedDevices();

        if(dispositivosPareados.size() > 0){
            for(BluetoothDevice dispositivo : dispositivosPareados){
                String nomeBT = dispositivo.getName();
                String macBT = dispositivo.getAddress();
                ArrayBluetooth.add(nomeBT +"\n"+macBT);
            }
        }
        setListAdapter(ArrayBluetooth);
    }

    @Override
    protected void onListItemClick(ListView l, View v, int position, long id) {
        super.onListItemClick(l, v, position, id);
        String informacaoGeral = ((TextView) v).getText().toString();
        String enderecoMAC = informacaoGeral.substring(informacaoGeral.length() - 17);
        //Toast.makeText(getApplicationContext(),"Mac: "+enderecoMAC,Toast.LENGTH_LONG).show();
        Intent retornaMAC = new Intent();
        retornaMAC.putExtra(ENDERECO_MAC, enderecoMAC);
        setResult(RESULT_OK,retornaMAC);
        finish();
    }
}
