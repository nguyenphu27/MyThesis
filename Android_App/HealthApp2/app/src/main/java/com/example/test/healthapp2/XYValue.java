package com.example.test.healthapp2;

/**
 * Created by chung on 4/29/2017.
 */

public class XYValue{
    private double x;
    private double y;

    public XYValue(double x, double y) {
        this.x = x;
        this.y = y;
    }

    public double getX(){
        return x;
    }

    public double getY(){
        return y;
    }

    public void setX(double x){
        this.x = x;
    }

    public void setY(double y){
        this.y = y;
    }
}
