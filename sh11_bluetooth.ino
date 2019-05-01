#include <SoftwareSerial.h>


// Bluetooth Configuration
// Arduino | HM-19
//    D2   |  TX
//    D3   |  RX
SoftwareSerial ble(2, 3);

// Misc Global Vars
const long Int24Max = 8388607;
const long Int24Min = -8388608;
const int MockCount = 8; // Count of extra variables needed
unsigned int count = 1;


void setup() {
  Serial.begin(115200);
  Serial.print("Sketch:   "); Serial.print(__FILE__);
  Serial.print("Uploaded: "); Serial.println(__DATE__);
  
  ble.begin(115200);
  Serial.println("Bluetooth SoftwareSerial started at 115200 bps");

  randomSeed(analogRead(5));
}

void loop() {
  // Sensors will have 8 inputs w/ 24bit signed integers.
  // Adding mock data to fill in missing sensors
  // NOTE: signed 24-bit int has range −8,388,608 to 8,388,607 (8 chars long)
  char extra[MockCount*13] = "";
  for (int i = 1; i <= MockCount; i++) {
    char temp[13];
    sprintf(temp, ",%ld", random(Int24Min, Int24Max));
    strcat(extra, temp);
  }

  String str = "[" + String(millis()) + "," + String(count) + String(extra) + "]";

  // Log str
  Serial.println(str);

  // WARNING: Seeing ble buffer overflow despite flush()
  ble.println(str);
  ble.flush();

  if (ble.overflow()) {
    Serial.println("WARNING: BLE is overflowing!");
  }
  
  if (count == 33) {
    count = 1;
  } else {
    count++;
  }

  delay(180);
}
