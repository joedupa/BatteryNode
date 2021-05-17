#define MQTT              true
#define WEBSOCKETS        true
#define ASYNCWEBSERVER    true
#define LITTLEDB          true

#include <WiFi.h>

const char* ssid = ""; // Your WiFi SSID
const char* password = ""; // Your WiFi Password

String deviceData;
String sensorData;
int device;

#if MQTT
#include <TinyMqtt.h>   // Thanks to https://github.com/hsaturn/TinyMqtt
#endif

#if WEBSOCKETS
#include <ArduinoWebsockets.h>  // Thanks to https://github.com/gilmaimon/ArduinoWebsockets
#endif

#if LITTLEDB
#include <Arduino.h>  // Arduino built in
#include "FS.h"       // Arduino built in
#include <LITTLEFS.h> // Arduino built in
#include <LittleDB.h> // Thanks to https://github.com/pouriamoosavi/LittleDB
#include <ezTime.h>   // Arduino built in
String Timestamp, Date, Time;
int8_t res;

#endif

#if ASYNCWEBSERVER
#include <ESPAsyncWebServer.h>  // Install manually to arduino folder

int index_html_gz_len = 2145;
const uint8_t index_html_gz[] = {
 0x1f,0x8b,0x08,0x08,0xaf,0x22,0x9f,0x60,0x00,0xff,0x69,0x6e,0x64,0x65,0x78,0x2e,0x68,0x74,0x6d,0x6c,0x2e,0x67,0x7a,0x00,0x75,0x56,0x51,0x6f,0xdb,0x36,0x10,0x7e,0x8e,0x7e,0xc5,0x41,0x0f,0xb3,0x8c,0x2c,0x52,0x96,0x16,0x43,0x10,0xcb,0x06,0x9a,0x62,0xd9,0x06,0xb4,0x2f,0x4d,0x86,0x3d,0x74,0x7d,0xa0,0x24,0x3a,0xe6,0x4a,0x91,0x02,0x49,0xd9,0xf5,0xd6,0xfc,0xf7,0x7e,0x27,0xc9,0xb6,0xec,0x24,0x82,0x6c,0xc9,0xe4,0xdd,0xc7,0xbb,0xef,0x3e,0x1e,0x9d,0xaf,0x42,0xad,0x17,0x51,0x5e,0xd8,0x6a,0x8b,0xc7,0xea,0x6a,0xf1,0xb7,0x2c,0xee,0x6d,0xf9,0x55,0x06,0x9f,0x67,0xf8,0x99,0x17,0xae,0xfb,0x44,0x79,0xb3,0xc8,0x95,0x69,0xda,0x40,0xaa,0x9a,0xc7,0x5e,0x9a,0xf0,0x51,0x7a,0x2f,0x1e,0x65,0x4c,0x61,0xdb,0xc8,0x79,0x1c,0xe4,0xb7,0x10,0x93,0x57,0xff,0xe1,0xfd,0xed,0x65,0x4c,0x6b,0xa1,0x5b,0xbc,0x5e,0xfe,0x9a,0xe1,0x8e,0x79,0x8d,0x36,0x04,0x6b,0xc8,0x9a,0x52,0xab,0xf2,0x6b,0x87,0x51,0x3d,0xc0,0x29,0x99,0xc6,0x8b,0x7b,0xbc,0xd3,0x7b,0x5b,0xd7,0xc2,0x54,0x79,0xd6,0x5b,0x2e,0xf2,0xac,0x81,0x5b,0xab,0xbb,0x15,0x9d,0x2c,0xa5,0x5a,0xcb,0x6a,0xb7,0xea,0x62,0xb0,0xf6,0xf4,0x13,0x0d,0x63,0x9e,0x76,0x46,0xb4,0x74,0xb6,0x26,0x2f,0xdd,0x5a,0x3a,0x12,0x4d,0x23,0x85,0xa3,0x42,0x6a,0xbb,0xa1,0x28,0xcf,0x5a,0x4e,0xd8,0x97,0x4e,0x35,0xa1,0x0f,0x7d,0xc2,0xa1,0x67,0xff,0x8a,0xb5,0xe8,0x47,0x27,0x8b,0x28,0x52,0x4b,0x4a,0xe2,0x3d,0x17,0x31,0x29,0x43,0x1b,0x65,0x2a,0xbb,0x99,0xd2,0xff,0xd1,0x1a,0x70,0x1b,0x4f,0x73,0x32,0x72,0x43,0x7b,0x23,0x38,0x6c,0xfc,0x4d,0x96,0xc5,0x74,0x3e,0xd8,0xa6,0xda,0x96,0x22,0x28,0x6b,0xd2,0x95,0xf5,0xc1,0x88,0x5a,0x62,0x2a,0xbe,0xb9,0xbe,0xbc,0x7e,0x1b,0x4f,0x67,0x94,0x65,0x64,0x1b,0x09,0xe0,0x3d,0x42,0x69,0x8d,0x91,0x25,0x7b,0x00,0x3f,0xb5,0xa6,0x9b,0x9e,0xd3,0xb2,0x35,0xfd,0x60,0xc2,0xab,0x47,0xf0,0xbb,0xe0,0x0b,0x09,0x82,0x35,0x0e,0x9e,0x2a,0x11,0x44,0x3f,0x18,0x45,0xd1,0xde,0xfc,0xc0,0xf0,0x10,0xf4,0xa8,0x6c,0x80,0xad,0x6c,0xd9,0xd6,0x18,0x49,0x1f,0x65,0xf8,0x4d,0x4b,0x7e,0xbd,0xdd,0xfe,0x59,0x25,0x93,0x91,0xd9,0x64,0x9a,0x76,0x85,0x9c,0x45,0xda,0x3e,0x26,0x13,0xae,0x93,0x32,0x8f,0x37,0x34,0x41,0x26,0x23,0xb3,0xe9,0x2c,0x42,0xc0,0xbc,0x5e,0x72,0x3c,0xfa,0x34,0x0a,0x87,0x11,0x4e,0xca,0xb8,0x0b,0x4c,0xab,0x71,0x3c,0xa5,0x93,0x22,0xc8,0x21,0xa4,0x64,0xa2,0xd5,0x04,0x48,0x5a,0xa5,0x0a,0xec,0xb8,0x3f,0x1e,0x3e,0x7e,0x80,0xf1,0x09,0xd0,0x2c,0x7a,0x35,0x9b,0x13,0x4b,0x64,0xc4,0x8a,0x30,0xd5,0xfb,0x95,0xd2,0x55,0xa2,0x55,0x17,0xe5,0xd9,0x19,0x27,0x60,0x4d,0xbd,0x67,0xe7,0x40,0xba,0x5c,0x07,0xc4,0x49,0x11,0xb1,0x28,0x58,0x31,0x76,0xc9,0x63,0x29,0x93,0x3e,0xa5,0xf9,0x7c,0x4e,0x13,0x1f,0x1c,0x68,0x99,0xd0,0xf7,0xef,0xb4,0x9b,0x81,0x64,0x7c,0x10,0xa6,0x84,0x35,0xdd,0x77,0xd3,0x0c,0x82,0xda,0xe1,0xfe,0xeb,0xe1,0xee,0xe2,0x9a,0x96,0xd6,0xd5,0x22,0x04,0xc8,0xb5,0x77,0xef,0xaa,0x38,0x2a,0xef,0x10,0xf8,0xb3,0x0a,0x77,0x95,0xf8,0xf4,0x92,0xd4,0xb9,0x2a,0xfb,0xc8,0x66,0x40,0x12,0x5a,0x3a,0x16,0xe6,0xef,0x16,0x5a,0x67,0x98,0xb1,0xb5,0xe5,0xaf,0xbd,0xf8,0x6e,0x88,0x85,0x3b,0xf2,0x7e,0xea,0x13,0x7e,0x29,0x9f,0x5b,0x6d,0x8b,0x21,0x9b,0x42,0x19,0xe1,0xb6,0xaf,0x85,0x3e,0x9a,0x25,0xe1,0xa9,0x80,0x1f,0x61,0xd3,0x52,0x58,0x41,0xd8,0x50,0x3b,0x22,0x40,0x43,0xc1,0x6d,0x82,0x25,0xe1,0x9c,0xd8,0x52,0xd1,0x2e,0x97,0x88,0x6b,0x10,0x33,0x8b,0xa3,0xde,0xde,0x69,0x2b,0xc2,0x9b,0xab,0x77,0x9d,0x01,0xb6,0x5d,0xab,0xf5,0x6c,0x98,0xea,0xc6,0x6e,0x7b,0x9f,0xe3,0x99,0x3b,0xa5,0xe5,0x27,0x29,0xaa,0x7e,0x02,0x3b,0x75,0x34,0x90,0x20,0xbf,0xb1,0x05,0x2a,0x8f,0x25,0xaa,0x93,0xb2,0x43,0x42,0x2c,0x50,0x3a,0xba,0x4e,0x97,0xec,0xcc,0xd2,0x20,0x1c,0x74,0x97,0x3a,0xe9,0x5b,0x1d,0x66,0xcf,0x5c,0x4e,0x13,0xe0,0x68,0xc6,0x43,0xc9,0x11,0x6a,0xdf,0x1b,0xf2,0x39,0xd9,0xd6,0xf5,0xdc,0x29,0x4f,0x06,0xdd,0x6b,0x25,0x9d,0xfc,0x99,0x7b,0x11,0xf8,0x1b,0xe8,0x42,0x35,0xde,0x5c,0x81,0x67,0x94,0x96,0x01,0x59,0x46,0x8d,0x05,0x9d,0xa0,0xa2,0x2e,0xa4,0xf3,0x3b,0x0a,0x0f,0xdb,0xbe,0x13,0x03,0xd7,0xfa,0x68,0xcd,0xb4,0xd8,0x06,0xf9,0x41,0x9a,0xc7,0xb0,0xe2,0x26,0x45,0xfc,0xd3,0x33,0xfa,0xb8,0x84,0xaf,0xab,0xe7,0x1f,0xf3,0x59,0x7e,0x13,0x75,0xa3,0x25,0xfd,0x72,0xf9,0x25,0x9e,0x45,0x10,0x37,0x25,0xbc,0x34,0xef,0xec,0xcb,0x19,0x1e,0xf9,0x09,0x0f,0xa9,0xee,0x56,0xc3,0xd4,0xf9,0xf9,0x74,0x14,0xe1,0x39,0x42,0x1c,0xe2,0x3b,0xe2,0xe8,0xb3,0xfa,0x32,0x92,0xf4,0xde,0x1e,0x95,0x3c,0x66,0x9b,0x08,0x56,0xd8,0xcc,0xa5,0xb6,0x5e,0x72,0xa5,0x99,0xcc,0xb0,0x02,0x85,0xb8,0x37,0x4c,0xe1,0xcb,0x1d,0x17,0x2d,0xc1,0xd3,0x05,0xb3,0xcb,0xb4,0x7b,0xd5,0x25,0x13,0x37,0xce,0x06,0x5b,0x5a,0x0d,0x46,0x9c,0xdd,0x20,0x77,0xea,0x70,0x7d,0x57,0x82,0x91,0x37,0xce,0x2c,0x16,0x72,0x69,0x5b,0x5d,0xe1,0xac,0xe9,0xa6,0x77,0x07,0x10,0x96,0x95,0x5a,0x47,0xf4,0x74,0x22,0x3a,0xb4,0xb9,0xea,0x9d,0x1f,0xcb,0xe9,0xd0,0x57,0x90,0xd5,0x53,0x04,0x87,0x33,0x5c,0x7d,0x6b,0xea,0xf3,0x39,0x39,0x0d,0x38,0xd7,0xdd,0x26,0x3f,0x9c,0x44,0xa3,0xb8,0x90,0x73,0xe7,0x58,0xa5,0x7c,0xe4,0x30,0x22,0x3d,0x91,0xd4,0x40,0x3a,0x08,0xfb,0x19,0x80,0x87,0xb8,0x24,0x14,0x17,0xc8,0xb7,0x4d,0x63,0x1d,0xf7,0xa8,0x62,0x4b,0x5b,0x26,0x66,0xa0,0x81,0xe1,0x06,0x00,0xf4,0xf8,0x3c,0xeb,0x8f,0x4e,0x9c,0xac,0xd9,0xf0,0x5f,0x22,0xeb,0xfe,0x5a,0xfc,0x00,0xb8,0x02,0x65,0x28,0x61,0x08,0x00,0x00
 };
