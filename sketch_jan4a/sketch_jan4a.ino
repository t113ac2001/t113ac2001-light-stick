const int Rpin = 9;
const int Gpin = 10;
const int Bpin = 11;
const int buttonPin = 7;
const int potPin = A1;

int mode = 0;
bool lastButton = HIGH;
bool powerOn = true;

unsigned long pressStart = 0;
unsigned long lastUpdate = 0;
int stepIndex = 0;

void setup() {
  pinMode(Rpin, OUTPUT);
  pinMode(Gpin, OUTPUT);
  pinMode(Bpin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
  bool buttonNow = digitalRead(buttonPin);

  // -------- 按鍵判斷 --------
  if (buttonNow == LOW && lastButton == HIGH) {
    pressStart = millis();
  }

  if (buttonNow == HIGH && lastButton == LOW) {
    unsigned long pressTime = millis() - pressStart;

    if (pressTime > 1000) {          // 長按
      powerOn = !powerOn;
      if (!powerOn) turnOff();
    } else {                          // 短按
      if (powerOn) mode = (mode + 1) % 3;
    }
  }

  lastButton = buttonNow;

  if (!powerOn) return;

  // -------- 節奏控制 --------
  int speedValue = analogRead(potPin);
  int delayTime = map(speedValue, 0, 1023, 40, 400);

  if (millis() - lastUpdate > delayTime) {
    lastUpdate = millis();
    stepIndex++;
  }

  // -------- 燈效 --------
  if (mode == 0) rainbowBreath(stepIndex);
  else if (mode == 1) dualFlash(stepIndex);
  else burstFlash(stepIndex);
}

// -------- 燈光函式 --------
void setColor(int r, int g, int b) {
  analogWrite(Rpin, 255 - r);
  analogWrite(Gpin, 255 - g);
  analogWrite(Bpin, 255 - b);
}

void turnOff() {
  setColor(0, 0, 0);
}

void rainbowBreath(int s) {
  int r = abs(128 - (s % 256));
  int g = abs(128 - ((s + 85) % 256));
  int b = abs(128 - ((s + 170) % 256));
  setColor(r, g, b);
}

void dualFlash(int s) {
  if (s % 2 == 0) setColor(255, 0, 255);
  else setColor(0, 255, 255);
}

void burstFlash(int s) {
  if (s % 4 == 0) setColor(255,255,255);
  else setColor(random(255), random(255), random(255));
}

