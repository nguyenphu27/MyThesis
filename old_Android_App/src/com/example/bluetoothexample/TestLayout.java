package com.example.bluetoothexample;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;

import android.app.Activity;
import android.os.Bundle;
import android.os.Environment;
import android.widget.TextView;

public class TestLayout extends Activity{
	
	TextView text1;
	String[] mline;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		setContentView(R.layout.test_layout);
		text1 = (TextView)findViewById(R.id.texttest);
		
		String sdcard = Environment.getExternalStorageDirectory() + "/HeartDatabse";
		File file = new File(sdcard, "HeartDiseaseDataSet.data");
		
		StringBuilder text = new StringBuilder();
		int count = 0;
		try {
			BufferedReader br = new BufferedReader(new FileReader(file));
			String line;
			mline = new String[297];
			int i=0;
			while((line = br.readLine()) != null){
				//count++;
				mline[i] = text.append(line)+"";
				text.append('\n');
				i++;
				text.delete(0, text.length());
			}
			
			br.close();
			
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		text1.setText(mline[296]+"");
	}
}
