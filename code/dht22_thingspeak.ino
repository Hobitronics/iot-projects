#include <WiFi.h>
#include <DHT.h>
#include <ThingSpeak.h>

// Replace with your WiFi credentials
const char* ssid = "Your wifi name";
const char* password = "Your Wifi password";

// ThingSpeak API
const char* thingSpeakApiKey = "YOUR_API_KEY";  // Replace with your Write API Key
unsigned long channelID = YOUR_CHANNEL_ID;       // Replace with your Channel ID

// DHT22 setup
#define DHTPIN 4        // GPIO4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// WiFi client and ThingSpeak
WiFiClient client;

void setup() {
  Serial.begin(115200);
  dht.begin();

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");

  ThingSpeak.begin(client);
}

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Check if readings are valid
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    delay(10000);
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C, Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  // Send to ThingSpeak
  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, humidity);
  int status = ThingSpeak.writeFields(channelID, thingSpeakApiKey);

  if (status == 200) {
    Serial.println("Data sent to ThingSpeak");
  } else {
    Serial.print("Error sending data. HTTP error code: ");
    Serial.println(status);
  }

  delay(20000); // Wait 20 seconds (ThingSpeak minimum)
}
