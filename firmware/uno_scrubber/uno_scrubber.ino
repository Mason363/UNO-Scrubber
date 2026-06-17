// MAKERWORLD UNO SCRUBBER - FINAL V2
// 2 Modes: Normal & Fast Scrub | Zero Latency

#define PIN_CLK 2
#define PIN_DT 3
#define PIN_SW 4   // Encoder Button
#define KEY_MODE 5 // Toggle Mode
#define KEY_NEW 6  // Macro: Shift + N
#define KEY_S 7    // Macro: S
#define KEY_DEL 8  // Macro: Option + Delete
#define LED_PIN 9  // Status LED

int mode = 0; // 0 = Normal, 1 = Fast
volatile int encoderCount = 0;
int lastReportedCount = 0;
volatile int lastEncoded = 0;
unsigned long lastDebounceTime = 0;

void setup() {
  Serial.begin(115200);
  pinMode(PIN_CLK, INPUT_PULLUP);
  pinMode(PIN_DT, INPUT_PULLUP);
  pinMode(PIN_SW, INPUT_PULLUP);
  pinMode(KEY_MODE, INPUT_PULLUP);
  pinMode(KEY_NEW, INPUT_PULLUP);
  pinMode(KEY_S, INPUT_PULLUP);
  pinMode(KEY_DEL, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);

  // Interrupts for smooth scrolling
  attachInterrupt(digitalPinToInterrupt(PIN_CLK), updateEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PIN_DT), updateEncoder, CHANGE);
}

void loop() {
  // Encoder Logic
  if (encoderCount != lastReportedCount) {
    bool turningRight = (encoderCount > lastReportedCount);
    lastReportedCount = encoderCount;
    if (mode == 0) { // Normal Speed
      if (turningRight) Serial.println("E_LEFT"); // Swapped for Mac Logic
      else Serial.println("E_RIGHT");
    }
    else if (mode == 1) { // Fast Speed
      if (turningRight) Serial.println("E_SHIFT_L");
      else Serial.println("E_SHIFT_R");
    }
  }

  // Button Logic
  if (checkButton(PIN_SW)) Serial.println("CMD_SPACE");
  if (checkButton(KEY_MODE)) {
    mode++;
    if (mode > 1) mode = 0;
    updateLED();
  }
  if (checkButton(KEY_NEW)) Serial.println("CMD_NEW");
  if (checkButton(KEY_S)) Serial.println("CMD_S");
  if (checkButton(KEY_DEL)) Serial.println("CMD_RIPDEL");
}

bool checkButton(int pin) {
  if (digitalRead(pin) == LOW) {
    if (millis() - lastDebounceTime > 200) {
      lastDebounceTime = millis();
      return true;
    }
  }
  return false;
}

void updateLED() {
  if (mode == 0) analogWrite(LED_PIN, 0);   // Off
  if (mode == 1) analogWrite(LED_PIN, 255); // Bright
}

void updateEncoder() {
  int MSB = digitalRead(PIN_CLK);
  int LSB = digitalRead(PIN_DT);
  int encoded = (MSB << 1) | LSB;
  int sum = (lastEncoded << 2) | encoded;
  if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderCount++;
  if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderCount--;
  lastEncoded = encoded;
}
