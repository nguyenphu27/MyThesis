package com.example.test.healthapp2;

import android.app.Activity;
import android.icu.util.Calendar;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;

import com.jjoe64.graphview.GraphView;
import com.jjoe64.graphview.helper.DateAsXAxisLabelFormatter;
import com.jjoe64.graphview.series.DataPoint;
import com.jjoe64.graphview.series.LineGraphSeries;

import java.util.Date;
import java.util.Random;

import android.os.Handler;


public class DisplayHeartRateActivity extends AppCompatActivity {

    private final Handler mHandler = new Handler();
    private Runnable mTimer;
    private Runnable mTimer2;
    private LineGraphSeries<DataPoint> series;
    private LineGraphSeries<DataPoint> series2;
    private double graph2LastXValue = 5d;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_display_heart_rate);

//        Calendar calendar = Calendar.getInstance();
//        int d1 = 1492900490;
//        Date d2 = calendar.getTime();
//        calendar.add(Calendar.DATE, 2);
//        Date d3 = calendar.getTime();
//        calendar.add(Calendar.DATE, 3);
//        Date d4 = calendar.getTime();
//        calendar.add(Calendar.DATE, 4);
//        Date d5 = calendar.getTime();
//        calendar.add(Calendar.DATE, 5);
//        Date d6 = calendar.getTime();

        GraphView graph = (GraphView) findViewById(R.id.graph);
        series = new LineGraphSeries<>(generateData());
        graph.addSeries(series);

        GraphView graph2 = (GraphView) findViewById(R.id.graph);
        series2 = new LineGraphSeries<>();
        graph2.addSeries(series2);
        graph2.getViewport().setXAxisBoundsManual(true);
        graph2.getViewport().setMinX(0);
        graph2.getViewport().setMaxX(40);

//        graph.getGridLabelRenderer().setLabelFormatter(new DateAsXAxisLabelFormatter(getApplicationContext()));
//        graph.getGridLabelRenderer().setNumHorizontalLabels(3);
//
//        graph.getViewport().setMinX(d1);
//        graph.getViewport().setMaxX(d3.getTime());
//        graph.getViewport().setMaxY(20);
//        graph.getViewport().setMinY(0);
//        graph.getViewport().setYAxisBoundsManual(true);
//        graph.getViewport().setXAxisBoundsManual(true);
//        graph.getViewport().setScalable(true);
//
//        graph.getGridLabelRenderer().setHumanRounding(false);
    }

    @Override
    public void onResume(){
        super.onResume();
        mTimer = new Runnable() {
            @Override
            public void run() {
                series.resetData(generateData());
                mHandler.postDelayed(this,300);
            }
        };
        mHandler.postDelayed(mTimer,300);
        mTimer2 = new Runnable() {
            @Override
            public void run() {
                graph2LastXValue += 1d;
                series.appendData(new DataPoint(graph2LastXValue, getRandom()),true, 40);
                mHandler.postDelayed(this,200);
            }
        };
        mHandler.postDelayed(mTimer2,1000);
    }
    @Override
    public void onPause() {
        mHandler.removeCallbacks(mTimer);
        mHandler.removeCallbacks(mTimer2);
        super.onPause();
    }

    private DataPoint[] generateData(){
        int count = 30;
        DataPoint[] values = new DataPoint[count];
        for(int i=0; i<count;i++){
            double x =i;
            double f = i*5+4/2;
            double y = 5+f;
            DataPoint v = new DataPoint(x,y);
        }
        return values;
    }

    double a =2;
    Random b = new Random();
    private double getRandom() {
        return a += b.nextDouble()*0.3+0.4-0.1;
    }
}
