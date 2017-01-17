package com.example.bluetoothexample.knn;

import android.annotation.SuppressLint;
import android.annotation.TargetApi;
import android.app.ActionBar;
import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.Build;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.AdapterView;
import android.widget.Button;
import android.widget.AdapterView.OnItemSelectedListener;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.RadioButton;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.Toast;

import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;

import org.json.JSONException;

import com.example.bluetoothexample.R;
import com.example.bluetoothexample.cloud.ResponseServer;
import com.example.bluetoothexample.customadapter.CustomSpinner;

public class HeartDiseaseActivity extends Activity implements OnItemSelectedListener {

	private TextView tvSystolic_Heart;

	private EditText mage;
	private EditText mchol;
	private EditText mheart;

	private Spinner schestpain;
	private RadioButton rexangyes;
	private RadioButton rexangno;
	private RadioButton rsexmale;;
	private RadioButton rsexfemale;
	private RadioButton rfbsYes;
	private RadioButton rfbsNo;

	private Button bDiagnostic;

	private String[] title_chestpain = new String[] { 
											"Đau thắt ngực điển hình",
											"Đau thắt ngực không điển hình", 
											"Đau không đau thắt ngực", 
											"Không có triệu chứng" };

	private int[] icon_chestpain = { R.drawable.chestpain1, 
									R.drawable.chestpain2,
									R.drawable.chestpain3, 
									R.drawable.chestpain4};

	HeartDiseaseDbAdapter heartdiseaseDb;
	HeartDiseaseIndex heartIndex;
	static double maxAge;
	static double minAge;
	static double maxTrestbps;
	static double minTrestbps;
	static double maxChol;
	static double minChol;
	static double maxThalach;
	static double minThalach;

	static double sex = 0;
	static double cp = 0;
	static double fbs = 0;
	static double exang = 0;

	private ArrayList<HashMap<String, String>> arrayListData;
	private ResponseServer response;

	private static final String OBJECT1 = "tamthu";
	private static final String OBJECT2 = "tamtruong";

	private static String systolic_temp;

	@TargetApi(Build.VERSION_CODES.HONEYCOMB)
	@SuppressLint({ "InlinedApi", "NewApi" })
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		setContentView(R.layout.pulsediagnostic_layout);

		getActionBar().setDisplayOptions(ActionBar.DISPLAY_SHOW_CUSTOM);
		getActionBar().setCustomView(R.layout.actionbar);
		TextView title = (TextView) findViewById(
				getResources().getIdentifier("action_bar_title", "id", getPackageName()));
		title.setText("Chẩn đoán bệnh tim");

