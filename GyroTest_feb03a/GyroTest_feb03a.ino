#include <Wire.h>

long accelX, accelY, accelZ;
float gForceX, gForceY, gForceZ;

float gFXcalib, gFYcalib, gFZcalib;
float rXcalib, rYcalib, rZcalib;

long gyroX, gyroY, gyroZ;
float rotX, rotY, rotZ;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wire.begin();
  setupMPU();
}

void loop() {
  // put your main code here, to run repeatedly:
  recordAccelRegisters();
  recordGyroRegisters();
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
  Wire.write(0x3B);
  Wire.endTransmission();
  Wire.requestFrom(0b1101000, 6);
  while(Wire.available() < 6);
  accelX = Wire.read()<<8|Wire.read();
  accelY = Wire.read()<<8|Wire.read();
  accelZ = Wire.read()<<8|Wire.read();
  processAccelData();
  gFXcalib = gForceX;
  gFYcalib = gForceY;
  gFZcalib = gForceZ;
  Wire.beginTransmission(0b1101000);
  Wire.write(0x43);
  Wire.endTransmission();
  Wire.requestFrom(0b1101000,6);
  while(Wire.available() < 6);
  gyroX = Wire.read()<<8|Wire.read();
  gyroY = Wire.read()<<8|Wire.read();
  gyroZ = Wire.read()<<8|Wire.read();
  processGyroData();
}

void recordAccelRegisters(){
  Wire.beginTransmission(0b1101000);
  Wire.write(0x3B);
  Wire.endTransmission();
  Wire.requestFrom(0b1101000, 6);
  while(Wire.available() < 6);
  accelX = Wire.read()<<8|Wire.read();
  accelY = Wire.read()<<8|Wire.read();
  accelZ = Wire.read()<<8|Wire.read();
  processAccelData();
}

void processAccelData(){
  gForceX = accelX / 16384.0;
  gForceY = accelX / 16384.0;
  gForceZ = accelX / 16384.0;
}

void recordGyroRegisters(){
  Wire.beginTransmission(0b1101000);
  Wire.write(0x43);
  Wire.endTransmission();
  Wire.requestFrom(0b1101000,6);
  while(Wire.available() < 6);
  gyroX = Wire.read()<<8|Wire.read();
  gyroY = Wire.read()<<8|Wire.read();
  gyroZ = Wire.read()<<8|Wire.read();
  processGyroData();
}

void processGyroData(){
  rotX = gyroX / 131.0;
  rotY = gyroY / 131.0;
  rotZ = gyroZ / 131.0;
  
}

void printData(){
  Serial.print("Gyro (degrees) - ");
  Serial.print(" X = ");
  Serial.print(rotX - rXcalib);
  Serial.print(" Y = ");
  Serial.print(rotY - rYcalib);
  Serial.print(" Z = ");
  Serial.print(rotZ - rZcalib);
  Serial.print("Accel (g's) - ");
  Serial.print(" X = ");
  Serial.print(gForceX - gFXcalib);
  Serial.print(" Y = ");
  Serial.print(gForceY - gFYcalib);
  Serial.print(" Z = ");
  Serial.print(gForceZ - gFZcalib);
  Serial.println("");
}
