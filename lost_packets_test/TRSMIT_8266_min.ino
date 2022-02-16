ADC_MODE(ADC_VCC); //vcc read-mode
#include <ESP8266WiFi.h>

#define RTCMEMORYSTART 65
#define MAXCYCLES 5000000

extern "C" {
#include "user_interface.h"
}
typedef struct {
  int count;
} rtcStore;
rtcStore rtcMem;

int device = 6;
int passwd = 123;
int apChannel = 8;
char* apSSID = "ESP8266";
int sleepTime = 1;
int upTime;
uint8_t sensorData[6];
int n = 0;

void setup() {
  rtcMem.count = 1;
  int n = WiFi.scanNetworks(false, false, apChannel, (uint8*) apSSID);
  Serial.begin(9600);
  Serial.println("");
}
void loop() { 
  sendValues();
  ESP.deepSleep(sleepTime * 1000000, WAKE_NO_RFCAL);
}
void sendValues() {
  readFromRTCMemory();
  int voltage = ESP.getVcc() / 1023.00 * 50; // * (float)1.07;
  sensorData[0] = device;
  sensorData[1] = voltage;
  sensorData[2] = device;
  sensorData[3] = apChannel;
  sensorData[4] = passwd;
  sensorData[5] = random(250);
  wifi_set_macaddr(STATION_IF, sensorData);
  int n = WiFi.scanNetworks(true, false, apChannel, (uint8*) apSSID);
}
void readFromRTCMemory() {
  system_rtc_mem_read(RTCMEMORYSTART, &rtcMem, sizeof(rtcMem));
  Serial.print("Sent packet number: "); Serial.println(rtcMem.count);
  writeToRTCMemory();
}
void writeToRTCMemory() {
  if (rtcMem.count <= MAXCYCLES) {
    rtcMem.count++;
  } else {
    rtcMem.count = 1;
  }
  system_rtc_mem_write(RTCMEMORYSTART, &rtcMem, 4);
}
