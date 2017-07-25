/*
The code to work the EDSAC simulating tape reader stop waiting for me dan
 */
#include <math.h>

int sensorPin[05] = {A4,A3,A2,A1,A0}; //pins for the analog ins
int ledPin = 13;      // select the pin for the LED
int sensorValue[05];
int oldValue[05];
int olderValue[05];
int oldestValue[05];
int bits[05];
double bitsValue[05];
double olderResponse = 0;
double oldestResponse = 0;
double currentResponse = 0;

void setup() {
  // declare the ledPin for the green leds as an OUTPUT:
  pinMode(ledPin, OUTPUT); 
  Serial.begin(9600); 
}

void loop() {
  // shift everything about:
  int i = 0;
  int j = 0;
  oldestResponse = olderResponse;
  olderResponse = currentResponse;
  for (i = 0; i < 5; i++){
    oldestValue[i] = olderValue[i];
    olderValue[i]  = oldValue[i];
    oldValue[i] = sensorValue[i];
    sensorValue[i] = analogRead(sensorPin[i]);
  };
  //make any changes to the bits if they have changed by a significant amount recently
  for (j = 0; j < 5; j++){
    if(sensorValue[j] < oldestValue[j] - 25){
      bits[j] = 1;
      bitsValue[j] = pow( 2 ,  j);
    }
    else if(sensorValue[j] < oldestValue[j] + 25){
      bits[j] = 0;
      bitsValue[j] = 0;
    };
  }

  currentResponse = bitsValue[0] + bitsValue[1] + bitsValue[2] + bitsValue[3] + bitsValue[4];
  if(currentResponse == olderResponse){
  }
  else{
    //[rint the number in binary
    Serial.print("Binary: ");
    Serial.print(bits[4]);
    Serial.print(bits[3]);
    Serial.print(bits[2]);
    Serial.print(bits[1]);
    Serial.print(bits[0]);
    Serial.print("    Actual value: ");
    //get the right number of spaces so the next bit lines up
    if(currentResponse < 10){
      Serial.print(" ");
    };
    //print the decimal value
    Serial.print(currentResponse);
    Serial.print("    Differences: ");
    int k = 0;
    for (k=0; k<5; k++){
      //work out the right number of spaces for each difference
      if (sensorValue[k] - oldestValue[k] < -99.5){
        Serial.print(" ");
      }
      else if (sensorValue[k] - oldestValue[k] < -9.5){
        Serial.print("  ");
      }
      else if (sensorValue[k] - oldestValue[k] < -0.5){
        Serial.print("   ");
      }
      else if (sensorValue[k] - oldestValue[k] < 9.5){
        Serial.print("    ");
      }
      else if (sensorValue[k] - oldestValue[k] < 99.5){
        Serial.print("   ");
      }
      else{
        Serial.print("  ");
      };
      //print those differences
      Serial.print(sensorValue[k] - oldestValue[k]);
    };
    //new line and carriage return
    Serial.println();
  };
  // turn the leds on so we can see the dots
  digitalWrite(ledPin, HIGH);  
  delay(30);                  
}
