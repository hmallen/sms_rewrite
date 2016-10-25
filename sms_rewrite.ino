const int smsPowerPin = 22;

void smsPowerOn() {
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

void smsPowerOff() {
  digitalWrite(smsPowerPin, HIGH);
  delay(1000);
  digitalWrite(smsPowerPin, LOW);
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

void smsHandler(bool execCommand) {
  String smsRecNumber = "";
  String smsMessage = "";

  int numIndex = smsMessageRaw.indexOf('"') + 3;
  Serial.print("numIndex: ");
  Serial.println(numIndex);

  int smsIndex = smsMessageRaw.lastIndexOf('"') + 3;
  Serial.print(F("smsIndex: "));
  Serial.println(smsIndex);

  for (numIndex; ; numIndex++) {
    char c = smsMessageRaw.charAt(numIndex);
    if (c == '"') break;
    smsRecNumber += c;
  }
  for (smsIndex; ; smsIndex++) {
    char c = smsMessageRaw.charAt(smsIndex);
    if (c == '\n' || c == '\r') break;
    smsMessage += c;
  }

  if (execCommand == true) {
    int smsCommand = 0;

    if (smsMessage.length() == 1) smsCommand = smsMessage.toInt();
    else; // Send SMS stating invalid command received (to incoming number)

    // Some sort of "if data available, then proceed to switch case"
    switch (smsCommand) {
      case 1:
        Serial.println("LED activated!");
        // Retrieve updated GPS coordinates and send via SMS
        digitalWrite(smsLED, HIGH);
        delay(1000);
        digitalWrite(smsLED, LOW);
        break;
      case 2:
        // Activate buzzer
        Serial.println(F("Piezo buzzer activated!"));
        digitalWrite(piezoPin, HIGH);
        delay(50);
        digitalWrite(piezoPin, LOW);
        break;
      case 3:
        // Trigger smoke signal
        Serial.println(F("Smoke signal activated!"));
        digitalWrite(smokePin, HIGH);
        delay(2000);
        digitalWrite(smokePin, LOW);
        break;
      default:
        Serial.print("INVALID COMMAND: ");
        Serial.println(smsMessage);
        break;
    }
  }
}

void smsFlush() {
  while (Serial1.available()) {
    char c = Serial1.read();
    delay(10);
  }
}

