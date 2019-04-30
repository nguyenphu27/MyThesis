package com.example.bluetoothexample;

import android.annotation.SuppressLint;
import android.annotation.TargetApi;
import android.app.ActionBar;
import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.database.Cursor;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.provider.ContactsContract.CommonDataKinds.Phone;
import android.telephony.SmsManager;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

@SuppressLint("NewApi")
@TargetApi(Build.VERSION_CODES.HONEYCOMB)
public class SmsSetting extends Activity {

	EditText etphone;
	TextView tvphone;

	Button btnadd;
	Button btnclear;
	Button btnContact;

	private static final int REQUEST_CODE = 1;
	SharedPreferences shareprf;
	String number;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_sms_setting);

		etphone = (EditText) findViewById(R.id.etPhone);
		tvphone = (TextView) findViewById(R.id.tvPhone);

		btnadd = (Button) findViewById(R.id.btnAdd);
		btnclear = (Button) findViewById(R.id.btnClear);
		btnContact = (Button) findViewById(R.id.bContact);

		getActionBar().setDisplayOptions(ActionBar.DISPLAY_SHOW_CUSTOM);
		getActionBar().setCustomView(R.layout.actionbar);
		TextView title = (TextView) findViewById(
				getResources().getIdentifier("action_bar_title", "id", getPackageName()));
		title.setText("Cài đặt SMS");

		ImageView bHome = (ImageView) findViewById(R.id.imHome);
		bHome.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View arg0) {
				// TODO Auto-generated method stub
				finish();
			}
		});

		shareprf = getSharedPreferences("listphone", Context.MODE_PRIVATE);

		btnadd.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View arg0) {
				// TODO Auto-generated method stub
				SharedPreferences.Editor editor = shareprf.edit();
				editor.putString("phonenumber", number);
				editor.commit();
				// sendSmS();
				Toast.makeText(getApplicationContext(), "Thêm thành công!!", Toast.LENGTH_SHORT).show();
				
				readshareprf();
			}

		});

		btnclear.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				clearPhone();
				Toast.makeText(getApplicationContext(), "Clear success!", Toast.LENGTH_SHORT).show();
				
				readshareprf();
			}

		});

		btnContact.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View arg0) {
				// TODO Auto-generated method stub
				getContact();
			}

		});
	}
	
	private void readshareprf() {
		// TODO Auto-generated method stub
		SharedPreferences prefs = getSharedPreferences("listphone", Context.MODE_PRIVATE);
		String _number = prefs.getString("phonenumber", null);
		tvphone.setText(_number);
	}

	private void clearPhone() {
		// TODO Auto-generated method stub
		SharedPreferences.Editor editor = shareprf.edit();
		editor.clear();
		editor.commit();
		Toast.makeText(getApplicationContext(), "Đã xóa!", Toast.LENGTH_SHORT).show();
	}

	private void getContact() {
		// TODO Auto-generated method stub
		Uri uri = Uri.parse("content://contacts");
		Intent intent = new Intent(Intent.ACTION_PICK, uri);
		intent.setType(Phone.CONTENT_TYPE);
		startActivityForResult(intent, REQUEST_CODE);
	}

	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent i) {
		// TODO Auto-generated method stub
		super.onActivityResult(requestCode, resultCode, i);

		if (requestCode == REQUEST_CODE) {
			if (resultCode == RESULT_OK) {
				Uri uri = i.getData();
				String[] projection = { Phone.NUMBER, Phone.DISPLAY_NAME };

				Cursor cursor = getContentResolver().query(uri, projection, null, null, null);
				cursor.moveToFirst();

				int numberColumnIndex = cursor.getColumnIndex(Phone.NUMBER);
				number = cursor.getString(numberColumnIndex);
				etphone.setText(number);
			}
		}
	}

	@Override
	protected void onResume() {
		// TODO Auto-generated method stub
		super.onResume();
		SharedPreferences prefs = getSharedPreferences("listphone", Context.MODE_PRIVATE);
		String _number = prefs.getString("phonenumber", null);
		if (_number != null) {
			tvphone.setText(_number);
		} else {
			tvphone.setText("null");
		}
	}
}
