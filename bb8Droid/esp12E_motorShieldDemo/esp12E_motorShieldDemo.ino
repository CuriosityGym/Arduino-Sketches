void setup() {
  // put your setup code here, to run once:
   pinMode(5, OUTPUT);
    pinMode(4, OUTPUT);
     pinMode(0, OUTPUT);
      pinMode(2, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  forward();
  delay(1000);
  backward();
  delay(1000);
  left();
  delay(1000);
  right();
  delay(1000);
  stop();
  delay(1000);
}

void stop(void)
{
    analogWrite(5, 0);
    analogWrite(4, 0);
}
 
void forward(void)
{
    analogWrite(5, 1023);
    analogWrite(4, 1023);
    digitalWrite(0, HIGH);
    digitalWrite(2, HIGH);
}
 
void backward(void)
{
    analogWrite(5, 1023);
    analogWrite(4, 1023);
    digitalWrite(0, LOW);
    digitalWrite(2, LOW);
}
 
void left(void)
{
    analogWrite(5, 1023);
    analogWrite(4, 1023);
    digitalWrite(0, LOW);
    digitalWrite(2, HIGH);
}
 
void right(void)
{
    analogWrite(5, 1023);
    analogWrite(4, 1023);
    digitalWrite(0, HIGH);
    digitalWrite(2, LOW);
}
