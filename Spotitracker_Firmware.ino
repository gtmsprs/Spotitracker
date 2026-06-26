```cpp
#include <Arduino.h>
#include <ArduinoJson.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <WiFi.h>
#include <SpotifyEsp32.h>
#include <SPI.h>

char* SSID = "PPGGE";
char* PASSWORD = "!Avatar@p2g2!";

const char* CLIENT_ID = "YOUR_CLIENT_ID";
const char* CLIENT_SECRET = "YOUR_CLIENT_SECRET";

Spotify sp(CLIENT_ID, CLIENT_SECRET);

String playlistNames[] = {
  "Cyberpunk",
  "Coding",
  "Gym",
  "Lofi",
  "Casual"
};

String playlistIDs[] = {
  "6cxqEcoQEMRlBx3mg8oEzn",
  "6ASdGziugOilYI18cWf3qc",
  "3IeXd0prMTYuyKvCPU1y8Q",
  "4tdyKU1OD0thmzzPWRw1wS",
  "0KhIP3cHsrHrbZfw9tOzly"
};

const int playlistCount =
    sizeof(playlistNames) / sizeof(playlistNames[0]);

void playPlaylist(String playlistID)
{
  String payload =
      "{\"context_uri\":\"spotify:playlist:" +
      playlistID +
      "\"}";

  response r =
      sp.RestApiPut(
          "/v1/me/player/play",
          payload.length(),
          payload.c_str());

  Serial.print("HTTP Status: ");
  Serial.println(r.status_code);

  if (r.status_code == 204)
  {
    Serial.println("Playlist Started!");
  }
  else
  {
    Serial.println("Playlist Failed!");
  }
}

void setup()
{
  Serial.begin(115200);
  delay(1000);

  WiFi.begin(SSID, PASSWORD);

  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("\nConnected!");

  Serial.println("Connecting to Spotify...");

  sp.begin();

  while (!sp.is_auth())
  {
    sp.handle_client();
    delay(100);
  }

  Serial.println("Connected to Spotify!");

  Serial.println("\n==== PLAYLIST MENU ====");

  for (int i = 0; i < playlistCount; i++)
  {
    Serial.print(i);
    Serial.print(" : ");
    Serial.println(playlistNames[i]);
  }

  Serial.println("\nType a number and press Enter.");
}

void loop()
{
  sp.handle_client();

  if (Serial.available())
  {
    int choice = Serial.parseInt();

    if (choice >= 0 && choice < playlistCount)
    {
      Serial.print("Playing: ");
      Serial.println(playlistNames[choice]);

      playPlaylist(playlistIDs[choice]);
    }
    else
    {
      Serial.println("Invalid playlist number.");
    }

    while (Serial.available())
      Serial.read();
  }

  delay(100);
}