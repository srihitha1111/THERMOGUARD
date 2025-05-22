#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>

// MLX90614 sensor object
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

// Wi-Fi Credentials
#define WIFI_SSID "YourWiFiName"
#define WIFI_PASSWORD "YourWiFiPassword"

// Firebase Credentials
#define API_KEY "your-firebase-web-api-key"
#define DATABASE_URL "https://your-project.firebaseio.com/"  // Include the full URL with https

// Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

void setup() {
  Serial.begin(115200);
  Wire.begin();

  // Initialize MLX90614 sensor
  if (!mlx.begin()) {
    Serial.println("Error connecting to MLX90614.");
    while (1);
  }

  // Connect to Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi Connected");

  // Firebase Configuration
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  // Optional: Anonymous Sign-in
  auth.user.email = "";
  auth.user.password = "";

  // Initialize Firebase
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() {
  // Read object temperature (in Celsius)
  float temp = mlx.readObjectTempC();
  Serial.print("Temperature: ");
  Serial.println(temp);

  // Push data to Firebase Realtime Database
  if (Firebase.RTDB.pushFloat(&fbdo, "/ThermoGuard/Temperature", temp)) {
    Serial.println("Data sent to Firebase successfully");
  } else {
    Serial.print("Firebase Error: ");
    Serial.println(fbdo.errorReason());
  }

  delay(10000);  // Send every 10 seconds
}
