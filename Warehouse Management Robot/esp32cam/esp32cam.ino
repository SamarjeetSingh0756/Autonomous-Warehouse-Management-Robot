#include <WiFi.h>
#include <esp_now.h>
#include <Wire.h>
#include "
Adafruit_TCS34725.h
#include <ESP32Servo.h>
#include <HTTPClient.h>
#include "esp_camera.h"
#include "esp_http_server.h"
#include <base64.h>
// ----------- CAMERA PINS -----------
#define PWDN_GPIO_NUM
32
#define RESET_GPIO_NUM
-1
#define XCLK_GPIO_NUM
0
#define SIOD_GPIO_NUM
26
#define SIOC_GPIO_NUM
27
#define Y9_GPIO_NUM
35
#define Y8_GPIO_NUM
34
#define Y7_GPIO_NUM
39
#define Y6_GPIO_NUM
36
#define Y5_GPIO_NUM
21
#define Y4_GPIO_NUM
19
#define Y3_GPIO_NUM
18
#define Y2_GPIO_NUM
5
#define VSYNC_GPIO_NUM
25
#define HREF_GPIO_NUM
23
#define PCLK_GPIO_NUM
22
// ----------- CONFIGURATIONS -----------
// Your WiFi credentials
const char* ssid = "Redmi12";
const char* password = "12345678";
// Your laptop IP address running the Flask app
const char* flaskServer = "http:// 192.168.150.199:5000";
// Update this to your IP
// MAC address of ESP8266
uint8_t receiverMAC[] = {0xF4, 0xCF, 0xA2, 0x4D, 0xE5, 0x11};
// Update this
// I2C Pins for ESP32-CAM
#define SDA_PIN 14
#define SCL_PIN 13
// Servo pin
const int servoPin = 15;
// Buzzer pin
const int buzzerPin = 2;
// Change this to your actual buzzer pin
// Global states
String currentMode = ""; // Start with no mode selected
bool emergencyStop = false;
String previousMode = ""; // To track mode changes
bool cameraStream
Active = true;
unsigned long lastImageSend = 0;
const unsigned long IMAGE_INTERVAL = 1000; // Send image every 1 second
// Objects
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS,
TCS34725_GAIN_4X);
Servo myServo;
// ----------- CAMERA SETUP -----------
bool setupCamera() {
camera_config_t config;
config.ledc_channel = LEDC_CHANNEL_0;
config.ledc_timer = LEDC_TIMER_0;
config.pin_d0 = Y2_GPIO_NUM;
config.pin_d1 = Y3_GPIO_NUM;
config.pin_d2 = Y4_GPIO_NUM;
config.pin_d3 = Y5_GPIO_NUM;
config.pin_d4 = Y6_GPIO_NUM;
config.pin_d5 = Y7_GPIO_NUM;
config.pin_d6 = Y8_GPIO_NUM;
config.pin_d7 = Y9_GPIO_NUM;
config.pin_xclk = XCLK_GPIO_NUM;
config.pin_pclk = PCLK_GPIO_NUM;
config.pin_vsync = VSYNC_GPIO_NUM;
config.pin_href = HREF_GPIO_NUM;
config.pin_sscb_sda = SIOD_GPIO_NUM;
config.pin_sscb_scl = SIOC_GPIO_NUM;
config.pin_pwdn = PWDN_GPIO_NUM;
config.pin_reset = RESET_GPIO_NUM;
config.xclk_freq_hz = 20000000;
config.pixel_format = PIXFORMAT_JPEG;
// Initialize with lower quality but faster
config.frame_size = FRAMESIZE_VGA;
config.jpeg_quality = 12; // 0-63, lower means higher quality
config.fb_count = 2;
// Camera init
esp_err_t err = esp_camera_init(&config);
if (err != ESP_OK) {
Serial.printf("Camera init failed with error 0x%x", err);
return false;
}
sensor_t * s = esp_camera_sensor_get();
s->set_brightness(s, 1); // Increase brightness
s->set_contrast(s, 1); // Increase contrast
return true;
}
// Function to send camera frame to Flask server
void sendCameraFrame() {
if (!cameraStream
Active || millis() - lastImageSend < IMAGE_INTERVAL) {
return;
}
camera_fb_t * fb = esp_camera_fb_get();
if (!fb) {
Serial.println("Camera capture failed");
return;
}
HTTPClient http;
http.begin(String(flaskServer) + "/upload_image");
http.addHeader("Content-Type", "application/octet-stream");
// Send the image data
int httpResponseCode = http.POST(fb->buf, fb->len);
if (httpResponseCode > 0) {
Serial.printf("Image uploaded, code: %d\n", httpResponseCode);
} else {
Serial.printf("Image upload error: %s\n", http.errorToString(httpResponseCode).c_str());
}
http.end();
esp_camera_fb_return(fb);
lastImageSend = millis();
}
// ----------- BUZZER FUNCTIONS -----------
void beep(int times) {
for (int i = 0; i < times; i++) {
digitalWrite(buzzerPin, HIGH);
delay(200);
digitalWrite(buzzerPin, LOW);
if (i < times - 1) {
delay(200); // Pause between beeps
}
}
}
// ----------- ESP-NOW SEND -----------
void sendCommand(uint8_t cmd) {
if (emergencyStop) {
uint8_t emergencyData[] = {0}; // Create a regular array for emergency stop
esp_now_send(receiverMAC, emergencyData, sizeof(emergencyData));
Serial.println("EMERGENCY: Forcing STOP");
return;
}
esp_now_send(receiverMAC, &cmd, 1);
Serial.printf("Sent command %d\n", cmd);
}
// ----------- MODE + COMM
AND FETCH -----------
void fetchModeAndCommand() {
HTTPClient http;
http.begin(String(flaskServer) + "/status");
int code = http.GET();
if (code == 200) {
String payload = http.getString();
previousMode = currentMode; // Save previous mode before updating
if (payload.indexOf("emergency") >= 0) {
emergencyStop = true;
sendCommand(0);
} else if (payload.indexOf("manual") >= 0) {
currentMode = "MANUAL";
if (previousMode != "MANUAL") {
beep(2); // Beep twice for manual mode
Serial.println("Switched to MANUAL mode");
}
} else if (payload.indexOf("auto") >= 0) {
currentMode = "AUTO";
if (previousMode != "AUTO") {
beep(3); // Beep three times for auto mode
Serial.println("Switched to AUTO mode");
}
}
}
http.end();
}
void fetchManualCommand() {
HTTPClient http;
http.begin(String(flaskServer) + "/manual_command");
int code = http.GET();
if (code == 200) {
String cmd = http.getString();
if (cmd == "start") sendCommand(1);
else if (cmd == "stop") sendCommand(0);
else if (cmd == "left") {
myServo.write(0);
delay(1000);
myServo.write(90);
}
else if (cmd == "right") {
myServo.write(180);
delay(1000);
myServo.write(90);
}
}
http.end();
}
// ----------- COLOR DETECTION -----------
void handleColorDetection() {
uint16_t clear, red, green, blue;
tcs.getRawData(&red, &green, &blue, &clear);
// Improved color detection based on raw values
// Using the data you provided
// Check for RED
if (red > 85 && green < 50 && blue < 50) {
Serial.println("Red detected");
sendCommand(0);
}
// Check for BLUE
else if (blue > 75 && red < 75 && green < 85 && blue > red) {
Serial.println("Blue detected");
// No longer waiting for approval
sendCommand(0);
}
// Check for GREEN
else if (green > 50 && red < 60 && blue < 50 && green > red && green > blue) {
Serial.println("Green detected — turning RIGHT");
sendCommand(0);
myServo.write(180);
delay(1500);
myServo.write(90);
sendCommand(1);
}
// Check for YELLOW
else if (red > 120 && green > 90 && blue < 65 && red > blue && green > blue) {
Serial.println("Yellow detected — turning LEFT");
sendCommand(0);
myServo.write(0);
delay(1500);
myServo.write(90);
sendCommand(1);
}
else {
Serial.println("Unknown color - no action");
// Don't send any command for unknown colors
}
}
// ----------- SETUP -----------void setup() { Serial.begin(115200); // Setup buzzer pin pinMode(buzzerPin, OUTPUT); digitalWrite(buzzerPin, LOW); myServo.attach(servoPin); myServo.write(90); // center // Initialize camera if (!setupCamera()) { Serial.println("Camera setup failed!"); } else { Serial.println("Camera initialized successfully"); } Wire.begin(SDA_PIN, SCL_PIN); if (!tcs.begin()) { Serial.println("Color sensor not found"); while (1); }
WiFi.mode(WIFI_ST
A);
WiFi.begin(ssid, password);
Serial.println("Connecting to WiFi...");
// Wait indefinitely for WiFi connection
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}
Serial.println("\nWiFi connected!");
beep(1); // One beep when WiFi is connected
Serial.print("IP Address: ");
Serial.println(WiFi.localIP());
if (esp_now_init() != ESP_OK) {
Serial.println("ESP-NOW init failed");
return;
}
esp_now_peer_info_t peerInfo = {};
memcpy(peerInfo.peer_addr, receiverMAC, 6);
peerInfo.channel = 0;
peerInfo.encrypt = false;
if (esp_now_add_peer(&peerInfo) != ESP_OK) {
Serial.println("Failed to add peer");
return;
}
Serial.println("Setup complete, waiting for mode selection...");
// Wait for user to select a mode
while (currentMode == "") {
fetchModeAndCommand();
delay(1000);
}
}
// ----------- LOOP -----------
void loop() {
bool wifiOk = WiFi.status() == WL_CONNECTED;
if (wifiOk) {
fetchModeAndCommand();
// Send camera frame regardless of mode
sendCameraFrame();
if (emergencyStop) {
delay(1000);
return;
}
if (currentMode == "AUTO") {
handleColorDetection();
} else if (currentMode == "MANUAL") {
fetchManualCommand();
}
} else {
// Try to reconnect WiFi
Serial.println("WiFi disconnected. Attempting to reconnect...");
WiFi.begin(ssid, password);
// Wait for reconnection
unsigne
d long reconnectStart = millis();
while (WiFi.status() != WL_CONNECTED && millis() - reconnectStart < 10000) {
delay(500);
Serial.print(".");
}
if (WiFi.status() == WL_CONNECTED) {
Serial.println("\nWiFi reconnected!");
beep(1);
}
}
delay(2000);
}