package com.example.test.healthapp2;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.EditText;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }

    public void heartRate(View view){
        Intent intent = new Intent(this, DisplayHeartRateActivity.class);
        startActivity(intent);
    }

    public void SPO2(View view){
        Intent intent = new Intent(this, DisplaySPO2Activity.class);
        startActivity(intent);
    }

    public void height(View view){
        Intent intent = new Intent(this, DisplayHeightActivity.class);
        startActivity(intent);
    }

    public void weight(View view){
        Intent intent = new Intent(this, DisplayWeightActivity.class);
        startActivity(intent);
    }

    public void temperature(View view){
        Intent intent = new Intent(this, DisplayTemperatureActivity.class);
        startActivity(intent);
    }

    public void bloodpressure(View view){
        Intent intent = new Intent(this, DisplayBloodPressureActivity.class);
        startActivity(intent);
    }

    public void EWS(View view){
        Intent intent = new Intent(this, EWSActivity.class);
        startActivity(intent);
    }
}

