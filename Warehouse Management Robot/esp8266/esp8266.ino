#include <ESP8266WiFi.h>
#include <SPI.h>
#include <MFRC522.h>
extern "C" {
#include <espnow.h>
}
// L298N motor driver pins (corrected to IN3 and IN4)
#define MOTOR_IN3 D1
#define MOTOR_IN4 D2
#define MOTOR_EN
D3
// RFID connections
#define RFID_POWER_PIN D5
// Controls power to RFID reader (optional)
#define RST_PIN D0
// Reset pin for RC522
#define SS_PIN D8
// SDA pin for RC522
// Create MFRC522 instance
MFRC522 mfrc522(SS_PIN, RST_PIN);
// Flags for RFID scanning
bool rfidActive = false;
bool rfidDataCaptured = false;
String capturedUID = "";
void startMotor() {
digitalWrite(MOTOR_IN3, HIGH);
digitalWrite(MOTOR_IN4, LOW);
analogWrite(MOTOR_EN, 1023);
// Full speed
digitalWrite(RFID_POWER_PIN, HIGH); // Power ON RFID
Serial.println("Motor started");
}
void stopMotor() {
digitalWrite(MOTOR_IN3, LOW);
digitalWrite(MOTOR_IN4, LOW);
analogWrite(MOTOR_EN, 0);
// Motor OFF
digitalWrite(RFID_POWER_PIN, LOW); // Power OFF RFID
Serial.println("Motor stopped");
}
void receiveCallback(uint8_t *mac, uint8_t *incomingData, uint8_t len) {
if (len != 1) {
Serial.println("Invalid data length");
return;
}
uint8_t command = incomingData[0];
Serial.printf("Received command: %d\n", command);
switch (command) {
case 0:
stopMotor();
break;
case 1:
startMotor();
break;
case 2:
Serial.println("Unknown color - maintaining current state");
break;
default:
Serial.printf("Unhandled command: %d\n", command);
}
}
void setup() {
Serial.begin(115200);
WiFi.mode(WIFI_STA);
WiFi.disconnect();
if (esp_now_init() != 0) {
Serial.println("ESP-NOW init failed");
return;
}
esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
esp_now_register_recv_cb(receiveCallback);
// Motor Pins
pinMode(MOTOR_IN3, OUTPUT);
pinMode(MOTOR_IN4, OUTPUT);
pinMode(MOTOR_EN, OUTPUT);
pinMode(RFID_POWER_PIN, OUTPUT);
stopMotor(); // Default: motor stopped, RFID off
// RFID Init
digitalWrite(RFID_POWER_PIN, HIGH);
delay(100); // Let RFID power stabilize
SPI.begin();
mfrc522.PCD_Init();
Serial.println("RFID reader initialized");
rfidActive = true;
}
void loop() {
if (rfidActive && !rfidDataCaptured) {
if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
// Capture UID
capturedUID = "";
for (byte i = 0; i < mfrc522.uid.size; i++) {
capturedUID += String(mfrc522.uid.uidByte[i], HEX);
}
capturedUID.toUpperCase();
// Sample Data (you can change dynamically later)
String source = "Warehouse_A";
String destination = "Dock_3";
String parcelType = "Electronics";
String timestamp = "2025-04-26 15:00"; // (Fixed timestamp for now)
Serial.println("\n--- RFID Tag Scanned ---");
Serial.println("Captured UID: " + capturedUID);
Serial.println("Source: " + source);
Serial.println("Destination: " + destination);
Serial.println("Parcel Type: " + parcelType);
Serial.println("Timestamp: " + timestamp);
Serial.println("-------------------------");
// After first scan, power off RFID
rfidDataCaptured = true;
rfidActive = false;
digitalWrite(RFID_POWER_PIN, LOW); // Save power
}
}