#endif

#if WEBSOCKETS
using namespace websockets;
WebsocketsServer WSserver;
#endif

#if ASYNCWEBSERVER
AsyncWebServer webserver(80);
#endif

#if MQTT
std::string sentTopic = "data";
std::string receivedTopic = "command";

MqttBroker broker(1883);
MqttClient myClient(&broker);

void receivedMessage(const MqttClient* source, const Topic& topic, const char* payload, size_t length)
{
  Serial << endl << "Received message on topic " << receivedTopic.c_str() << " with payload " << payload << endl;
  processData();
}

#endif

#if WEBSOCKETS
void handle_message(WebsocketsMessage msg) {
  
  Serial.print("Message Received from javascript websocket client: ");
  Serial.println(msg.data());
  processData();
}
#endif

void setup()
{
  Serial.begin(115200);
  delay(500);

  Serial << "Starting WiFi connection......." << endl;

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial << '-';
    delay(500);
  }

  Serial << "Connected to " << ssid << " IP address: " << WiFi.localIP() << endl;

#if ASYNCWEBSERVER
    webserver.on("/", HTTP_GET, [](AsyncWebServerRequest * request) 
    {
    AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", index_html_gz, sizeof(index_html_gz));
    response->addHeader("Content-Encoding", "gzip");
    request->send(response);
    });
    
    webserver.begin();
