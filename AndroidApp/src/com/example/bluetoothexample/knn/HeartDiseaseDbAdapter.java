package com.example.bluetoothexample.knn;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;

import android.os.Environment;

public class HeartDiseaseDbAdapter {
	HeartDiseaseIndex[] heartdiseaseindex;
	String[] mline;
	int n;
	
	public void importHeartDiseaseIndex() throws IOException{
		String sdcard = Environment.getExternalStorageDirectory() + "/HeartDatabse";
		File file = new File(sdcard, "HeartDiseaseDataSet.data");
		
		StringBuilder sb = new StringBuilder();		
		BufferedReader br = new BufferedReader(new FileReader(file));
		
		n = 297;
		heartdiseaseindex = new HeartDiseaseIndex[n];
		mline = new String[n];
		String line;
		int i = 0;
		
		
		while((line = br.readLine()) != null){
			mline[i] = sb.append(line) +"";	
			
			if(mline[i] != null){
				heartdiseaseindex[i] = new HeartDiseaseIndex();
				String[] temp = mline[i].split(",");
				heartdiseaseindex[i].age = Double.parseDouble(temp[1]);
				heartdiseaseindex[i].sex = Double.parseDouble(temp[2]);                
				heartdiseaseindex[i].cp = Double.parseDouble(temp[3]);
                heartdiseaseindex[i].trestbps = Double.parseDouble(temp[4]);
                heartdiseaseindex[i].chol = Double.parseDouble(temp[5]);
                heartdiseaseindex[i].fbs = Double.parseDouble(temp[6]);
                heartdiseaseindex[i].thalach = Double.parseDouble(temp[8]);
                heartdiseaseindex[i].exang = Double.parseDouble(temp[9]);
                heartdiseaseindex[i].num = Double.parseDouble(temp[14]);
			}
			
			sb.append('\n');
			i++;
			sb.delete(0, sb.length());
		}
		br.close();
	}	
	
	//Max Value of Age Indexes
    public double maxAge() {
        double maxAge = heartdiseaseindex[0].age;
        for(int j = 0; j < n; j++){
            if(heartdiseaseindex[j].age > maxAge)
                maxAge = heartdiseaseindex[j].age;
        }
        return maxAge;
    }
    
    //Min Value of Age Indexes
    public double minAge() {
        double minAge = heartdiseaseindex[0].age;
        for(int j = 0; j < n; j++){
            if(heartdiseaseindex[j].age < minAge)
                minAge = heartdiseaseindex[j].age;
        }
        return minAge;
    }
    
    //Max Value of Trestbps Indexes
    public double maxTrestbps() {
        double maxTrestbps = heartdiseaseindex[0].trestbps;
        for(int j = 0; j < n; j++){
            if(heartdiseaseindex[j].trestbps > maxTrestbps)
                maxTrestbps = heartdiseaseindex[j].trestbps;
        }
        return maxTrestbps;
    }
    
    //Min Value of Trestbps Indexes
    public double minTrestbps() {
        double minTrestbps = heartdiseaseindex[0].trestbps;
        for(int j = 0; j < n; j++){
            if(heartdiseaseindex[j].trestbps < minTrestbps)
                minTrestbps = heartdiseaseindex[j].trestbps;
        }
        return minTrestbps;
    }
    
  //Max Value of Chol Indexes
    public double maxChol() {
        double maxChol = heartdiseaseindex[0].chol;
        for(int j = 0; j < n; j++){
            if(heartdiseaseindex[j].chol > maxChol)
                maxChol = heartdiseaseindex[j].chol;
        }
        return maxChol;
    }
    
    //Min Value of Chol Indexes
    public double minChol() {
        double minChol = heartdiseaseindex[0].chol;
        for(int j = 0; j < n; j++){
            if(heartdiseaseindex[j].chol < minChol)
                minChol = heartdiseaseindex[j].chol;
        }
        return minChol;
    }
    
  //Max Value of Thalach Indexes
    public double maxThalach() {
        double maxThalach = heartdiseaseindex[0].thalach;
        for(int j = 0; j < n; j++){
            if(heartdiseaseindex[j].thalach > maxThalach)
                maxThalach = heartdiseaseindex[j].thalach;
        }
        return maxThalach;
    }
    
