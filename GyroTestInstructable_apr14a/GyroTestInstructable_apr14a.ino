#include <Wire.h>

//Wiring
//VCC to 5V(MPU-6050 works with 3.3V but GY-521 increases it to 5V.),
//GND to GND,
//SCL to A5,
//SDA to A4,
//ADO to GND,
//INT to digital pin 2.

const int MPU_addr=0x68; int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
int minVal=265; 
int maxVal=402;

double x; 
double y; 
double z;

double xcal;
double ycal;
double zcal;

double xsum;
double ysum;
double zsum;

void setup(){ 
  Wire.begin(); 
  Wire.beginTransmission(MPU_addr); 
  Wire.write(0x6B); Wire.write(0); 
  Wire.endTransmission(true); 
  
  Serial.begin(9600); 
  
  Wire.beginTransmission(MPU_addr); 
  Wire.write(0x3B); 
  Wire.endTransmission(false); 
  Wire.requestFrom(MPU_addr,14,true); 
  
  AcX=Wire.read()<<8|Wire.read(); 
  AcY=Wire.read()<<8|Wire.read(); 
  AcZ=Wire.read()<<8|Wire.read(); 
  
  int xAng = map(AcX,minVal,maxVal,-90,90); 
  int yAng = map(AcY,minVal,maxVal,-90,90); 
  int zAng = map(AcZ,minVal,maxVal,-90,90);
  
  xcal= RAD_TO_DEG * (atan2(-yAng, -zAng)+PI); 
  ycal= RAD_TO_DEG * (atan2(-xAng, -zAng)+PI); 
  zcal= RAD_TO_DEG * (atan2(-yAng, -xAng)+PI);

} 

void loop(){ 
  Wire.beginTransmission(MPU_addr); 
  Wire.write(0x3B); 
  Wire.endTransmission(false); 
  Wire.requestFrom(MPU_addr,14,true); 
  /*
  for(int i=0; i<20, i++;){
    AcX=Wire.read()<<8|Wire.read(); 
    AcY=Wire.read()<<8|Wire.read(); 
    AcZ=Wire.read()<<8|Wire.read(); 
  
    int xAng = map(AcX,minVal,maxVal,-90,90); 
    int yAng = map(AcY,minVal,maxVal,-90,90); 
    int zAng = map(AcZ,minVal,maxVal,-90,90);
  
    xsum= RAD_TO_DEG * (atan2(-yAng, -zAng)+PI) + xsum; 
    ysum= RAD_TO_DEG * (atan2(-xAng, -zAng)+PI) + ysum; 
    zsum= RAD_TO_DEG * (atan2(-yAng, -xAng)+PI) + zsum;

  }
  
  x= xsum / 20.00;
  y= ysum / 20.00;
  z= zsum / 20.00;
  */
  AcX=Wire.read()<<8|Wire.read(); 
  AcY=Wire.read()<<8|Wire.read(); 
  AcZ=Wire.read()<<8|Wire.read(); 
  
  int xAng = map(AcX,minVal,maxVal,-90,90); 
  int yAng = map(AcY,minVal,maxVal,-90,90); 
  int zAng = map(AcZ,minVal,maxVal,-90,90);
  
  x= RAD_TO_DEG * (atan2(-yAng, -zAng)+PI); 
  y= RAD_TO_DEG * (atan2(-xAng, -zAng)+PI); 
  z= RAD_TO_DEG * (atan2(-yAng, -xAng)+PI);
  
  Serial.print("AngleX= "); 
  Serial.println(x - xcal);
  Serial.print("AngleY= "); 
  Serial.println(y - ycal);
  Serial.print("AngleZ= "); 
  Serial.println(z - zcal); 
  Serial.println("-----------------------------------------"); 
  delay(4000); 
}