#endif
    
#if WEBSOCKETS
    WSserver.listen(8084);
#endif

#if MQTT
    broker.begin();

  // ============= Client Subscribe ================
    myClient.setCallback(receivedMessage);
    myClient.subscribe(receivedTopic);
    myClient.subscribe(sentTopic);
#endif

#if LITTLEDB
  waitForSync();
  Serial.println("UTC: " + UTC.dateTime());
  
  Timezone America;
  America.setLocation("America/New_York");
  //America.setPosix("EST--5EDT,M3.2.0,M11.1.0/2");
  Serial.println("EST time: " + America.dateTime());
  Serial.println("Time now is:" + America.dateTime("l ~t~h~e jS ~o~f F Y, g:i A") );
                                          //Saturday the 25th of August 2018, 2:23 PM
  Serial.println(" " + America.dateTime("mdyHi") );                                       
  Timestamp = " " + America.dateTime("mdyHi");
  Serial.println(" " + America.dateTime("mdy") );                                       
  Date = " " + America.dateTime("mdy");
  Serial.println(" " + America.dateTime("Hi") );                                       
  Time = " " + America.dateTime("Hi");
  
  delay(1000);
  LITTLEFS.begin(true);
 //LITTLEFS.format();

  int8_t res;


  res = execQuery("create db test_db");
  res = execQuery("use db test_db");
  
  String schem = "timestamp id, date int, time int, location text, sensorData text, deviceData text";
  schem.trim();
  res = execQuery("create table test_tbl (" + schem + ")");
  

  String tblPath = "/test_db/test_tbl";
  String schemPath = "/test_db/s.test_tbl";
  
  File schemFile = LITTLEFS.open(schemPath);
  
  

  String schemFromFile = schemFile.readString();
  schemFromFile.trim();
  schemFile.close();

  String insert = "insert into test_tbl values (";
  insert += Timestamp;
  insert += ",";
  insert += Date;
  insert += ",";
  insert += Time;
  insert += ", ";
  insert += device;
  insert += ", ";
  insert += sensorData;
  insert += ", ";
  insert += deviceData;
  insert += ")";
  
  res = execQuery(insert);
  
  insert = "insert into test_tbl values (";
  insert += Timestamp;
  insert += ",";
  insert += Date;
  insert += ",";
  insert += Time;
  insert += ", ";
  insert += device;
  insert += ", ";
  insert += sensorData;
  insert += ", ";
  insert += deviceData;
  insert += ")";
  
  res = execQuery(insert);

  insert = "insert into test_tbl values (";
  insert += Timestamp;
  insert += ",";
  insert += Date;
  insert += ",";
  insert += Time;
  insert += ", ";
  insert += device;
  insert += ", ";
  insert += sensorData;
  insert += ", ";
  insert += deviceData;
  insert += ")";
  
  res = execQuery(insert);

  insert = "insert into test_tbl values (";
  insert += Timestamp;
  insert += ",";
  insert += Date;
  insert += ",";
  insert += Time;
  insert += ", ";
  insert += device;
  insert += ", ";
  insert += sensorData;
  insert += ", ";
  insert += deviceData;
  insert += ")";
  
  res = execQuery(insert);
  
  
