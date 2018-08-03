// defines pins numbers
const int trigPin = 8;
const int echoPin = 7;
int lmotorA=9;
int lmotorB=11;
int rmotorA=10;
int rmotorB=12;

// defines variables
long duration;
int distance;
void setup() {
pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin, INPUT); // Sets the echoPin as an Input
pinMode(lmotorA, OUTPUT); 
pinMode(lmotorB, OUTPUT); 
pinMode(rmotorA, OUTPUT); 
pinMode(rmotorB, OUTPUT); 
//pinMode(9, OUTPUT);
Serial.begin(9600); // Starts the serial communication
}
void loop() {
     // digitalWrite(9, HIGH);
     // Clears the trigPin
     digitalWrite(trigPin, LOW);
     delayMicroseconds(2);
     // Sets the trigPin on HIGH state for 10 micro seconds
     digitalWrite(trigPin, HIGH);
     delayMicroseconds(10);
     digitalWrite(trigPin, LOW);
     // Reads the echoPin, returns the sound wave travel time in microseconds
     duration = pulseIn(echoPin, HIGH);
     // Calculating the distance
     distance= duration*0.034/2;
     // Prints the distance on the Serial Monitor
     Serial.print("Distance: ");
     Serial.println(distance);
     if(distance <= 15)
       {
        stopRobot();
        delay(1000);
        turnRight();
        delay(1000);
       }
      else{
        frwd(); 
      }
}

void frwd(){
  digitalWrite(lmotorA, HIGH);  
  digitalWrite(lmotorB, LOW);
  digitalWrite(rmotorA, HIGH);
  digitalWrite(rmotorB, LOW);
}

void turnLeft(){
  digitalWrite(lmotorA, LOW);
  digitalWrite(lmotorB, LOW);
  digitalWrite(rmotorA, HIGH);
  digitalWrite(rmotorB, LOW);
}



void backwrd(){
  digitalWrite(lmotorA, LOW);
  digitalWrite(lmotorB, HIGH);
  digitalWrite(rmotorA, LOW);
  digitalWrite(rmotorB, HIGH);
}

void turnRight(){
  digitalWrite(lmotorA, HIGH);
  digitalWrite(lmotorB, LOW);
  digitalWrite(rmotorA, LOW);
  digitalWrite(rmotorB, LOW);
}

void stopRobot(){
  digitalWrite(lmotorA, LOW);
  digitalWrite(lmotorB, LOW);
  digitalWrite(rmotorA, LOW);
  digitalWrite(rmotorB, LOW);
}
