#include<Servo.h>


//We use three servo motors.
Servo servo1;
Servo servo2;   //Servo2 on servo1
Servo servo3;   //Barrel

//The servo motor must be connected to the pwm.
int servoPin1=3;
int servoPin2=5;
int servoPin3=6;


int firePin=4; //Flame detection sensor.
int waterPin=1;//Water level detection sensor.
int coPin=8;//Co detection sensor,
int IN1=10; // IN1 and IN2 is motor driver of water pump,
int IN2=11;//

int fireVal = 0;
int waterVal = 0,coVal=0, mode=0;  //The values received from each sensor are stored.

int angle; //Servo motor angle
int flag=0;  //Determine whether the mode is automatic or manual.
unsigned char receive;  //Data received from the board.
int flag2=0;  //Decide whether to start or stop the water pump.




void setup() {   //set up code.
    servo1.attach(servoPin1);
    servo2.attach(servoPin2);
    servo3.attach(servoPin3);
    pinMode(firePin,INPUT);
    pinMode(IN1,OUTPUT);
    pinMode(IN2,OUTPUT);
    pinMode(13,OUTPUT);
    Serial.begin(115200);  //Communication with board at 115200 baud rate.
}

void loop() {
  
  /*
      We receives 1 byte from the board.   1 1 1 1 1 1 1 1
 (bit) first   Determine whether the mode is automatic or manual.
      second   Determine left and right of servo motor, if the first dip switch is up, The servo motor does not move.
      third    Determine up and dowm of servo motor, if the second dip switch is up, The servo motor does not move.
      fourth   The tower's barrel goes up.   
      fifth    The tower's barrel goes down.
      sixth    Turn the tower to the left.
      seventh  Turn the tower to the right.
      eighth   Shoot water.  
  
  */
  
Start:
     
if(flag==1&&((receive&0x80)==0)){ //Manual mode , only water pump can be controlled.
        while(!Serial.available()); //Until We can receive.
           receive=Serial.read();  
           if((!(receive&0x01))&&flag2){//Stop motor.
             stopMotor();
             flag2=0;
           }
           else if((receive&0x01) && (!flag2)){ //Shoot water
             MotorStart();
             flag2=1;
           }
           flag=0;
           sendData();       //Send data to board.
 }          
  else if(((receive&0x80)==0x80)||flag==true){
    while(!Serial.available());
           receive=Serial.read();
           if(receive==0){ //Since received data is 0, it switches to automatic mode.
             goto Auto;
           }
           if((!(receive&0x01))&&flag2){   //Stop water pump
             stopMotor();
             flag2=0;
           }
           else if((receive&0x01) && (!flag2)){//Start water pump.
             MotorStart();
             flag2=1;
           }
          sendData(); //Send data to board
          
          
          /*
          Since the left and right sides of the tower are controlled by servo motors 1 and 2,
          it is necessary to turn the motor according to each situation.
          */
     if((receive&0x44)==0x04)//Turn the tower to the left.
     {
         if(servo2.read()==0){
              if(servo1.read()<=5)
                     servo1.write(0);  //If the angle of servo motor is below 5 , it moves to 0.
               
               else
                    servo1.write(servo1.read()-5);// Reduce the angle of servo motor by 5 degrees.
            }
           else{ 
                if(servo2.read()<=5)//If the angle of servo motor is below 5 , it moves to 0.
                     servo2.write(0);                  
                else
                    servo2.write(servo2.read()-5); // Reduce the angle of servo motor by 5 degrees.
            }
     }
     
     else if((receive&0x42)==0x02)//Turn the tower to the right.
     {
       if(servo1.read()==180){
              if(servo2.read()+5>180)
               {
.                  servo2.write(180);  // Since the angle of servo motor2 can not e bigger than 180, Set to 180.
               }
               else
                    servo2.write(servo2.read()+5);
            }
        else{
                if(servo1.read()+5>180)
                {
                   servo1.write(180);  // Since the angle of servo motor2 can not e bigger than 180, Set to 180.                }
                else
                    servo1.write(servo1.read()+5);
         }

     
     }
     
     //Changed the angle of servo motor 3 to controll barrel of tower.
     
     if((receive&0x30)==0x10){  //If it is smaller than 160, raise the barrel by 3 degrees. 
       if(servo3.read()<=157)
         servo3.write(servo3.read()+3);
       else 
           servo3.write(160);    
         
     }
     else if((receive&0x28)==0x08){ // If it is greater than 60 degrees, reduce the servo motor angle by 4.
          if(servo3.read()<=60){
              servo3.write(60);
          }
          else
              servo3.write(servo3.read()-4);
     }
     flag=1;   // Pass conditional and set flag to 1.
   }
  
else{  
Auto:    /*
            In automatic mode, the tower maintains a guard.
            Servo 1 and  servo 2 alterately operate,
            if the value received from board is 128 or more, switch to manual mode.
         */
      for(angle=servo2.read(); angle>=0; angle-=3)
      {
        
          while(!Serial.available());
           receive=Serial.read();
           digitalWrite(13,HIGH);
          sendData(); 
           if(receive>=128){ //If receive is greater than 128, switch to manual mode.
                     flag=1;
                     goto Start;           
              }
          
          servo2.write(angle);  
        }
        
      for(angle=servo1.read(); angle>=0; angle-=3)
      {
          while(!Serial.available());
           receive=Serial.read();
          sendData();
           if(receive>=128){//If receive is greater than 128, switch to manual mode.
                     flag=1;
                     goto Start;           
              }
          servo1.write(angle); 
      }
       

      for(angle=0; angle<=180; angle+=3)
      {
          while(!Serial.available());
           receive=Serial.read();
          sendData();
           if(receive>=128){//If receive is greater than 128, switch to manual mode.
                     flag=1;
                     goto Start;           
              }
          
          servo1.write(angle); 
        }
       
      for(angle=0; angle<=180; angle+=3)
      {
          while(!Serial.available());
           receive=Serial.read();
          sendData();
           if(receive>=128){//If receive is greater than 128, switch to manual mode.
                     flag=1;
                     goto Start;           
              }
          servo2.write(angle);
     
    }
      
   
      for(angle=180; angle>=0; angle-=3)
      {
          while(!Serial.available());
           receive=Serial.read();
          sendData();
           if(receive>=128){//If receive is greater than 128, switch to manual mode.
                     flag=1;
                     goto Start;           
              }
          
          servo2.write(angle); 
        }
      
      for(angle=180; angle>=0; angle-=3)
      {
          while(!Serial.available());
           receive=Serial.read();
          sendData();
           if(receive>=128){//If receive is greater than 128, switch to manual mode.
                     flag=1;
                     goto Start;           
              }
          servo1.write(angle);    
      }

   }
    
}
void stopMotor()  //Stop the water pump when the suppression is completed.
{
    digitalWrite(IN1,LOW);
    digitalWrite(IN2,LOW);
}
void MotorStart()//Start water pump. Shoot water
{
    digitalWrite(IN1,LOW);
    digitalWrite(IN2,HIGH);
}

void sendData()
{
  delay(30);   //delay
  char buf[30];
  
  //Receive value from sensor
  fireVal=analogRead(firePin);
  waterVal=analogRead(waterPin);
  coVal=analogRead(coPin);
  
  //Data to send to the board.
  sprintf(buf, "%4d %4d %4d", fireVal, waterVal, coVal);
  Serial.write(buf,15);  //Send data to the board.
  Serial.flush();        
}