/*
  Serial.println("=======================================");
  String id = "0515211250";
  String select = "select from test_tbl where id=";
  select = select + id;
  res = execQuery(select);
  getData();
  Serial.println("=======================================");
  


  Serial.println("=======================================");
  int room = "25";
  select = "select from test_tbl where location =";
  select = select + room;
  
  execQuery(select);
  String dateofrecord = getText(selectedRows->rows[0], "date"); // == "051521");
  //getData();
  Serial.println("=======================================");
  
  Serial.println("=======================================");
  String date = "051521";
  select = "select from test_tbl where date=";
  select = select + date;
  res = execQuery(select);
  getData();
  Serial.println("=======================================");
  */
  //res = execQuery("update test_tbl set name=updated where id=1234567890abc");
  //selectRes = execQuery("select from test_tbl where id=1234567890abc");
  //res = execQuery("delete from test_tbl where id=1234567890abc");
  
  //res = execQuery("drop table test_tbl");
  
  //res = execQuery("drop db test_db");

  delay(2000);

  //res = execQuery("update test_tbl set location=livingroom where id=1234567890abc");
  //res = execQuery("delete from test_tbl where id=1234567890abc");
  
  
 
  tblPath = prefix + CONNECTED_DB + "/test_tbl";
  File tblFile = LITTLEFS.open(tblPath);
  while(tblFile.available()){
  Serial.print(tblFile.read(), DEC);
  Serial.print(" ");
  }
  Serial.println();
  listDir(LITTLEFS, "/", 2);

  getData();
  
