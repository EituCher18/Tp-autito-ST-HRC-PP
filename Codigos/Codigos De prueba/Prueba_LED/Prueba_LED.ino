#define Led1
#define Led2 
#define Led3
#define Led4
void setup() {
pinMode(Led1, OUTPUT);
pinMode(Led2, OUTPUT);
pinMode(Led3, OUTPUT);
pinMode(Led4, OUTPUT);
}

void loop() {
digitalWrite(Led1, HIGH);
digitalWrite(Led2, HIGH);
digitalWrite(Led3, HIGH);
digitalWrite(Led4, HIGH);
delay(1000);
digitalWrite(Led1, LOW);
digitalWrite(Led2, LOW);
digitalWrite(Led3, LOW);
digitalWrite(Led4, LOW);
delay(1000);
}
