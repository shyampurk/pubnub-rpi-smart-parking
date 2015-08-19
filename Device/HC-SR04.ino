//Reference Value to Get The Status of Parking
#define parked 200
#define fault 4000

//Pin Setting for the 3 Ultrasonic Sensors
#define trigPin 13
#define echoPin 12
#define trigPin2 4
#define echoPin2 5
#define trigPin3 8
#define echoPin3 9

//Flags
boolean d1 = 0;
boolean d2 = 0;
boolean d3 = 0;
/*
//Deducting the Low Pulse fromm the Echo Pin
unsigned long echoDeduct(uint8_t pin, uint8_t state, unsigned long timeout)
{
        uint8_t bit = digitalPinToBitMask(pin);
        uint8_t port = digitalPinToPort(pin);
        uint8_t stateMask = (state ? bit : 0);
        unsigned long width = 0; 
        unsigned long numloops = 0;
        unsigned long maxloops = microsecondsToClockCycles(timeout) / 16;
        
        while ((*portInputRegister(port) & bit) == stateMask)
                if (numloops++ == maxloops)
                        return 0;
        
        while ((*portInputRegister(port) & bit) != stateMask)
                if (numloops++ == maxloops)
                        return 0;
        
        while ((*portInputRegister(port) & bit) == stateMask)
                width++;

        return clockCyclesToMicroseconds(width * 10 + 16); 
}
*/
//Initialize the Pins and BAUD rate 
void setup() {
  Serial.begin (9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(trigPin3, OUTPUT);
  pinMode(echoPin3, INPUT);
}

void loop() {
  long duration, distance,duration2, distance2,duration3, distance3;
  //Trigger Pin is made High for 10us and Wait for the Echo Pulse 
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2); 
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); 
  digitalWrite(trigPin, LOW);
  //Obtain the Duration from the HIGH Pulse from Echo Pin
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
  delay(300);
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2); 
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10); 
  digitalWrite(trigPin2, LOW);
  duration2 = pulseIn(echoPin2, HIGH);
  distance2 = (duration2/2) / 29.1;
  delay(300);
  digitalWrite(trigPin3, LOW);
  delayMicroseconds(2); 
  digitalWrite(trigPin3, HIGH);
  delayMicroseconds(10); 
  digitalWrite(trigPin3, LOW);
  duration3 = pulseIn(echoPin3, HIGH);
  distance3 = (duration3/2) / 29.1;
  delay(200);
  //Parking Lot 1
  //Send the Sensor Status for the RPi using the Serial Communication 
  if(distance < fault && distance2 < fault && distance3 < fault) 
  {
          if(distance <= parked && distance2 <= parked && distance3 <= parked)
          {
            Serial.write("01");
          }
          else if(distance <= parked && distance2 <= parked && distance3 > parked)
          {
            Serial.write("02");
          }
          else if(distance <= parked && distance2 > parked && distance3 <= parked)
          {
            Serial.write("03");
          }
          else if(distance <= parked && distance2 > parked && distance3 > parked)
          {
            Serial.write("04");
          }
          else if(distance > parked && distance2 <= parked && distance3 <= parked)
          {
            Serial.write("05");
          }
          else if(distance > parked && distance2 <= parked && distance3 > parked)
          {
            Serial.write("06");
          }
          else if(distance > parked && distance2 > parked && distance3 <= parked)
          {
            Serial.write("07");
          }
          else if(distance > parked && distance2 > parked && distance3 > parked)
          {
            Serial.write("08");
          }
  }
  else
  {
          if(distance > fault && distance2 <= parked && distance3 <= parked)
          {
            Serial.write("09");
          }
          else if(distance > fault && distance2 <= parked && distance3 > parked)
          {
            Serial.write("10");
          }
          else if(distance > fault && distance2 > parked && distance3 <= parked)
          {
            Serial.write("11");
          }
          else if(distance > fault && distance2 > parked && distance3 > parked)
          {
            Serial.write("12");
          }
          else if(distance <= parked && distance2 > fault && distance3 <= parked)
          {
            Serial.write("13");
          }
          else if(distance <= parked && distance2 > fault && distance3 > parked)
          {
            Serial.write("14");
          }
          else if(distance > parked && distance2 > fault && distance3 <= parked)
          {
            Serial.write("15");
          }
          else if(distance > parked && distance2 > fault && distance3 > parked)
          {
            Serial.write("16");
          }
          else if(distance <= parked && distance2 <= parked && distance3 > fault)
          {
            Serial.write("17");
          }
          else if(distance <= parked && distance2 > parked && distance3 > fault)
          {
            Serial.write("18");
          }
          else if(distance > parked && distance2 <= parked && distance3 > fault)
          {
            Serial.write("19");
          }
          else if(distance > parked && distance2 > parked && distance3 > fault)
          {
            Serial.write("20");
          }
  }
  delay(5000);
}

//End of the Program
