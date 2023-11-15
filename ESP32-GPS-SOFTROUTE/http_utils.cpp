#include "http_utils.h"
#include <HTTPClient.h>

int sendPUTRequest(const char* serverAddress, const char* requestBody, const char* authToken) {
  HTTPClient http;
  http.begin(serverAddress);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Authorization", authToken);
  
  int httpResponseCode = http.PUT(requestBody);

  http.end();
  return httpResponseCode;
}