#endif
  
}  // End of Setup


void loop()
{
#if MQTT
  broker.loop();  // Don't forget to add loop for every broker and clients
  myClient.loop();
#endif
  
#if WEBSOCKETS
  auto client = WSserver.accept();
  client.onMessage(handle_message);
  client.send("Device Data: " + deviceData);
  client.send("SensorData: " + sensorData);
  
  while (client.available()) {
  client.poll();
  }
#endif
  
 static auto next=millis();               // The next line is an efficient delay() replacement
 if (millis() > next){next += 10000;}

} // End of Loop

void processData()
{
               device = random(25,28);
               int voltage = random (260, 330);
               int temperature = random (1,110);
               int humidity = random (1,100);
               int pressure = random (1,300);
               int light = random (1,100);
               
               sensorData = ("{" + String(device) + "," + String(voltage) + "," + String(temperature) + "," + String(humidity) + "," + String(pressure) + "," + String(light) + "}");
               //Serial.println();
               //Serial.print("Received Sensor data: "); 
               //Serial.println(sensorData);
               //Serial.println();
 
               myClient.publish("Sensor", sensorData);

               device = random(25,28);
               int rssi = random (-30, -90);
               int deviceMode = random (1,3);
               int WiFiChannel = random (1,11);
               int deviceIP = device;
               int sleepTime = random (1,100);
               
               deviceData = ("{" + String(device) + "," + String(rssi) + "," + String(deviceMode) + "," + String(WiFiChannel) + "," + String(deviceIP) + "," + String(sleepTime) + "}");
               //Serial.print("Received Status data : "); 
               //Serial.println(deviceData);
               //Serial.println();
                      
               myClient.publish("Device", deviceData);
}

#if LITTLEDB
void getData()

{
  
  String query;
  int count = 5;
  int i;
  for(i=0; i< count; i++) {
    query = "insert into test_tbl values (";
    query.concat(i);
    query.concat(", 0515211250, 051521,26,");
    query.concat(i);
    //query.concat(", ");
    //query.concat(i);
    query.concat(", insert select multiple rows )");
    execQuery(query);
   }

   
    execQuery("select from test_tbl where location=insert select multiple rows");
    for(i=0; i< selectedRows->rowsLen; i++) {
    
    String dateofrecord = getText(selectedRows->rows[i], "date");
    Serial.print("Date:  "); Serial.println(dateofrecord); 
    String sensors = getText(selectedRows->rows[i], "sensorData");
    Serial.print("Date:  "); Serial.println(sensors); 
    String devices = getText(selectedRows->rows[i], "deviceData");
    Serial.print("Date:  "); Serial.println(devices); 
    int32_t rooms = getInt32(selectedRows->rows[i], "location");
    Serial.print("Date:  "); Serial.println(rooms); 
  } 
 }
 #endif
