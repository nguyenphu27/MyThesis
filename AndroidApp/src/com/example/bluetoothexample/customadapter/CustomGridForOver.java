package com.example.bluetoothexample.customadapter;

import android.content.Context;
import android.graphics.Color;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.TextView;
import com.example.bluetoothexample.R;

public class CustomGridForOver extends BaseAdapter{

	private Context mContext;
	private final String[] title;
	private final String[] value;
	private final String[] unit;
	private final int[] iconId;
	
	public CustomGridForOver(Context c, String[] title, String[] value, String[] unit, int[] iconId) {
		// TODO Auto-generated constructor stub
		mContext = c;
		this.value = value;
		this.unit = unit;
		this.iconId = iconId;
		this.title = title;
	}
	
	@Override
	public int getCount() {
		// TODO Auto-generated method stub
		return title.length;
	}

	@Override
	public Object getItem(int arg0) {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public long getItemId(int arg0) {
		// TODO Auto-generated method stub
		return 0;
	}

	@Override
	public View getView(int position, View view, ViewGroup parent) {
		// TODO Auto-generated method stub
		View grid;
		LayoutInflater inflater = (LayoutInflater) mContext
				.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
		if(view == null){
				grid = new View(mContext);

				grid = inflater.inflate(R.layout.single_grid_for_all_measure, null);
				TextView etitle = (TextView) grid.findViewById(R.id.tv_title_all_measure);
				TextView evalue = (TextView) grid.findViewById(R.id.tv_value_all_measure);
				TextView eunit = (TextView) grid.findViewById(R.id.tv_unit_all_measure);
				ImageView eIcon = (ImageView) grid.findViewById(R.id.imv_all_measure);
				
				etitle.setText(title[position]);
				evalue.setText(value[position]);
				eunit.setText(unit[position]);
				eIcon.setImageResource(iconId[position]);
				
				if(position == 0){
					etitle.setTextColor(Color.RED);
					evalue.setTextColor(Color.RED);
					eunit.setTextColor(Color.RED);
				}
				
				if(position == 1){
					etitle.setTextColor(new Color().parseColor("#FF9100"));
					evalue.setTextColor(new Color().parseColor("#FF9100"));
					eunit.setTextColor(new Color().parseColor("#FF9100"));
				}
				
				if(position == 2){
					etitle.setTextColor(new Color().parseColor("#0B7803"));
					evalue.setTextColor(new Color().parseColor("#0B7803"));
					eunit.setTextColor(new Color().parseColor("#0B7803"));
					//evalue.setTextSize(35);
				}
				
				if(position == 3){
					etitle.setTextColor(new Color().parseColor("#15B7BD"));
					evalue.setTextColor(new Color().parseColor("#15B7BD"));
					evalue.setTextScaleX(0.7f);
					eunit.setTextColor(new Color().parseColor("#15B7BD"));
				}
				
				if(position == 4){
					etitle.setTextColor(new Color().parseColor("#FFC107"));
					evalue.setTextColor(new Color().parseColor("#FFC107"));
					eunit.setTextColor(new Color().parseColor("#FFC107"));
					//evalue.setTextSize(40);
				}
				
				if(position == 5){
					etitle.setTextColor(new Color().parseColor("#7CB342"));
					evalue.setTextColor(new Color().parseColor("#7CB342"));
					eunit.setTextColor(new Color().parseColor("#7CB342"));
					//evalue.setTextSize(40);
				}
				
	 	}else{
	 		grid = (View) view;
	 	}
		return grid;
	}

}