		ImageView bHome = (ImageView) findViewById(R.id.imHome);
		bHome.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View arg0) {
				// TODO Auto-generated method stub
				finish();
			}
		});

		tvSystolic_Heart = (TextView) findViewById(R.id.tv_systolic_diagnostic_heart);
		mage = (EditText) findViewById(R.id.etage);
		mchol = (EditText) findViewById(R.id.etcholesterol);
		mheart = (EditText) findViewById(R.id.etheart);

		schestpain = (Spinner) findViewById(R.id.spinnerCheckPain);
		rexangyes = (RadioButton) findViewById(R.id.rbtexangYes);
		rexangno = (RadioButton) findViewById(R.id.rbtexangNo);
		rsexmale = (RadioButton) findViewById(R.id.rbtSexMale);
		rsexfemale = (RadioButton) findViewById(R.id.rbtSexFemale);
		rfbsYes = (RadioButton) findViewById(R.id.rbtfbsYes);
		rfbsNo = (RadioButton) findViewById(R.id.rbtfbsNo);
		bDiagnostic = (Button) findViewById(R.id.btnDiagnostic);

		//
		heartdiseaseDb = new HeartDiseaseDbAdapter();
		heartIndex = new HeartDiseaseIndex();
		try {
			heartdiseaseDb.importHeartDiseaseIndex(); // import database
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		schestpain.setAdapter(new CustomSpinner(getApplicationContext(), title_chestpain, icon_chestpain));
		//
		arrayListData = new ArrayList<HashMap<String, String>>();

		schestpain.setOnItemSelectedListener(this);
		bDiagnostic.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View arg0) {
				// TODO Auto-generated method stub
				disease();
			}
		});
	}

	@Override
	protected void onResume() {
		// TODO Auto-generated method stub
		super.onResume();
		if (!checkNetwork(getBaseContext())) {
			Toast.makeText(getApplicationContext(), "Please! Check your network.", Toast.LENGTH_SHORT).show();
			systolic_temp = "";
		} else {
			new Thread(new Runnable() {

				@Override
				public void run() {
					// TODO Auto-generated method stub
					try {
						response = new ResponseServer();
						String _json = response.getJson();
						response.getArrayList(_json, arrayListData, OBJECT1, OBJECT2);
					} catch (JSONException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
					systolic_temp = arrayListData.get(arrayListData.size() - 1).get(OBJECT1);
					runOnUiThread(new Runnable() {
						public void run() {
							tvSystolic_Heart.setText(systolic_temp + " mmHg");
						}
					});
				}
			}).start();
			;
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

	private void disease() {
		// TODO Auto-generated method stub
		String age = mage.getText().toString();
		String chol = mchol.getText().toString();
		String heart = mheart.getText().toString();

		if (age.isEmpty() || chol.isEmpty() || heart.isEmpty() || systolic_temp.equals("")
				|| ((!rsexfemale.isChecked()) && !(rsexmale.isChecked()))
				|| ((!rexangno.isChecked()) && (!rexangyes.isChecked()))
				|| ((!rfbsYes.isChecked()) && (!rfbsNo.isChecked()))) {
			Toast.makeText(getApplicationContext(), "Invalid! Please check again.", Toast.LENGTH_SHORT).show();
		} else {

			double nage = Double.parseDouble(age);
			double ntrestbps = Double.parseDouble(systolic_temp);
			double nchol = Double.parseDouble(chol);
			double nheart = Double.parseDouble(heart);

			getMaxValueDb();
			compareInputValue(nage, ntrestbps, nchol, nheart);

			heartdiseaseDb.normalizeHeartDiseaseContinuosIndexes(maxAge, minAge, maxTrestbps, minTrestbps, maxChol,
					minChol, maxThalach, minThalach);
			heartdiseaseDb.normalizeHeartDiseaseDiscreteIndexes(sex, cp, fbs, exang);
			heartIndex.normalizeHeartDiseaseContinuosIndexes(maxAge, minAge, maxTrestbps, minTrestbps, maxChol, minChol,
					maxThalach, minThalach, nage, ntrestbps, nchol, nheart);

			heartdiseaseDb.computeDistances(heartIndex);

			double[] distance = new double[297];
			heartdiseaseDb.arrangeDistances(distance);

			distanceObject(distance);

			int code;

			if (heartIndex.num == 0) {
				code = 0;
			} else {
				code = 1;
			}

			Intent iDiseaseResult = new Intent("com.example.bluetoothexample.knn.DISEASERESULT");
			iDiseaseResult.putExtra("resultcode", code);
			startActivity(iDiseaseResult);
		}
	}

	private void getMaxValueDb() {
		// TODO Auto-generated method stub
		maxAge = heartdiseaseDb.maxAge();
		minAge = heartdiseaseDb.minAge();
		maxTrestbps = heartdiseaseDb.maxTrestbps();
		minTrestbps = heartdiseaseDb.minTrestbps();
		maxChol = heartdiseaseDb.maxChol();
		minChol = heartdiseaseDb.minChol();
		maxThalach = heartdiseaseDb.maxThalach();
		minThalach = heartdiseaseDb.minThalach();
	}

	private void compareInputValue(double nage, double ntrestbps, double nchol, double nheart) {
		// TODO Auto-generated method stub
		if (nage > maxAge)
			maxAge = nage;
		if (nage < minAge)
			minAge = nage;
		if (ntrestbps > maxTrestbps)
			maxTrestbps = ntrestbps;
		if (ntrestbps < minTrestbps)
			minTrestbps = ntrestbps;
		if (nchol > maxChol)
			maxChol = nchol;
		if (nchol < minChol)
			minChol = nchol;
		if (nheart > maxThalach)
			maxThalach = nheart;
		if (nheart < minThalach)
			minThalach = nheart;
	}

	private void distanceObject(double[] distance) {
		// TODO Auto-generated method stub
		int k = 5;
		int[] resultCount = new int[2];
		int max_resultCount = 0;
		double[] resultDistance = new double[2];
		double min_resultDistance = 0;

		for (int i = 0; i < k; i++) {
			if (heartdiseaseDb.compareDistances(distance[i]) == 0) {
				resultCount[0]++;
				resultDistance[0] += distance[i];
			}
			if (heartdiseaseDb.compareDistances(distance[i]) == 1) {
				resultCount[1]++;
				resultDistance[1] += distance[i];
			}
			if (heartdiseaseDb.compareDistances(distance[i]) == 2) {
				resultCount[1]++;
				resultDistance[1] += distance[i];
			}
			if (heartdiseaseDb.compareDistances(distance[i]) == 3) {
				resultCount[1]++;
				resultDistance[1] += distance[i];
			}
			if (heartdiseaseDb.compareDistances(distance[i]) == 4) {
				resultCount[1]++;
				resultDistance[1] += distance[i];
			}
		}

		for (int i = 0; i < 2; i++) {
			if (resultCount[i] > max_resultCount)
				max_resultCount = resultCount[i];
		}

		int count = 0;
		for (int i = 0; i < 2; i++) {
			if (resultCount[i] == max_resultCount) {
				heartIndex.num = i;
				count++;
			}
		}

		if (count > 1) {
			for (int i = 0; i < count; i++)
				if (resultCount[i] == max_resultCount)
					min_resultDistance = resultDistance[i];
			for (int i = 0; i < count; i++)
				if ((resultCount[i] == max_resultCount) && (resultDistance[i] < min_resultDistance))
					min_resultDistance = resultDistance[i];
			for (int i = 0; i < count; i++)
				if (resultDistance[i] == min_resultDistance)
					heartIndex.num = i;
		}
	}

	public void onRadioButonSexClicked(View v) {
		boolean checked = ((RadioButton) v).isChecked();
		switch (v.getId()) {
		case R.id.rbtSexMale:
			if (checked) {
				sex = 1;
				rsexfemale.setChecked(false);
			}
			break;
		case R.id.rbtSexFemale:
			if (checked) {
				sex = 0;
				rsexmale.setChecked(false);
			}
			break;
		}
	}

	public void onRadioButonExangClicked(View v) {
		boolean checked = ((RadioButton) v).isChecked();
		switch (v.getId()) {
		case R.id.rbtexangYes:
			if (checked) {
				exang = 1;
				rexangno.setChecked(false);
			}
			break;
		case R.id.rbtexangNo:
			if (checked) {
				exang = 0;
				rexangyes.setChecked(false);
			}
			break;
		}
	}

	public void onRadioButonFbsClicked(View v) {
		boolean checked = ((RadioButton) v).isChecked();
		switch (v.getId()) {
		case R.id.rbtfbsYes:
			if (checked) {
				fbs = 1;
				rfbsNo.setChecked(false);
			}
			break;
		case R.id.rbtfbsNo:
			if (checked) {
				fbs = 0;
				rfbsYes.setChecked(false);
			}
			break;
		}
	}

	@Override
	public void onItemSelected(AdapterView<?> parent, View view, int position, long mId) {
		// TODO Auto-generated method stub
		cp = position + 1;
	}

	@Override
	public void onNothingSelected(AdapterView<?> arg0) {
		// TODO Auto-generated method stub

	}
}
