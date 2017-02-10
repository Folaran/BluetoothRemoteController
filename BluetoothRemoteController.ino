#include <PulseInZero.h>
#include <PS3BT.h>
#include <usbhub.h>
#include <AFMotor.h>
#define trigPin 12
#define echoPin 13
AF_DCMotor motor1(1,MOTOR12_64KHZ);
AF_DCMotor motor2(2, MOTOR12_2KHZ);


#ifdef dobogusinclude
#include <spi4teensy3.h>
#include <SPI.h>
#endif


USB Usb;

BTD Btd(&Usb);

PS3BT PS3(&Btd, 0x00, 0x15, 0x83, 0x3D, 0x0A, 0x57);

bool printTemperature;
bool printAngle;


void setup(){
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
  PulseInZero::setup(pulseInComplete);
  PulseInZero::begin();

  #if !defined(__MIPSEL__)
    while (!Serial);
  #endif
   if (Usb.Init() == -1) {
   Serial.print(F("\r\nOSC did not start"));
   while (1); //halt
  }
  Serial.print(F("\r\nPS3 Bluetooth Library Started"));
}
  

void loop() {
  Usb.Task();
  motor1.setSpeed(150);
  motor2.setSpeed (150);
    
  long duration, distance;
  digitalWrite(trigPin, LOW);  
  
  digitalWrite(trigPin, HIGH);

  digitalWrite(trigPin, LOW);
  //  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
   

   if (distance < 25)/*if there'slowing: */ { 
      motor1.run(FORWARD); //if there's no obstacle ahead, Go Forward! 
      motor2.run(RELEASE);
   } 
  else{
      motor2.run(RELEASE);
  }
    
  if (PS3.getButtonClick(L1)) 
      motor2.run(FORWARD);

  if (PS3.getButtonClick(R1))
      motor2.run(BACKWARD);
  if (PS3.getButtonClick(R2))
      motor2.run(RELEASE);
     
  if (PS3.getButtonClick(CROSS))
      motor1.run(FORWARD); 
  if (PS3.getButtonClick(SQUARE))
      motor1.run(BACKWARD);
  if (PS3.getButtonClick(CIRCLE))
      motor1.run(RELEASE);

}
void pulseInComplete(unsigned long duration){
 
  PulseInZero::begin();  // Start listening again...  
}
