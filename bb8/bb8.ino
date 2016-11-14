int motor_left[] = {2, 3};
int motor_right[] = {4, 5};
int motor1Speed = 6;
int motor2Speed = 9;
//int potPin = A2;
int speed;

// --------------------------------------------------------------------------- Setup
void setup() 
    {
      Serial.begin(57600);

      // Setup motors
      for(int i = 0; i < 2; i++) 
         {
           pinMode(motor_left[i], OUTPUT);
           pinMode(motor_right[i], OUTPUT);
         }
        pinMode(motor1Speed, OUTPUT); 
        pinMode(motor2Speed, OUTPUT);
    }

// --------------------------------------------------------------------------- Loop
void loop() 
    {
      //speed = analogRead(A2);
      drive_forward();
      delay(2000);
      motor_stop();

      
      drive_backward();
      delay(2000);
      motor_stop();
      Serial.println("2");

      turn_left();
      delay(2000);
      motor_stop();
      Serial.println("3");

      turn_right();
      delay(2000);
      motor_stop();
      Serial.println("4");

      motor_stop();
      delay(1000);
      motor_stop();
      Serial.println("5");
    }



// --------------------------------------------------------------------------- Drive

void motor_stop()
    {
      digitalWrite(motor_left[0], LOW);
      digitalWrite(motor_left[1], LOW);

      digitalWrite(motor_right[0], LOW);
      digitalWrite(motor_right[1], LOW);
      delay(25);
    }

void drive_forward()
    {
      //analogWrite(motor1Speed, speed/4);  
      //analogWrite(motor2Speed, speed/4); 
      digitalWrite(motor_left[0], HIGH);
      digitalWrite(motor_left[1], LOW);

      digitalWrite(motor_right[0], HIGH);
      digitalWrite(motor_right[1], LOW);
    }

void drive_backward()
    {
      digitalWrite(motor_left[0], LOW);
      digitalWrite(motor_left[1], HIGH);

      digitalWrite(motor_right[0], LOW);
      digitalWrite(motor_right[1], HIGH);
    }

void turn_left()
    {
      digitalWrite(motor_left[0], LOW);
      digitalWrite(motor_left[1], HIGH);

      digitalWrite(motor_right[0], HIGH);
      digitalWrite(motor_right[1], LOW);
    }

void turn_right()
    {
      digitalWrite(motor_left[0], HIGH);
      digitalWrite(motor_left[1], LOW);

      digitalWrite(motor_right[0], LOW);
      digitalWrite(motor_right[1], HIGH);
    }
