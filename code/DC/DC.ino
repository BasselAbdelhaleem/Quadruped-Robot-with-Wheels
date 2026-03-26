#define IN1 2
#define IN2 3
#define PWM 5
int sp = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(PWM, OUTPUT);
}

void loop()
{
  sp = map(analogRead(A0), 0, 1023, 0, 255);
  Serial.println(sp);
  analogWrite(PWM, sp);
  digitalWrite(IN1,HIGH) ;
  digitalWrite(IN2,LOW) ;
}
