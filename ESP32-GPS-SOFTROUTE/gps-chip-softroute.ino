#include "NMEA.h"

#include "wifi_utils.h"
#include "http_utils.h"

#define DELAY 3000
#define LEN(arr) ((int)(sizeof(arr) / sizeof(arr)[0]))

const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* serverAddress = "http://localhost:8090/api/v1/tracking/1";
const char* authToken = "";

union {
  char bytes[4];
  float valor;
} velocidadeGPS;

float latitude;
float longitude;

NMEA gps(GPRMC);                            // Creates a GPS data connection with sentence type GPRMC


void setup() {
  Serial.begin(115200);
  Serial2.begin(9600);
  setupWiFi(ssid, password);                      // Serial1 is connected to the custom chip
  Serial.println("Data received from GPS Softroute:");
}

void loop() {

  while (Serial2.available()) {             // Waits for serial port data

    char serialData = Serial2.read();       // Receives data from GPS serial port
    Serial.print(serialData);


    if (gps.decode(serialData)) {           // Checks if the GPS sentence is valid
      if (gps.gprmc_status() == 'A') {      // Checks if GPS status is 'A'

        velocidadeGPS.valor = gps.gprmc_speed(KMPH);        // Receives GPS speed in km/h

      } else {
        velocidadeGPS.valor = 0;
      }

      latitude  = gps.gprmc_latitude();
      longitude = gps.gprmc_longitude();

      // Add line break
      Serial.println();
      Serial.println();

      // Show Latitude
      Serial.print(" Latitude: ");
      Serial.println(latitude, 8);

      // Show Longitude
      Serial.print("Longitude: ");
      Serial.println(longitude, 8);

      // Show Speed ​​in km/h
      Serial.print("    Speed: ");
      Serial.print(velocidadeGPS.valor);
      Serial.println(" Km/h");

      conectToDabatase(latitude, longitude);

      // Converts Geographic Coordinates to Cartesian Plane
      convertCoordinatesToCartesian(latitude, longitude);
    }
  }
}

void convertCoordinatesToCartesian(float latitude, float longitude) {

  float latRadius = latitude  * (PI) / 180;  // Convert from Degrees to Radians
  float lonRadius = longitude * (PI) / 180;

  int earthRadius = 6371; // Radius in km

  float posX = earthRadius * cos(latRadius) * cos(lonRadius);
  float posY = earthRadius * cos(latRadius) * sin(lonRadius);

  Serial.print("        X: ");  Serial.println(posX);
  Serial.print("        Y: ");  Serial.println(posY);
}

void conectToDabatase(float latitude, float longitude) {
   String requestBody = String("{\"latitude\":") + latitude + ",\"longitude\":" + longitude + "}";
    Serial.println(requestBody);
    int httpResponseCode = sendPUTRequest(serverAddress, requestBody.c_str(), authToken);

    if (httpResponseCode == 200) {
      Serial.println("Actualizado con éxito en el servidor.");
    } else {
      Serial.print("Error en la solicitud. Código de respuesta HTTP: ");
      Serial.println(httpResponseCode);
    }
}