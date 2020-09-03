  #include <Wire.h>
#include <Adafruit_MotorShield.h>

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMSbot(0x61); // Rightmost jumper closed
Adafruit_MotorShield AFMStop(0x60); // Default address, no jumpers

// Connect a stepper motor with 200 steps per revolution (1.8 degree)
// to motor port #2 (M3 and M4)
Adafruit_StepperMotor *xMotor = AFMStop.getStepper(200, 2);
Adafruit_StepperMotor *yMotor = AFMStop.getStepper(200, 1);
Adafruit_StepperMotor *zMotor = AFMSbot.getStepper(200, 2);

//This stores the string received over Bluetooth
String msg;
// These will store the integer values to be sent to the stepper motors
int xint;
int yint;
int zint;

// This function is to separate the string into key:value pairs
String splitString(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

// This function is to get the value from the key:value pair
String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;
    
    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[1]+1) : "";
}

void setup()
{
  Serial.begin(9600);//Initialize communications to the serial monitor in the Arduino IDE
  Serial.println("Started");//Tell the serial monitor that the sketch has started.
  int motorSpeed = 50;
  AFMStop.begin();
  AFMSbot.begin();
  xMotor->setSpeed(motorSpeed);  // 10 rpm 
  yMotor->setSpeed(motorSpeed);
  zMotor->setSpeed(motorSpeed);
}

void loop()
{
  if (Serial.available() > 0){
  msg = Serial.readString();
  Serial.println(msg);
  // The next three lines split the message into three separate key:value pairs 
  String xval = splitString(msg, ',', 0);
  String yval = splitString(msg, ',', 1);
  String zval = splitString(msg, ',', 2);
  // The next three lines split the key:value pairs and return just the value
  String x = getValue(xval, ':', 0);
  String y = getValue(yval, ':', 0);
  String z = getValue(zval, ':', 0);
  // The next three lines convert the values from string to int

  Serial.println(xval);
  Serial.println(yval);
  Serial.println(zval);
  Serial.println(x);
  Serial.println(y);
  Serial.println(z);
  xint = x.toInt();
  yint = y.toInt();
  zint = z.toInt();
//Move the x motor
  if (xint < 0){
    xint = -xint;
    xMotor->step(xint, BACKWARD, DOUBLE);
    xint = 0;
    xMotor->release();
  }
  else if (xint > 0){
    xMotor->step(xint, FORWARD, DOUBLE);
    xint = 0;
    xMotor->release();
  }
  else {
    xMotor->release();
  }

//move the y motor
    if (yint < 0){
    yint = -yint;
    yMotor->step(yint, BACKWARD, DOUBLE);
    yint = 0;
    yMotor->release();
  }
  else if (yint > 0){
    yMotor->step(yint, FORWARD, DOUBLE);
    yint = 0;
    yMotor->release();
  }
  else {
    yMotor->release();
  }

//Move the z motor
  if (zint < 0){
    zint = -zint;
    zMotor->step(zint, BACKWARD, DOUBLE);
    zint = 0;
    zMotor->release();
  }
  else if (zint > 0){
    zMotor->step(zint, FORWARD, DOUBLE);
    zint = 0;
    zMotor->release();
  }
  else {
    zMotor->release();
  }


  }
}
