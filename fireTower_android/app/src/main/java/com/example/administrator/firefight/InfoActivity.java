package com.example.administrator.firefight;



import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import java.net.URL;
import java.util.*;
import java.io.*;
import java.net.MalformedURLException;
import java.net.HttpURLConnection;
import java.net.ProtocolException;
import android.os.Handler;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;


/**
 * Created by Administrator on 2016-11-23.
 */

public class InfoActivity extends AppCompatActivity {
    ArrayList<Data> list=new ArrayList<Data>();    //Date and water Height information
    private URL Url;     //Communicating with the Apache server
    private String strUrl, response;    //URL information,
    MyThread myThread = new MyThread();   //Communicate with the server, Main thread is so busy so we create sub threads
    parseThread parsing = new parseThread();  //We received the Json format data, so need parsing.

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_info);

        findViewById(R.id.btn2).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                myThread.start();  //When the button is pressed, start myTread for communicates with the server.
            }
        });

    }

    private class MyThread extends Thread {
        @Override
        public void run() {
            strUrl = "http://52.78.53.34:9000/*"; //URL's wanted explore
            try {
                Url = new URL(strUrl); // URL화 한다.
                HttpURLConnection conn = (HttpURLConnection) Url.openConnection(); // URL connection
                conn.setDoInput(true); // Read mode
                conn.setUseCaches(false); // Whether or not to receive casching data
                InputStream is = conn.getInputStream(); //inputstream open
                StringBuilder builder = new StringBuilder(); //Object for storing strings
                BufferedReader reader = new BufferedReader(new InputStreamReader(is, "UTF-8")); //String set
                String line = "";


                while ((line = reader.readLine()) != null) {
                    builder.append(line);
                }
                response = builder.toString();  //Information received from the server
                myThread.interrupt();  //Since we have received information from the server, we exit thread
                parsing.start();        //Parsing the information

            } catch (MalformedURLException | ProtocolException exception) {
                exception.printStackTrace();
            } catch (IOException io) {
                io.printStackTrace();
            }
        }
    }

    private class parseThread extends Thread {
        @Override
        public void run() {
            try {
                JSONArray jarray = new JSONArray(response);    //We received data  json format.
                for(int i=0; i<jarray.length(); i++){
                        JSONObject jObject=jarray.getJSONObject(i);
                        String id=jObject.getString("id");
                        String date=jObject.getString("date");   //date information
                        String water=jObject.getString("water");  //water height information
                        Data data=new Data(date,water);
                        list.add(i,data);   //list is information to display in the ListView
                }
                Intent intent = new Intent(getApplication(), listViewActivity.class);
                intent.putExtra("InfoList", list);  //Send list to listViewActivity
                startActivity(intent);
                parseThread.interrupted(); //parseThread exit.

            }

            catch (JSONException e) {
                e.printStackTrace();
            }


        }
    }
}
