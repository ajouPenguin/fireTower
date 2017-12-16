package com.example.administrator.firefight;

import android.app.Dialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.support.v7.app.AlertDialog;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

public class LoginActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login);

        Button btn=(Button)findViewById(R.id.btn1);

        btn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                System.out.println("로그인이 시작 됬음");   //If the ID and password are the same login[i][0],login[i][1], go to InfoActivity
                String[][] login = new String[4][2];  //Administrator's ID and password information
                login[0][0] = "한강희";
                login[0][1] = "1234";
                login[1][0] = "조현";
                login[1][1] = "1234";
                login[2][0] = "최재웅";
                login[2][1] = "1234";
                login[3][0] = "김성훈";
                login[3][1] = "1234";
                EditText edit1 = (EditText) findViewById(R.id.edt1);    //ID
                EditText edit2 = (EditText) findViewById(R.id.edt2);    //Password
                    for (int i = 0; i < 4; i++) {
                        if (login[i][0].equals(edit1.getText().toString()) && login[i][1].equals(edit2.getText().toString())) {
                            Intent intent = new Intent(getApplicationContext(), InfoActivity.class);
                            startActivity(intent);
                            break;
                        }

                    }
            }
        });
    }
}

