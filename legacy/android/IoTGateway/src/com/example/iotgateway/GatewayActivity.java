package com.example.iotgateway;

import java.util.ArrayList;

import com.hisilicon.proxyservice.LogViewListener;
import com.hisilicon.proxyservice.ProxyService;

import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;
import android.widget.Toast;


public class GatewayActivity extends Activity implements View.OnClickListener, LogViewListener {

	private int max_count_log = 20;
	private Button start_button;
	private Button stop_button;
	private ListView log_list;
	private ArrayAdapter<String> log_adapter;
	private ArrayList<String> log_array;
	
	private ProxyService proxy_service;
	private static final String TAG="Gateway";
	
	private Handler log_hander = new Handler(){
        @Override
        public void handleMessage(Message msg) {
            switch(msg.what){
            case 0:
            	log_adapter.notifyDataSetChanged();
            	log_list.setAdapter(log_adapter); 
                break;
            default:
                break;
            }
        }
    };
    
	private Thread log_thread = new Thread(){ 
        @Override 
        public void run() {              
        	log_hander.sendEmptyMessage(0);
        }
 };
 
		
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_gateway);
				
		start_button = (Button) findViewById(R.id.button_start_gateway);
		stop_button = (Button) findViewById(R.id.button_stop_gateway);
		log_list = (ListView) findViewById(R.id.log_list_view);
		
		start_button.setOnClickListener(this);
		stop_button.setOnClickListener(this);
		
		log_array = new ArrayList<String>();
		log_array.add("Log:");
		log_adapter = new ArrayAdapter<String>(this,
                android.R.layout.simple_list_item_1, log_array);
		
		log_list.setAdapter(log_adapter);
		
		proxy_service = new ProxyService();
		proxy_service.setListViewListener(this);
		
		/*
		 * load business shared library
		 * and initialize 
		 */
		proxy_service.load();
		proxy_service.init();
		
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.gateway, menu);
		return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		// Handle action bar item clicks here. The action bar will
		// automatically handle clicks on the Home/Up button, so long
		// as you specify a parent activity in AndroidManifest.xml.
		int id = item.getItemId();
		if (id == R.id.action_settings) {
			return true;
		}
		return super.onOptionsItemSelected(item);
	}

	@Override
	public void onClick(View arg0) {
        switch (arg0.getId()) {  
        case R.id.button_start_gateway:
        {
            Log.d(TAG, "Start IoT Gateway\n"); 
            Toast.makeText(this, "Start IoT Gateway",Toast.LENGTH_SHORT ).show();
            
            if (proxy_service.startService() >= 0 )
            {
            	start_button.setEnabled(false);
            	stop_button.setEnabled(true);
            	
            }
            
            break;
        }
        case R.id.button_stop_gateway:  
        {
            Log.d(TAG, "Stop IoT Gateway\n"); 
            Toast.makeText(this, "Stop IoT Gateway",Toast.LENGTH_SHORT ).show();
            
            if (proxy_service.stopService() >= 0 )
            {
            	start_button.setEnabled(true);
            	stop_button.setEnabled(false);
            }

            break;  
        }
        default:  
            break;  
        }  		
		
	}

	@Override
	public void printOutput(String message) {

		if (log_array.size() > max_count_log) {
			log_array.remove(1);
		}
		
		log_array.add(message);
		log_thread.run();
	}
}
