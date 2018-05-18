#include <ESP8266WiFi.h>

void setup() {
  // start the serial connection
  Serial.begin(115200);
  Serial.println();
  
  //Input login
  WiFi.begin("network-name", "pass-to-network");

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());

}

void loop() {
  // put your main code here, to run repeatedly:
  String temp = getVal("Temperature");
  String moist = getVal"Moisture");
  Serial.println(temp + "," + moist);
  delay(10000);
}

String getVal(String type) {

  WiFiClient client;
  String value;

  Serial.println("Making http request");
  if (client.connect("io.adafruit.com", 80)) {
    https://io.adafruit.com/api/v2/DisAalborg/feeds/kontoret.moisture/data
    client.println("GET /api/v2/DisAalborg/feeds/kontoret." + type + "/data");
    client.println("Host: " + "io.adafruit.com" + ":80");
    client.println("User-Agent: ESP8266/1.0");
    client.println("Content-type: text/xml; charset=\"utf-8\"");
    client.println(F("Content-type: application/json"));

    client.println("Connection: close"); 
    //client.println(F("Connection: keep-alive"));
    Serial.println("GETing hue status");
    while (client.connected())
      {
        if (client.available())
        {
          Serial.println("searching for value");
          client.findUntil("\"value\":", "\0");
          value = client.readStringUntil(',');
          Serial.println(temp);
        }
      }
    Serial.println("closing down client");
    client.flush();  
    client.stop();
  }
  return value;
}
