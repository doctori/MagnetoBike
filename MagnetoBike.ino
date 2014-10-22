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
#define SHAPE_SIZE 600
#define ROTATION_SPEED 0 // ms delay between cube draws

Adafruit_MCP23017 mcp;
Adafruit_LSM303 lsm;
int SCREEN_WIDTH = uView.getLCDWidth();
int SCREEN_HEIGHT = uView.getLCDHeight();

float d = 3;
float px[] = { -d,  d,  d, -d, -d,  d,  d, -d };
float py[] = { -d, -d,  d,  d, -d, -d,  d,  d };
float pz[] = { -d, -d, -d, -d,  d,  d,  d,  d };

float p2x[] = {0,0,0,0,0,0,0,0};
float p2y[] = {0,0,0,0,0,0,0,0};

float r[] = {0,0,0};

MicroViewWidget *widget[3];	

int leftPin = 8;
int rightPin = 9;
int topPin = 10;
void drawCube(int x,int y,int z)
{
	r[0]=x*PI/180.0; // Add a degree
	r[1]=y*PI/180.0; // Add a degree
	r[2]=z*PI/180.0; // Add a degree
	if (r[0] >= 360.0*PI/180.0) r[0] = 0;
	if (r[1] >= 360.0*PI/180.0) r[1] = 0;
	if (r[2] >= 360.0*PI/180.0) r[2] = 0;

	for (int i=0;i<8;i++)
	{
		float px2 = px[i];
		float py2 = cos(r[0])*py[i] - sin(r[0])*pz[i];
		float pz2 = sin(r[0])*py[i] + cos(r[0])*pz[i];

		float px3 = cos(r[1])*px2 + sin(r[1])*pz2;
		float py3 = py2;
		float pz3 = -sin(r[1])*px2 + cos(r[1])*pz2;

		float ax = cos(r[2])*px3 - sin(r[2])*py3;
		float ay = sin(r[2])*px3 + cos(r[2])*py3;
		float az = pz3-150;
		
		p2x[i] = SCREEN_WIDTH/2+ax*SHAPE_SIZE/az;
		p2y[i] = SCREEN_HEIGHT/2+ay*SHAPE_SIZE/az;
	}

	uView.clear(PAGE);
	for (int i=0;i<3;i++) 
	{
		uView.line(p2x[i],p2y[i],p2x[i+1],p2y[i+1]);
		uView.line(p2x[i+4],p2y[i+4],p2x[i+5],p2y[i+5]);
		uView.line(p2x[i],p2y[i],p2x[i+4],p2y[i+4]);
	}    
	uView.line(p2x[3],p2y[3],p2x[0],p2y[0]);
	uView.line(p2x[7],p2y[7],p2x[4],p2y[4]);
	uView.line(p2x[3],p2y[3],p2x[7],p2y[7]);
	uView.display();
}

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
	uView.clear(PAGE);	// erase the memory buffer, when next uView.display() is called, the OLED will be cleared.
	widget[0] = new MicroViewSlider(0,0,0,20);		// declare widget0 as a Slider at x=0, y=0, min=0, max=100
	widget[1] = new MicroViewSlider(0,15,0,20);	// declare widget0 as a Slider at x=0, y=10, min=0, max=150
	widget[2] = new MicroViewSlider(0,30,0,20);		// declare widget0 as a Slider at x=0, y=20, min=0, max=50

} 
void turnItOn(Adafruit_LSM303 lsm,Adafruit_MCP23017 mcp){
  lsm.read();
  int x = map(lsm.magData.x,-800,800,-100,100);
  int accx = map(lsm.accelData.x,-1000,1000,0,20);
 
  int y = map(lsm.magData.y,-800,800,-100,100);
  int accy = map(lsm.accelData.y,-1000,1000,0,20);
  int z = map(lsm.magData.z,-800,800,-100,100);
  float accz = map(lsm.accelData.z,-1000,1000,0,20);
  if(x > -20 && x < 20){
    mcp.digitalWrite(leftPin,HIGH);
  }else{
    mcp.digitalWrite(leftPin,LOW); 
  }
  if(y > -20 && y < 20){
    mcp.digitalWrite(rightPin,HIGH);
  }else{
    mcp.digitalWrite(rightPin,LOW);
  }
   if(z > -20 && z < 20){
    mcp.digitalWrite(topPin,HIGH);
  }else{
    mcp.digitalWrite(topPin,LOW);
  }
 widget[0]->setValue(accx);
  widget[1]->setValue(accy);
  widget[2]->setValue(accz);
  uView.display();
  
}
void loop()  {
  turnItOn(lsm,mcp);
  delay(200);

}



