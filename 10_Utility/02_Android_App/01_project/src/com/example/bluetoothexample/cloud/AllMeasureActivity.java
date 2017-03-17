package com.example.bluetoothexample.cloud;

import java.util.ArrayList;
import java.util.HashMap;

import org.json.JSONException;
import com.example.bluetoothexample.R;
import com.example.bluetoothexample.customadapter.CustomGridForOver;

import android.app.ActionBar;
import android.app.Activity;
import android.content.Context;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.GridView;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

public class AllMeasureActivity extends Activity{

	private GridView grid;
	private String[] mtitle = {"Nhịp tim","Nhiệt độ cơ thể", "SpO2", "Huyết áp","Môi trường","Chiều cao"};
	private String[] value = new String[9];
	private String[] unit = {"bpm","\u00b0C","%","mmHg","\u00b0C","cm"};
	private int[] icon = {	  R.drawable.cardiogram,
							  R.drawable.thermometer,
							  R.drawable.oxygen_symbol,			
							  R.drawable.blood_pressure,
							  R.drawable.cloudy,
							  R.drawable.body_height
							};
	
	private ArrayList<HashMap<String, String>> arrayListData;
	private ResponseServer response;
	
	private static final String OBJECT1 = "nhiptim";
	private static final String OBJECT2 = "nhietdo";
	private static final String OBJECT3 = "spo2";
	private static final String OBJECT4 = "tamthu";
	private static final String OBJECT5 = "tamtruong";
	private static final String OBJECT6 = "moitruong";
	private static final String OBJECT7 = "chieucao";
	private static final String OBJECT8 = "ngay";
	
	private TextView tvdate;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_cloud_all_measure);
		
		getActionBar().setDisplayOptions(ActionBar.DISPLAY_SHOW_CUSTOM);
		getActionBar().setCustomView(R.layout.actionbar);
		TextView title = (TextView) findViewById(
				getResources().getIdentifier("action_bar_title", "id", getPackageName()));
		title.setText("Chỉ số tổng quát");
		
		ImageView bHome = (ImageView) findViewById(R.id.imHome);
		bHome.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View arg0) {
				// TODO Auto-generated method stub
				finish();
			}
		});
		
		grid = (GridView) findViewById(R.id.grid_all_measure);
		arrayListData = new ArrayList<HashMap<String, String>>();
		tvdate = (TextView) findViewById(R.id.tv_date_all_measure);
	}

	@Override
	protected void onResume() {
		// TODO Auto-generated method stub
		super.onResume();
		if(!checkNetwork(getBaseContext())){
			Toast.makeText(getApplicationContext(), "Please! Check your network.", Toast.LENGTH_LONG).show();
		}else{
			new Thread(new Runnable() {
				
				@Override
				public void run() {
					// TODO Auto-generated method stub
					try {
						response = new ResponseServer();
						String _json = response.getJson();
						response.getArrayList(_json, arrayListData, 
								OBJECT1, OBJECT2, OBJECT3, OBJECT4, OBJECT5, OBJECT6, OBJECT7,OBJECT8);
					} catch (JSONException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
					
					value[0] = arrayListData.get(arrayListData.size() -1).get(OBJECT1);
					value[1] = arrayListData.get(arrayListData.size() -1).get(OBJECT2);
					value[2] = arrayListData.get(arrayListData.size() -1).get(OBJECT3);
					value[3] = arrayListData.get(arrayListData.size() -1).get(OBJECT4) + "/"+
							   arrayListData.get(arrayListData.size() -1).get(OBJECT5); 
					value[4] = arrayListData.get(arrayListData.size() -1).get(OBJECT6);
					value[5] = arrayListData.get(arrayListData.size() -1).get(OBJECT7);
					
					
					runOnUiThread(new Runnable() {
						
						@Override
						public void run() {
							// TODO Auto-generated method stub
							grid.setAdapter(new CustomGridForOver(getApplicationContext(), mtitle, value, unit, icon));
							tvdate.setText("Ngày đo: " + arrayListData.get(arrayListData.size() -1).get(OBJECT8));
						}
					});
				}
			}).start();
		}
	}
	
	private boolean checkNetwork(Context context) {
		ConnectivityManager cm = (ConnectivityManager) context.getSystemService(Context.CONNECTIVITY_SERVICE);
		NetworkInfo netInfo = cm.getActiveNetworkInfo();
		if (netInfo != null && netInfo.isConnectedOrConnecting()) {
			return true;
		}
		return false;
	}

}
