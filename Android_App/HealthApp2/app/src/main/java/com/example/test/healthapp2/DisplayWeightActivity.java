package com.example.test.healthapp2;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;

import com.jjoe64.graphview.GraphView;
import com.jjoe64.graphview.Viewport;
import com.jjoe64.graphview.series.DataPoint;
import com.jjoe64.graphview.series.LineGraphSeries;

import java.util.Random;

public class DisplayWeightActivity extends AppCompatActivity {

    private static final Random rand = new Random();
    private LineGraphSeries<DataPoint> series;
    private int LastX = 0;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_display_weight);

        GraphView graph = (GraphView) findViewById(R.id.graph);
        series = new LineGraphSeries<>();
        graph.addSeries(series);

        Viewport viewport = graph.getViewport();

        viewport.setYAxisBoundsManual(true);
        viewport.setMinY(0);

        viewport.setScrollable(true);

        viewport.setScalable(true);
        viewport.setScalableY(true);

    }

    @Override
    protected void onResume(){
        super.onResume();
        new Thread(new Runnable() {
            @Override
            public void run() {
                for(int i = 0;i < 20; i++)
                {
                    runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            addEntry();
                        }
                    });
                    try{
                        Thread.sleep(60);
                    }catch (InterruptedException e){
                        break;
                    }
                }
            }
        }).start();
    }

    private void addEntry(){
        series.appendData(new DataPoint(LastX++, rand.nextDouble()*10), true, 50);
    }

}
