
int lmotorA=9;
int lmotor1B=11;
int rmotorA=10;
int rmotorB=12;


void setup() {
pinMode(lmotorA, OUTPUT); 
pinMode(lmotorB, OUTPUT); 
pinMode(rmotorA, OUTPUT); 
pinMode(rmotorB, OUTPUT); 
//pinMode(9, OUTPUT);
Serial.begin(9600); // Starts the serial communication
}
void loop() {
    frwd();
    delay(5000);
    backwrd();
    delay(5000);
    turnLeft();
    delay(5000);
    turnRight();
    delay(5000);
}

void frwd(){
  digitalWrite(lmotorA, HIGH);  
  digitalWrite(lmotorB, LOW);
  digitalWrite(rmotorA, LOW);
  digitalWrite(rmotorB, HIGH);
}

void turnLeft(){
  digitalWrite(lmotorA, LOW);
  digitalWrite(lmotorB, HIGH);
  digitalWrite(rmotorA, LOW);
  digitalWrite(rmotorB, HIGH);
}



void backwrd(){
  digitalWrite(lmotorA, LOW);
  digitalWrite(lmotorB, HIGH);
  digitalWrite(rmotorA, HIGH);
  digitalWrite(rmotorB, LOW);
}

void turnRight(){
  digitalWrite(lmotorA, HIGH);
  digitalWrite(lmotorB, LOW);
  digitalWrite(rmotorA, HIGH);
  digitalWrite(rmotorB, LOW);
}

void stopRobot(){
  digitalWrite(lmotorA, LOW);
  digitalWrite(lmotorB, LOW);
  digitalWrite(rmotorA, LOW);
  digitalWrite(rmotorB, LOW);
}
