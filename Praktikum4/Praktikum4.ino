const int laserPin = 50;
const int receiverPin = 52;

void setup() {
  pinMode(laserPin, OUTPUT);
  pinMode(receiverPin, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(laserPin, HIGH);
  Serial.begin(9600);
  Serial2.begin(9600);
}

void loop() {
  String received = Serial2.readString();

  while(received.length() == 0) {
    delay(50);
    received = Serial2.readString();
  }
  Serial.println(received);
  if(digitalRead(receiverPin) == 1) {
    Serial2.write("Tembus\n");
  } else {
    Serial2.write("Terhalang\n");
  }
  
  delay(2000);
}
