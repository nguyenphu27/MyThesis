package com.example.bluetoothexample.knn;

public class HeartDiseaseIndex {
	double age;
	double sex;
	double cp;
	double trestbps; //systolic
	double chol;
	double fbs;
	double thalach; //heart
	double exang; //gym
	double num;
	
	double nage;
	double nsex;
	double ncp;
	double ntrestbps;
	double nchol;
	double nfbs;
	double nthalach;
	double nexang;
	
	double distance;
	double predict;
		
    //Normalize Heart Disease Continuous Indexes
    public void normalizeHeartDiseaseContinuosIndexes(double maxAge, double minAge, double maxTrestbps, double minTrestbps,
    		double maxChol, double minChol, double maxThalach, double minThalach, 
    		double mage, double mtrestbps, double mchol, double mthalach) {
        nage = (mage - minAge)
                    / (maxAge - minAge);
        ntrestbps = (mtrestbps - minTrestbps)
                    / (maxTrestbps - minTrestbps);
        nchol = (mchol - minChol)
                / (maxChol - minChol);
        nthalach = (mthalach - minThalach)
                    / (maxThalach - minThalach);
    }
}
