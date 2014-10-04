#include <space01.h>
#include <space02.h>
#include <space03.h>
#include <font5x7.h>
#include <font8x16.h>
#include <7segment.h>
#include <MicroView.h>
#include <fontlargenumber.h>
#include <Adafruit_LSM303.h>
#include <Wire.h>
#include <Adafruit_MCP23017.h>

Adafruit_MCP23017 mcp;
Adafruit_LSM303 lsm;

int leftPin = 4;
int rightPin = 5;
int topPin = 6;
void setup()  {
  uView.begin();		// begin of MicroView
  uView.clear(ALL);	// erase hardware memory inside the OLED controller

  mcp.begin();
  mcp.pinMode(leftPin,OUTPUT);
  mcp.pinMode(rightPin,OUTPUT);
  mcp.pinMode(topPin,OUTPUT);
  uView.clear(PAGE);
  uView.setCursor(18,6);
  uView.print("Hello");
  uView.setCursor(13,15);
  uView.print("Bitches");
  uView.display(); 
  delay(1000);
if (!lsm.begin()){
    uView.clear(PAGE);
    uView.setCursor(18,18);
    uView.print("LSM FAILURE");
    uView.display();
    while(1);
  }
} 
void turnItOn(Adafruit_LSM303 lsm,Adafruit_MCP23017 mcp,int y){
  lsm.read();   
  if(lsm.magData.x > -200 && lsm.magData.x < 200){
    mcp.digitalWrite(leftPin,HIGH);
  }else{
    mcp.digitalWrite(leftPin,LOW); 
  }
  if(lsm.magData.y > -200 && lsm.magData.y < 200){
    mcp.digitalWrite(rightPin,HIGH);
  }else{
    mcp.digitalWrite(rightPin,LOW);
  }
   if(lsm.magData.z > -200 && lsm.magData.z < 200){
    mcp.digitalWrite(topPin,HIGH);
  }else{
    mcp.digitalWrite(topPin,LOW);
  }
    uView.clear(PAGE);
    uView.setCursor(0,y);
    uView.print("Mag X:");
    uView.print((int)lsm.magData.x);
    y=y+9;
    uView.setCursor(0,y);
    uView.print("Mag Y:");
    uView.print((int)lsm.magData.y);
    y=y+9;
    uView.setCursor(0,y);    
    uView.print("Mag Z:");
    uView.println((int)lsm.magData.z);
    uView.display();
}
void loop()  {
   int y = 0; 
  turnItOn(lsm,mcp,y);
  delay(200);
}


