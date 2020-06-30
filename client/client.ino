int inPin = D4;
int val = 1;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(inPin, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  val = digitalRead(inPin);
  Serial.println(val);
}
