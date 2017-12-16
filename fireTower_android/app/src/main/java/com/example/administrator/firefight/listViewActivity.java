package com.example.administrator.firefight;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.Toast;

import java.util.ArrayList;

/**
 * Created by Administrator on 2016-11-27.
 */

public class listViewActivity extends AppCompatActivity {
    @Override
    public void onCreate(Bundle savaInstanceState) {
        super.onCreate(savaInstanceState);
        setContentView(R.layout.activity_listview);
        System.out.println("ListView start");

        Intent intent=getIntent();
        ArrayList<Data> al = (ArrayList<Data>)intent.getSerializableExtra("InfoList"); //
        ListView listView=(ListView)findViewById(R.id.ListView1);
        MyAdapter myAdapter=new MyAdapter(this,al);
        listView.setAdapter(myAdapter);   //ListView
        listView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> adapterView, View view, int position, long id) {
                Toast.makeText(getApplicationContext(),
                        adapterView.getItemAtPosition(position).toString(),Toast.LENGTH_SHORT).show();
            }
        });
    }
}
