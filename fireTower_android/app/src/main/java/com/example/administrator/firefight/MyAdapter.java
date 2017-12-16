package com.example.administrator.firefight;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.TextView;

import java.util.ArrayList;

/**
 * Created by Administrator on 2016-12-01.
 */

public class MyAdapter extends BaseAdapter {      //We use customView for distinguish data

    private Context context;
    private ArrayList<Data> arrData;    //Date and water height information
    private LayoutInflater inflater;

    public MyAdapter(Context c, ArrayList<Data> arr) {
        this.context = c;
        this.arrData = arr;
        inflater = (LayoutInflater)c.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
    }

    public int getCount() {
        return arrData.size();
    }

    public Object getItem(int position) {
        return arrData.get(position).getDate();
    }

    public long getItemId(int position) {
        return position;
    }

    public View getView(int position, View convertView, ViewGroup parent) {

        if(convertView == null){
            convertView = inflater.inflate(R.layout.activity_list, parent, false);
        }

        TextView date = (TextView)convertView.findViewById(R.id.date);
        date.setText(arrData.get(position).getDate());

        TextView water = (TextView)convertView.findViewById(R.id.water);
        water.setText(arrData.get(position).getWater());

        return convertView;
    }
}
