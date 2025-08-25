#include <Arduino.h>
#include "HX711.h"

// Define Pins 
// const int LOADCELL_DOUT_PIN = 2;
// const int LOADCELL_SCK_PIN = 3;
const int LOADCELL_DOUT_PIN = 4;
const int LOADCELL_SCK_PIN = 5;

//Encoder Pins
const int encoderC1 = 2;
const int encoderC2 = 3;
volatile int lastEncoded = 0; // Here updated value of encoder store.
volatile long encoderValue = 0; // Raw encoder values

//Motor Driver Pins
const int in1 = 12;
const int in2 = 13;
const int enb = 11;

//Speed Calculation 
unsigned long int start = millis();

//Motor control using millis
unsigned long int previousMotorMillis = 0;
const long int interval = 10;

// Millis() for load cell 
unsigned long int load_cellMillis = 0;
const long int load_interval = 100;

// Motor start time 
unsigned long int motorstart = 0;
bool isrun = false;


HX711 scale;

void setup() {

  /////// Motor Driver Setup////////////////////////////////////
  pinMode(in1, OUTPUT); 
  pinMode(in2, OUTPUT); 
  pinMode(enb,OUTPUT);
  digitalWrite(enb, HIGH);
  Serial.begin(57600);

  ////////////////Encoder Setup ////////////////////////////////
  //Initialising the Encoder 
  pinMode(encoderC1, INPUT_PULLUP); 
  pinMode(encoderC2, INPUT_PULLUP);
  

  digitalWrite(encoderC1, HIGH); //turn pullup resistor on
  digitalWrite(encoderC2, HIGH); //turn pullup resistor on

  //call updateEncoder() when any high/low changed seen
  //on interrupt 0 (pin 2), or interrupt 1 (pin 3) 
  attachInterrupt(0, updateEncoder, CHANGE); 
  attachInterrupt(1, updateEncoder, CHANGE);

  // // Corrected lines
  // attachInterrupt(digitalPinToInterrupt(encoderC1), updateEncoder, CHANGE); 
  // attachInterrupt(digitalPinToInterrupt(encoderC2), updateEncoder, CHANGE);

  

  ///////////////Load cell setup////////////////////////

  // Serial.println("HX711 Demo");
  // Serial.println("Initializing the scale");

  // scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

  // Serial.println("Before setting up the scale:");
  // Serial.print("read: \t\t");
  // Serial.println(scale.read());      // print a raw reading from the ADC

  // Serial.print("read average: \t\t");
  // Serial.println(scale.read_average(20));   // print the average of 20 readings from the ADC

  // Serial.print("get value: \t\t");
  // Serial.println(scale.get_value(5));   // print the average of 5 readings from the ADC minus the tare weight (not set yet)

  // Serial.print("get units: \t\t");
  // Serial.println(scale.get_units(5), 1);  // print the average of 5 readings from the ADC minus tare weight (not set) divided
  // // by the SCALE parameter (not set yet)
            
  // scale.set_scale(442.11);
  // //scale.set_scale(-471.497);                      // this value is obtained by calibrating the scale with known weights; see the README for details
  // scale.tare();               // reset the scale to 0

  // Serial.println("After setting up the scale:");

  // Serial.print("read: \t\t");
  // Serial.println(scale.read());                 // print a raw reading from the ADC

  // Serial.print("read average: \t\t");
  // Serial.println(scale.read_average(20));       // print the average of 20 readings from the ADC

  // Serial.print("get value: \t\t");
  // Serial.println(scale.get_value(5));   // print the average of 5 readings from the ADC minus the tare weight, set with tare()

  // Serial.print("get units: \t\t");
  // Serial.println(scale.get_units(5), 1);        // print the average of 5 readings from the ADC minus tare weight, divided
  //           // by the SCALE parameter set with set_scale

  // Serial.println("Readings:");
}

void loop() {

  // for (int i = 0; i <= 500; i++){
  //   digitalWrite(in1, LOW); 
  //   digitalWrite(in2, HIGH);
  //   // Serial.print("Forward  ");
  //   //Serial.println(encoderValue);
  // }

  ////////////////Motor Logic using millis()/////////////////////
  unsigned long currentMillis = millis();
  // if (currentMillis - previousMotorMillis >= interval) {
  //   previousMotorMillis = currentMillis;  

    // Your motor control logic (e.g., run forward)
     digitalWrite(in1, HIGH);
     digitalWrite(in2, LOW);

    // // Print running time
    //   if(!isrun){
    //     motorstart = millis();
    //     isrun = true;
    //   }
    
    // Calculate time since motor started
    // unsigned long motorRunTime = millis() - motorstart;

    Serial.print("encoder value ");
    Serial.println(encoderValue);
    // Serial.print(motorRunTime);
    // Serial.println(" ms");

 
  // }

  ///////////////////////////////////////////////////////////

  // Serial.print("one reading:\t");
  // Serial.print(scale.get_units(), 1);
  // Serial.print("\t| average:\t");
  // Serial.println(scale.get_units(10), 5);

  // delay(5000);

  ///////////////Load cell logic using Millis()///////////////////
  // if(currentMillis-load_cellMillis >= load_interval){
  //   Serial.print("one reading:\t");
  //   Serial.print(scale.get_units(), 1);
  //   Serial.print("\t| average:\t");
  //   Serial.println(scale.get_units(10), 5);
  // }



}
void updateEncoder(){
  int MSB = digitalRead(encoderC1); //MSB = most significant bit
  int LSB = digitalRead(encoderC2); //LSB = least significant bit

  int encoded = (MSB << 1) |LSB; //converting the 2 pin value to single number
  int sum  = (lastEncoded << 2) | encoded; //adding it to the previous encoded value

  if(sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderValue --;
  if(sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderValue ++;

  lastEncoded = encoded; //store this value for next time

}
