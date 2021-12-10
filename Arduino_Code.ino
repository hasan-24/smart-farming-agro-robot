/*
 * bt rx:
front- a
back- b
left- c
right- d
*/

/*sensors:
 * d4,d5- motion sensor
 * d6-ldr
 * d22- sound sensor
 * d23- buzzer
 * a0- moisture
 * a1- raindrop
 * a2- water
 * a3-soilmoisture
 * a4-sonar trig
 * a5-sonar echo
 */
#include "Adafruit_CCS811.h"
Adafruit_CCS811 ccs;
////////////////////////////
#define trig A4
#define echo A5
long duration, distance, sonar;
///////////////////////////////
char data;
char a,b,c,d;
int spd=255;
int spd2=spd/5;
int tm=420;
int co2, tvoc;
int motionfront, motionback, sound, moisture, raindrop, water, soilmoisture, light;
void setup() {
  Serial.begin(9600);
  if(!ccs.begin()){
    Serial.println("Failed to start sensor!");
    while(1);
  }
  while(!ccs.available());
  
  
  pinMode(4,INPUT); //motion front
  pinMode(5,INPUT); //motion back
  pinMode(6,INPUT); //LDR
  pinMode(22,INPUT); //sound sensor
  pinMode(23,OUTPUT); //buzzer -----------
  pinMode(A0,INPUT); //moisture sensor
  pinMode(A1,INPUT); //raindrop sensor
  pinMode(A2,INPUT); //water sensor
  pinMode(A3,INPUT); //soilmoisture


  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

  pinMode(8,OUTPUT);  //EN L
  pinMode(9,OUTPUT);  //L+
  pinMode(10,OUTPUT); //L-
  pinMode(11,OUTPUT); //R+
  pinMode(12,OUTPUT); //R-
  pinMode(13,OUTPUT); //EN R
}

void loop() {
motionfront= digitalRead(4);
motionback= digitalRead(5);
light= digitalRead(6);
sound= digitalRead(22);
moisture= analogRead(A0);
raindrop= analogRead(A1);
water= analogRead(A2);
soilmoisture=analogRead(A3);
SonarSensor(trig, echo);
sonar= distance;

co2=ccs.geteCO2();
  tvoc=ccs.getTVOC();
  if(ccs.available()){
    if(!ccs.readData()){
      Serial.print("CO2: ");
      Serial.print(co2);
      Serial.print("ppm, TVOC: ");
      Serial.println(tvoc);
    }
    else{
      Serial.println("ERROR!");
      while(1);
    }
  }

Serial.print("distance=");
Serial.print(sonar);
Serial.println("cm");
Serial.print("font motion:");
Serial.println(motionfront);
Serial.print("back motion:");
Serial.println(motionback);
Serial.print("sound sensor:");
Serial.println(sound);
Serial.print("air moisture:");
Serial.println(moisture);
Serial.print("rain drops:");
Serial.println(raindrop);
Serial.print("land water level:");
Serial.println(water);
Serial.print("light:");
Serial.println(light);
Serial.println("-------------------------------------------");
Serial.println(" ");

if(Serial.available() > 0)  
{
    data = Serial.read(); 
    Serial.println(data);
}
    if(data=='a')  //front
    {
      analogWrite(8, spd);
      analogWrite(13, spd);
      digitalWrite(9, HIGH);
      digitalWrite(10, LOW);
      digitalWrite(11, HIGH);
      digitalWrite(12, LOW);
      Serial.println("front");
      data='x';
    }
    else if(data=='b')  //back
    {
      analogWrite(8, spd);
      analogWrite(13, spd);
      digitalWrite(9, LOW);
      digitalWrite(10, HIGH);
      digitalWrite(11, LOW);
      digitalWrite(12, HIGH);
      Serial.println("back");
      data='x';
    }
    else if(data=='c')  //left 
    {

      analogWrite(8, 0);
      analogWrite(13, spd); 
      digitalWrite(11, HIGH);
      digitalWrite(12, LOW);
      Serial.println("left"); 
      data='x';    
    }
    else if(data=='d')  //right
    {
      analogWrite(8, spd);
      analogWrite(13, 0);
      digitalWrite(9, HIGH);
      digitalWrite(10, LOW);
      Serial.println("right");
      data='x';
    }
    
    //////////////////////////////////////////
    else if(data=='m')  //s1
    {
      Serial.println("speed 1");
      spd=170;
      tm=2000;
      data='x';
    }
    else if(data=='n')  //s2
    {
      Serial.println("speed 2");
      spd=210;
      tm=670;
      data='x';
    }
    else if(data=='o')  //s3
    {
      Serial.println("speed 3");
      spd=255;
      tm=420;
      data='x';
    }

    /////////////////////////////////////
    else
    {
      analogWrite(9, 0);
      analogWrite(10, 0);
      analogWrite(11, 0);
      analogWrite(12, 0);
      Serial.println("stop movement");
    }
}



void SonarSensor(int trig,int echo)
{
digitalWrite(trig, LOW);
delayMicroseconds(2);
digitalWrite(trig, HIGH);
delayMicroseconds(10);
digitalWrite(trig, LOW);
duration = pulseIn(echo, HIGH);
distance = (duration/2) / 29.1;
}
