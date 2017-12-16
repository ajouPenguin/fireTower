package com.example.administrator.firefight;

import java.io.Serializable;

/**
 * Created by Administrator on 2016-12-01.
 */


public class Data implements Serializable{   //Data and water height information

    private String date;
    private String water;

    public Data(String date, String water){
        this.date = date;
        this.water = water;
    }

    public String getDate() {return date;}

    public String getWater() {
        return water;
    }

}

