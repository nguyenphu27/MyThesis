package com.example.bluetoothexample;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;

public class FlashIntroActivity extends Activity{
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		setContentView(R.layout.login_screen);
		
		Thread thread = new Thread(){

			@Override
			public void run() {
				// TODO Auto-generated method stub
				super.run();
				try {
					Thread.sleep(2000);
				} catch (InterruptedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}finally{
					Intent i = new Intent(getApplicationContext(), MainApp.class);
					startActivity(i);
				}
			}
			
		};
		thread.start();
	}

	@Override
	protected void onRestart() {
		// TODO Auto-generated method stub
		super.onRestart();
		finish();
	}
	
}
