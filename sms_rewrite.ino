const int smsPowerPin = 22;

void smsPower() {
  digitalWrite(smsPowerPin, HIGH);
  delay(500);
  digitalWrite(smsPowerPin, LOW);

  delay(5000);

  Serial1.println("ATE0");
  delay(100);
  Serial1.println("ATQ1");
  delay(100);
  Serial1.println("ATV0");
  delay(100);
  Serial1.println("AT+CMGF=1");
  delay(100);
  Serial1.println("AT+CNMI=2,2,0,0,0");
  delay(100);
  Serial1.println("AT+CMGR=\"REC UNREAD\"");
  delay(100);

  smsFlush();
}

void setup() {
  pinMode(smsPowerPin, OUTPUT); digitalWrite(smsPowerPin, LOW);
  
  Serial.begin(9600);
  Serial1.begin(19200);

  smsPower();

  delay(5000);
}

void loop() {
  if (Serial.available()) {
    Serial1.write(Serial.read());
    delay(10);
  }
  if (Serial1.available()) {
    Serial.write(Serial1.read());
    delay(10);
  }
}

void smsFlush() {
  while (Serial1.available()) {
    char c = Serial1.read();
    delay(10);
  }
}

