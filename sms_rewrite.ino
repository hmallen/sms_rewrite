#define smsPowerDelay 5000

const int smsPowerPin = 22;

void smsPower(bool powerState) {
  // Power on GPRS and set proper modes for operation
  if (powerState == true) {
    digitalWrite(smsPowerPin, HIGH);
    delay(500);
    digitalWrite(smsPowerPin, LOW);

    delay(smsPowerDelay);

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
  }

  // Power off GPRS
  else {
    digitalWrite(smsPowerPin, HIGH);
    delay(1000);
    digitalWrite(smsPowerPin, LOW);
  }
}

void setup() {
  pinMode(smsPowerPin, OUTPUT); digitalWrite(smsPowerPin, LOW);

  Serial.begin(9600);
  Serial1.begin(19200);

  smsPower(true);

  smsFlush();
}

void loop() {
  if (Serial.available()) {
    while (Serial.available()) {
      Serial1.write(Serial.read());
      delay(10);
    }
    delay(100);
    if (Serial1.available()) {
      while (Serial1.available()) {
        Serial.write(Serial1.read());
        delay(10);
      }
    }
  }
  if (Serial1.available()) {
    String smsMessageRaw = "";
    while (Serial1.available()) {
      char c = Serial1.read();
      smsMessageRaw += c;
      delay(10);
    }
    Serial.print("smsMessageRaw: ");
    Serial.println(smsMessageRaw);
    smsHandler(smsMessageRaw, true);
  }
}

void smsHandler(String smsMessageRaw, bool execCommand) {
  String smsRecNumber = "";
  String smsMessage = "";

  int numIndex = smsMessageRaw.indexOf('"') + 3;
  Serial.print("numIndex: ");
  Serial.println(numIndex);

  int smsIndex = smsMessageRaw.lastIndexOf('"') + 3;
  Serial.print("smsIndex: ");
  Serial.println(smsIndex);

  for (numIndex; ; numIndex++) {
    char c = smsMessageRaw.charAt(numIndex);
    if (c == '"') break;
    smsRecNumber += c;
  }
  Serial.print("smsRecNumber: ");
  Serial.println(smsRecNumber);

  for (smsIndex; ; smsIndex++) {
    char c = smsMessageRaw.charAt(smsIndex);
    if (c == '\n' || c == '\r') break;
    smsMessage += c;
  }
  Serial.print("smsMessage: ");
  Serial.println(smsMessage);

  if (execCommand == true) {
    int smsCommand = 0;

    if (smsMessage.length() == 1) smsCommand = smsMessage.toInt();
    else; // Send SMS stating invalid command received (to incoming number)

    // Some sort of "if data available, then proceed to switch case"
    switch (smsCommand) {
      case 1:
        Serial.println("Command #1");
        break;
      case 2:
        // Activate buzzer
        Serial.println("Command #2");
        break;
      case 3:
        // Trigger smoke signal
        Serial.println("Command #3");
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
