const int pinCurrent = A6;  // A6
const int pinBuzzer = 2;    // D2

int powerOnPrevious = 2;  // 0 = power off, 1 = power on, 2 = undefined
unsigned int powerOnLastSecondTimes = 0;

unsigned int powerOnSeconds = 0;
unsigned int powerOffSeconds = 0;


void setup() {
  pinMode(pinCurrent, INPUT);
  pinMode(pinBuzzer, OUTPUT);
  Serial.begin(250000);  // https://arduino.stackexchange.com/a/299
  Serial.println("Init. Idea: 11.01.2025. Firmware: 11.01.2025.");
}

void beep() {
  Serial.println("beep()");
  tone(pinBuzzer, 4000);
  Serial.println("beep() end");
}
void stopBeeping() {
  Serial.println("stopBeeping()");
  noTone(pinBuzzer);
  Serial.println("stopBeeping() end");
}

void loop() {
  Serial.println("---------");
  Serial.println("Loop start");
  unsigned long previousMillis = millis();
  Serial.println("Millis: ");
  Serial.println(previousMillis);

  powerOnLastSecondTimes = 0;
  while (millis() - previousMillis < 1000) {         // https://arduino.stackexchange.com/a/12591
    int pinCurrentRead = analogRead(pinCurrent);  // 112 us, 8900 times per second
    // There may be ones between zeroes when there is no current

    // 120 119 118 110 104 93 85 76 67 59 49 41 32 23 14 7 3 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 2 5 9 15 25 33 43 52 60 68 75 85 91 100 107 113 115 116 116 118 118 118 119 118 118 118 118 114 106 95 89 79 71 63 53 45 35 28 19 11 3 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 2 4 8 14 21 32 41 49 58 65 73 82 90 98 105 111 115 116 117 118 118 118 119 118 118 119 119 116 107 99 91 83 75 67 58 49 41 31 23 13 7 2 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 2 6 11 19 27 37 47 53 63 71 79 87 95 103 110 114 115 116 118 118 118
    if (pinCurrentRead > 10) powerOnLastSecondTimes += 1;
  }
  int powerOnCurrent = powerOnLastSecondTimes > 200;  // 4000 when it is powered on

  Serial.println("powerOnLastSecondTimes:");
  Serial.println(powerOnLastSecondTimes);

  Serial.println("powerOnCurrent:");
  Serial.println(powerOnCurrent);

  if (powerOnPrevious == 2) {  // initialize state
    Serial.println("Initializing state");
    powerOnPrevious = powerOnCurrent;
  } else {
    if (powerOnPrevious == 1 && powerOnCurrent == 0) {
      Serial.println("powerOnPrevious == 1 && powerOnCurrent == 0");
      if (powerOnSeconds > 5) {  // debounced // make sure the device was turned on for at least 5 seconds to emit notification
        Serial.println("powerOnSeconds > 5. Notifying 1000ms");
        Serial.println("powerOnSeconds = 0");
        powerOnSeconds = 0;
        Serial.println("powerOffSeconds = 0");
        powerOffSeconds = 0;
        beep();
        delay(1000);
        stopBeeping();
      }
    }
    if (powerOnPrevious == 0 && powerOnCurrent == 1) {
      Serial.println("powerOnPrevious == 0 && powerOnCurrent == 1");
      if (powerOffSeconds > 2) {  // debounced // make sure the device was turned off for at least 2 seconds to emit notification
        Serial.println("powerOffSeconds > 2. Notifying 100ms");
        Serial.println("powerOnSeconds = 0");
        powerOnSeconds = 0;
        Serial.println("powerOffSeconds = 0");
        powerOffSeconds = 0;
        beep();
        delay(100);
        stopBeeping();
      }
    }

    if (powerOnCurrent) {
      Serial.println("powerOffSeconds = 0");
      powerOffSeconds = 0;
      Serial.println("powerOnSeconds += 1");
      powerOnSeconds += 1;
    }
    if (!powerOnCurrent) {
      Serial.println("powerOffSeconds += 1");
      powerOffSeconds += 1;
      Serial.println("powerOnSeconds = 0");
      powerOnSeconds = 0;
    }

    Serial.println("powerOnSeconds");
    Serial.println(powerOnSeconds);
    Serial.println("powerOffSeconds");
    Serial.println(powerOffSeconds);


    Serial.println("Setting powerOnPrevious");
    powerOnPrevious = powerOnCurrent;
  }
  Serial.println("Loop end");
}