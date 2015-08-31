//Reference Value to Get The Status of Parking
#define parked 100
#define fault 4500

//Pin Setting for the 3 Ultrasonic Sensors
#define trigPin 13
#define echoPin 12
#define trigPin2 4
#define echoPin2 5
#define trigPin3 8
#define echoPin3 9
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

//
void loop() {
  long duration, distance,duration2, distance2,duration3, distance3;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2); 
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); 
  digitalWrite(trigPin, LOW);
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
  delay(300);

  if(distance < fault && distance2 < fault && distance3 < fault) {
  if(distance <= parked && distance2 <= parked && distance3 <= parked)
  {
    Serial.write("222");
  }
  else if(distance <= parked && distance2 <= parked && distance3 > parked)
  {
    Serial.write("221");
  }
  else if(distance <= parked && distance2 > parked && distance3 <= parked)
  {
    Serial.write("212");
  }
  else if(distance <= parked && distance2 > parked && distance3 > parked)
  {
    Serial.write("211");
  }
  else if(distance > parked && distance2 <= parked && distance3 <= parked)
  {
    Serial.write("122");
  }
  else if(distance > parked && distance2 <= parked && distance3 > parked)
  {
    Serial.write("121");
  }
  else if(distance > parked && distance2 > parked && distance3 <= parked)
  {
    Serial.write("112");
  }
  else if(distance > parked && distance2 > parked && distance3 > parked)
  {
    Serial.write("111");
  }
  }
  else{
  if(distance > fault && distance2 <= parked && distance3 <= parked)
  {
    Serial.write("322");
  }
  else if(distance > fault && distance2 <= parked && distance3 > parked)
  {
    Serial.write("321");
  }
  else if(distance > fault && distance2 > parked && distance3 <= parked)
  {
    Serial.write("312");
  }
  else if(distance > fault && distance2 > parked && distance3 > parked)
  {
    Serial.write("311");
  }
  else if(distance <= parked && distance2 > fault && distance3 <= parked)
  {
    Serial.write("232");
  }
  else if(distance <= parked && distance2 > fault && distance3 > parked)
  {
    Serial.write("231");
  }
  else if(distance > parked && distance2 > fault && distance3 <= parked)
  {
    Serial.write("132");
  }
  else if(distance > parked && distance2 > fault && distance3 > parked)
  {
    Serial.write("131");
  }
  else if(distance <= parked && distance2 <= parked && distance3 > fault)
  {
    Serial.write("223");
  }
  else if(distance <= parked && distance2 > parked && distance3 > fault)
  {
    Serial.write("213");
  }
  else if(distance > parked && distance2 <= parked && distance3 > fault)
  {
    Serial.write("123");
  }
  else if(distance > parked && distance2 > parked && distance3 > fault)
  {
    Serial.write("113");
  }
  else if(distance > fault && distance2 > fault && distance3 <= parked)
  {
    Serial.write("332");
  }
  else if(distance > fault && distance2 > fault && distance3 > parked)
  {
    Serial.write("331");
  }
  else if(distance <= parked && distance2 > fault && distance3 > fault)
  {
    Serial.write("233");
  }
  else if(distance > parked && distance2 > fault && distance3 > fault)
  {
    Serial.write("133");
  }
  else if(distance > fault && distance2 <= parked && distance3 >fault)
  {
    Serial.write("323");
  }
  else if(distance > fault && distance2 > parked && distance3 >fault)
  {
    Serial.write("313");
  }
  else if(distance > fault && distance2 > fault && distance3 > fault)
  {
    Serial.write("333");
  }
  }
  delay(5000);
}

//End of the Program