    //Min Value of Thalach Indexes
    public double minThalach() {
        double minThalach = heartdiseaseindex[0].thalach;
        for(int j = 0; j < n; j++){
            if(heartdiseaseindex[j].thalach < minThalach)
                minThalach = heartdiseaseindex[j].thalach;
        }
        return minThalach;
    }
    
  //Normalize Heart Disease Continuous Indexes
    public void normalizeHeartDiseaseContinuosIndexes(double maxAge, double minAge, double maxTrestbps, double minTrestbps,
    		double maxChol, double minChol, double maxThalach, double minThalach) {
        for(int j = 0; j < n; j++) {
            heartdiseaseindex[j].nage = (heartdiseaseindex[j].age - minAge)
                    / (maxAge - minAge);
            heartdiseaseindex[j].ntrestbps = (heartdiseaseindex[j].trestbps - minTrestbps)
                    / (maxTrestbps - minTrestbps);
            heartdiseaseindex[j].nchol = (heartdiseaseindex[j].chol - minChol)
                    / (maxChol - minChol);
            heartdiseaseindex[j].nthalach = (heartdiseaseindex[j].thalach - minThalach)
                    / (maxThalach - minThalach);
        }
    }
    
    public void normalizeHeartDiseaseDiscreteIndexes(double sex, double cp, double fbs, double exang){
        for(int j = 0; j < n; j++){
            
            if(heartdiseaseindex[j].sex != sex)
                heartdiseaseindex[j].nsex = 1;
            else
                heartdiseaseindex[j].nsex = 0;
            
            if(heartdiseaseindex[j].cp != cp)
                heartdiseaseindex[j].ncp = 1;
            else
                heartdiseaseindex[j].ncp = 0;
            
            if(heartdiseaseindex[j].fbs != fbs)
                heartdiseaseindex[j].nfbs = 1;
            else
                heartdiseaseindex[j].nfbs = 0;   
            
            if(heartdiseaseindex[j].exang != exang)
                heartdiseaseindex[j].nexang = 1;
            else
                heartdiseaseindex[j].nexang = 0;
        }
    }
    
    //Compute distances between measured heart disease index and heart disease indexes in file HeartDiseaseDataset
    public void computeDistances(HeartDiseaseIndex mheartdiseaseindex) {
        for(int j = 0; j < n; j++) {
            heartdiseaseindex[j].distance = (double) Math.sqrt(Math.pow((heartdiseaseindex[j].nage - mheartdiseaseindex.nage),2)
                    + Math.pow((heartdiseaseindex[j].ntrestbps - mheartdiseaseindex.ntrestbps),2)
                    + Math.pow((heartdiseaseindex[j].nchol - mheartdiseaseindex.nchol),2)
                    + Math.pow((heartdiseaseindex[j].nthalach - mheartdiseaseindex.nthalach),2)
                    
                    + Math.pow((heartdiseaseindex[j].nsex - mheartdiseaseindex.nsex),2)
                    + Math.pow((heartdiseaseindex[j].ncp - mheartdiseaseindex.ncp),2)
                    + Math.pow((heartdiseaseindex[j].nfbs - mheartdiseaseindex.nfbs),2)
                    + Math.pow((heartdiseaseindex[j].nexang - mheartdiseaseindex.nexang),2));
        }
    }
    
    //Arrange distances between measured health index and health indexes in file HealthIndex
    public void arrangeDistances(double[] l_distance) {      
        double temp;      
        for(int i = 0; i < n; i++) {
            l_distance[i] = heartdiseaseindex[i].distance;
        }
        
        for(int i = 0; i < n - 1; i++)
            for(int j = i + 1; j < n; j++)
                if(l_distance[i] > l_distance[j]) {
                    temp = l_distance[j];
                    l_distance[j] = l_distance[i];
                    l_distance[i] = temp;
                }   
    }
    
    public int compareDistances(double x) {
        for(int j = 0; j < n; j++) {
            if(heartdiseaseindex[j].distance == x && heartdiseaseindex[j].num == 0) {
                return 0;
            }
            if(heartdiseaseindex[j].distance == x && heartdiseaseindex[j].num == 1) {
                return 1;
            }
            if(heartdiseaseindex[j].distance == x && heartdiseaseindex[j].num == 2) {
                return 2;
            }
            if(heartdiseaseindex[j].distance == x && heartdiseaseindex[j].num == 3) {
                return 3;
            }
            if(heartdiseaseindex[j].distance == x && heartdiseaseindex[j].num == 4) {
                return 4;
            }
        }
        return -1;
    }
   
}
