#include <ESP8266WiFi.h>

char deviceStatus[512];
char str [512];
char s [60];

int apChannel = 8;
char apssid[] = "ESP8266";
char ssid[] = "your_router_name";
char pass[]= "your_router_password";
int passwd = 123;
int device;
const char* location;
int count = 0;

int command = 206;
uint8_t mac[6] = {command};
int deviceStatus2; int deviceStatus3;  int deviceStatus4; int deviceStatus5; 

extern "C" void preinit() {
  wifi_set_opmode(STATIONAP_MODE);
  wifi_set_macaddr(SOFTAP_IF, mac);
}

WiFiEventHandler probeRequestPrintHandler;

void startWiFiClient()
{
  Serial.println("Connecting to " + (String)ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: " + WiFi.localIP().toString());
}

void startWiFiAP()
{
  WiFi.softAP(apssid, "<notused>", apChannel, 0, 0);   //(gateway, "<notused>", 7, 1, 0) for hidden SSID.
  Serial.println("AP started");
  Serial.println("IP address: " + WiFi.softAPIP().toString());
}

void setup()
{
  Serial.begin(9600);
  Serial.println();
  startWiFiClient();
  startWiFiAP();
  probeRequestPrintHandler = WiFi.onSoftAPModeProbeRequestReceived(&onProbeRequest);
}

void loop(){}

void onProbeRequest(const WiFiEventSoftAPModeProbeRequestReceived& dataReceived) {
  if (dataReceived.mac[4] == passwd && dataReceived.mac[3] == apChannel)
  {
    device = dataReceived.mac[0];
    if (device == 06) location = "Date_Palms";

     float voltage = dataReceived.mac[1];
     voltage = voltage * 2 / 100;
     deviceStatus2 = (dataReceived.mac[2]);
     deviceStatus3 = (dataReceived.mac[3]);
     deviceStatus4 = (dataReceived.mac[4]);
     deviceStatus5 = (dataReceived.mac[5]);

       Serial.print("Received packet number: "); Serial.println(count++);
  }
} 
