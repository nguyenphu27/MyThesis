package com.example.test.healthapp2;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Spinner;

public class EWSActivity extends AppCompatActivity implements AdapterView.OnItemSelectedListener{

    private static int mconsciousness;
    private static int mrespiration;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_ews);

        Spinner spinner = (Spinner) findViewById(R.id.spinner2);

        ArrayAdapter<CharSequence> adapter = ArrayAdapter.createFromResource(this, R.array.consciousness_level, android.R.layout.simple_spinner_item);
        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        spinner.setAdapter(adapter);
    }

    @Override
    public void onItemSelected(AdapterView<?> parent, View view, int pos, long id){
        String item = parent.getItemAtPosition(pos).toString();

    }

    public void onNothingSelected(AdapterView<?> parent){

    }
}
