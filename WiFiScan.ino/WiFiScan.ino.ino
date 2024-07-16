#include "WiFi.h"
#include <UniversalTelegramBot.h>
#include <WiFiClientSecure.h>

// Replace with your network credentials
const char* ssid = "WIFI@ITERA";
const char* password= "JanganRemasBijiKu";


// Initialize Telegram BOT
#define BOT_TOKEN "6949427763:AAF2cV4ZKKCTciCWBdRbm_P7FciPy5TAWTo"
WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);

// telegram ID
const String chat_id= "6026236730" ;

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid,password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  client.setInsecure(); // Use with caution. For production, you should install the server's certificate.
  Serial.println("Setup done");
}

void loop() {
  Serial.println("Scan start");

  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  Serial.println("Scan done");

  String message;
  if (n == 0) {
    message = "No networks found";
    Serial.println("no networks found");
  } else {
    message = String(n) + " networks found:\n";
    Serial.print(n);
    Serial.println(" networks found");
    Serial.println("Nr | SSID                             | RSSI | CH | Encryption");
    for (int i = 0; i < n; ++i) {
      // Create the message to send to the Telegram bot
      String networkInfo;
      networkInfo += String(i + 1) + " | ";
      networkInfo += String(WiFi.SSID(i)) + " | ";
      networkInfo += String(WiFi.RSSI(i)) + " | ";
      networkInfo += String(WiFi.channel(i)) + " | ";
      switch (WiFi.encryptionType(i)) {
        case WIFI_AUTH_OPEN:            networkInfo += "open"; break;
        case WIFI_AUTH_WEP:             networkInfo += "WEP"; break;
        case WIFI_AUTH_WPA_PSK:         networkInfo += "WPA"; break;
        case WIFI_AUTH_WPA2_PSK:        networkInfo += "WPA2"; break;
        case WIFI_AUTH_WPA_WPA2_PSK:    networkInfo += "WPA+WPA2"; break;
        case WIFI_AUTH_WPA2_ENTERPRISE: networkInfo += "WPA2-EAP"; break;
        case WIFI_AUTH_WPA3_PSK:        networkInfo += "WPA3"; break;
        case WIFI_AUTH_WPA2_WPA3_PSK:   networkInfo += "WPA2+WPA3"; break;
        case WIFI_AUTH_WAPI_PSK:        networkInfo += "WAPI"; break;
        default:                        networkInfo += "unknown";
      }
      networkInfo += "\n";
      message += networkInfo;
      Serial.print(networkInfo);
      delay(10);
    }
  }
  Serial.println("");

  // Delete the scan result to free memory for code below
  WiFi.scanDelete();

  // Send message to the Telegram bot
  if (bot.sendMessage("6026236730", message, "")) {
    Serial.println("Message sent successfully");
  } else {
    Serial.println("Failed to send message");
  }

  // Wait a bit before scanning again
  delay(60000); // Scan every 60 seconds
}
