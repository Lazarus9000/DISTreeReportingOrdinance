// ESP8266 to Adafruit IO and back

/************************** Configuration ***********************************/

// edit the config.h tab and enter your Adafruit IO credentials
// and any additional configuration needed for WiFi, cellular,
// or ethernet clients.
#include "config.h"

#include <MedianFilter.h>

/************************ Example Starts Here *******************************/

// this int will hold the current count for our sketch
int count = 0;

// set up the feeds
AdafruitIO_Feed *temp = io.feed("kontoret.temperature");
AdafruitIO_Feed *hum = io.feed("kontoret.humidity");
AdafruitIO_Feed *moist = io.feed("kontoret.moisture");


#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN            2         // Pin which is connected to the DHT sensor.

// Uncomment the type of sensor in use:
#define DHTTYPE           DHT11     // DHT 11 
DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS;

int moisture = 0;
  
void setup() {

  pinMode(BUILTIN_LED, OUTPUT);
  pinMode(A0, INPUT);
  
  // start the serial connection
  Serial.begin(115200);

  // wait for serial monitor to open
  while (! Serial);

  Serial.print("Connecting to Adafruit IO");

  // connect to io.adafruit.com
  io.connect();

  // wait for a connection
  while (io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());

  dht.begin();
  Serial.println("DHTxx Unified Sensor Example");
  // Print temperature sensor details.
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.println("Temperature");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" *C");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" *C");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" *C");
  Serial.println("------------------------------------");
  // Print humidity sensor details.
  dht.humidity().getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.println("Humidity");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println("%");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println("%");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println("%");
  Serial.println("------------------------------------");
  // Set delay between sensor readings based on sensor details.
  delayMS = sensor.min_delay / 1000;

  

}

void loop() {

  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();

  sensors_event_t event;

  float iterations = 9;
  float count = 0;
  //The window size of the median filter is linked to the amount of iterations
  int nine = 9;
  MedianFilter mediantempvar(nine, 0);

  for (int i = 0; i <= iterations; i++) {
    io.run();
    // Delay between measurements.
    delay(delayMS);
    // Get temperature event and print its value.
    
    dht.temperature().getEvent(&event);


    if (isnan(event.temperature)) {
      Serial.println("Error reading temperature!");
      //temp->save(0);
    }
    else {
      Serial.print("Temperature: ");
      Serial.print(event.temperature);
      Serial.println(" *C");

      if (count < 9) {
        mediantempvar.in(event.temperature);
      }

      count++;

    }
  }

 temp->save(mediantempvar.out());

  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
  Serial.println("Error reading humidity!");
  }
  else {
    Serial.print("Humidity: ");
    Serial.print(event.relative_humidity);
    Serial.println("%");
    hum->save(event.relative_humidity);
  }

  moisture = analogRead(A0);
  moisture = moisture/10;
  moist->save(moisture);

}
