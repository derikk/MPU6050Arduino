#include <Wire.h>

long gyroX, gyroY, gyroZ;
long lgyroX, lgyroY, lgyroZ;
float rotX, rotY, rotZ;
float rXcalib, rYcalib, rZcalib;
float degX, degY, degZ;
long lXcalib, lYcalib, lZcalib;

void setup() {
  // put your setup code here, to run once
  Serial.begin(9600);
  Wire.begin();
  setupMPU();
  callibrate();
}
void loop() {
  // put your main code here, to run repeatedly:
  recordGyroRegisters();
  calculateDegrees();
  printData();
  delay(1000);
}
void setupMPU(){
  Wire.beginTransmission(0b1101000);//code
  Wire.write(0x6B);
  Wire.write(0b00000000);
  Wire.endTransmission();
  Wire.beginTransmission(0b1101000);//code
  Wire.write(0x1B);
  Wire.write(0b00000000);
  Wire.endTransmission();
  Wire.beginTransmission(0b1101000);//code
  Wire.write(0x1C);  
  Wire.write(0b00000000);
  Wire.endTransmission(); 
}
void callibrate(){
  Wire.beginTransmission(0b1101000);
  Wire.write(0x43);
  Wire.endTransmission();
  for(int i=0; i<10; i++){
    Wire.requestFrom(0b1101000,6);
    while(Wire.available() < 6);
    lXcalib = lXcalib + Wire.read()<<8|Wire.read();
    lYcalib = lYcalib + Wire.read()<<8|Wire.read();
    lZcalib = lZcalib + Wire.read()<<8|Wire.read();
    delay(100);
  }
  rXcalib = lXcalib/10;
  rYcalib = lYcalib/10;
  rZcalib = lZcalib/10;
  Serial.println("...Callibrated...");
  Serial.print(rXcalib);
  Serial.print(rYcalib);
  Serial.print(rZcalib);
}
void recordGyroRegisters(){
  Wire.beginTransmission(0b1101000);
  Wire.write(0x43);
  Wire.endTransmission();
  for(int i=0; i<10; i++){
    Wire.requestFrom(0b1101000,6);
    while(Wire.available() < 6);
    lgyroX = lgyroX + Wire.read()<<8|Wire.read();
    lgyroY = lgyroY + Wire.read()<<8|Wire.read();
    lgyroZ = lgyroZ + Wire.read()<<8|Wire.read();
    delay(100);
  }
  processGyroData();
}
void processGyroData(){
  gyroX = lgyroX/10;
  gyroY = lgyroY/10;
  gyroZ = lgyroZ/10;
  rotX = gyroX / 131.0;
  rotY = gyroY / 131.0;
  rotZ = gyroZ / 131.0;
}
void calculateDegrees(){
  degX = degX + rotX;
  degY = degY + rotY;
  degZ = degZ + rotZ;
}
void printData(){
  //Serial.print("Gyro (degrees) - ");
  Serial.print(" X = ");
  //Serial.print(degX);
  Serial.print(rotX);
  Serial.print(" Y = ");
  //Serial.print(degY);
  Serial.print(rotY);
  Serial.print(" Z = ");
  //Serial.print(degZ);
  Serial.print(rotZ);
}
