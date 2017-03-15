package com.example.bluetoothexample;

import com.example.bluetoothexample.customadapter.CustomGrid;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.DialogInterface.OnClickListener;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.view.View;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.ListView;
import android.widget.Toast;

public class Setting extends Activity {

	ListView listSetting;
	String[] title = new String[] { "Sms", "Bluetooth", "Xóa dữ liệu","Khuyến nghị", "Liên hệ"};
	int[] icon = {R.drawable.ic_smsplane,
				  R.drawable.ic_bluetooth_setting,
				  R.drawable.ic_clear_setting,
				  R.drawable.ic_help_setting,
				  R.drawable.ic_about_setting};
	
	private DatabaseAdapter mDbHelper;
	boolean doubleBackToExitPressedOnce = false;
	
	@Override
	public void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		setContentView(R.layout.setting_layout);
		listSetting = (ListView) findViewById(R.id.listSetting);
		
		mDbHelper = new DatabaseAdapter(this);
		mDbHelper.open();	
		
		listSetting.setAdapter(new CustomGrid(getApplicationContext(), title, icon, 1));

		listSetting.setOnItemClickListener(new OnItemClickListener() {

			@Override
			public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
				// TODO Auto-generated method stub
				switch (position) {
				case 0:
					Intent isms = new Intent(getApplicationContext(), SmsSetting.class);
					startActivity(isms);
					break;
				case 1:
					Intent ibluetooth = new Intent(getApplicationContext(), BluetoothSetting.class);
					startActivity(ibluetooth);
					break;
				case 2:						
					new AlertDialog.Builder(getParent())
						.setTitle("Delete Data")
						.setMessage("Are you sure?")
						.setPositiveButton("Yes", new OnClickListener() {
							
							@Override
							public void onClick(DialogInterface arg0, int arg1) {
								// TODO Auto-generated method stub
								mDbHelper.deleteAllData();
								Toast.makeText(getApplicationContext(), "Delete data success!", Toast.LENGTH_SHORT).show();
							}
						})
						.setNegativeButton("Cancle", new OnClickListener() {
							
							@Override
							public void onClick(DialogInterface dialog, int which) {
								// TODO Auto-generated method stub
								dialog.dismiss();
							}
						}).show();
					
					break;
				case 3:
					Intent irecomend = new Intent(getApplicationContext(), RecomendActivity.class);
					startActivity(irecomend);
					break;
				case 4:
					Intent iabout = new Intent(getApplicationContext(), AboutActivity.class);
					startActivity(iabout);
					break;
				}
			}
		});

	}

/*	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		if (android.os.Build.VERSION.SDK_INT < android.os.Build.VERSION_CODES.DONUT && keyCode == KeyEvent.KEYCODE_BACK
				&& event.getRepeatCount() == 0) {
			onBackPressed();
		}
		return super.onKeyDown(keyCode, event);
	}

	public void onBackPressed() {
		finish();
	}*/
	@Override
	public void onBackPressed() {
	    if (doubleBackToExitPressedOnce) {
	        super.onBackPressed();
	        return;
	    }
	    this.doubleBackToExitPressedOnce = true;
	    Toast.makeText(this, "Nhấn lại lần nữa để thoát!", Toast.LENGTH_SHORT).show();

	    new Handler().postDelayed(new Runnable() {

	        @Override
	        public void run() {
	            doubleBackToExitPressedOnce=false;                       
	        }
	    }, 2000);
	}
}
