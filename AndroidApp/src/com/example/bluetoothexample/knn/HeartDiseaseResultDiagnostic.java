package com.example.bluetoothexample.knn;

import android.annotation.SuppressLint;
import android.annotation.TargetApi;
import android.app.ActionBar;
import android.app.Activity;
import android.content.Intent;
import android.os.Build;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.ImageView;
import android.widget.TextView;

import com.example.bluetoothexample.R;

@TargetApi(Build.VERSION_CODES.HONEYCOMB)
@SuppressLint({ "InlinedApi", "NewApi" })
public class HeartDiseaseResultDiagnostic extends Activity{

	TextView tvResult;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_knn_result);
		tvResult = (TextView) findViewById(R.id.tvResultDiagnostic);
		
		getActionBar().setDisplayOptions(ActionBar.DISPLAY_SHOW_CUSTOM);
		getActionBar().setCustomView(R.layout.actionbar);
		TextView title = (TextView) findViewById(
				getResources().getIdentifier("action_bar_title", "id", getPackageName()));
		title.setText("Kết quả chuẩn đoán");
		
		ImageView bHome = (ImageView) findViewById(R.id.imHome);
		bHome.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View arg0) {
				// TODO Auto-generated method stub
				finish();
			}
		});
		
		
		Intent i = getIntent();
		int resultCode = i.getExtras().getInt("resultcode");

		if(resultCode == 0){
			tvResult.setText("Sức khỏe của bạn bình thường!");
		}else{
			tvResult.setText("Bạn có nguy cơ mắc bệnh tim mạch!");
		}
	}

}
