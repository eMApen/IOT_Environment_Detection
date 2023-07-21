#include "DBUpdate.h"

#define DEVICE "ESP32"

// InfluxDB client instance with preconfigured InfluxCloud certificate
InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN, InfluxDbCloud2CACert);


// Data point
Point sensor("Light");


uint32_t updateflag = 0;

float light_update = 0;

void InfluxDB_Init(){
    // Initialising the UI will init the display too.

    // Add tags
    sensor.addTag("device", DEVICE);
    sensor.addTag("SSID", WiFi.SSID());

    // Check server connection
    if (client.validateConnection()) {
        Serial.print("Connected to InfluxDB: ");
        Serial.println(client.getServerUrl());
    } 
    else {
        Serial.print("InfluxDB connection failed: ");
        Serial.println(client.getLastErrorMessage());
    }
}



float Influxdb_Writting(int light_value){
  //20s左右上传一次数据
  light_update = light_update + (float)light_value;
  if(updateflag == 400)
  {
    updateflag = 0;
    sensor.clearFields();
    // Store measured value into point
    // Report RSSI of currently connected network
    sensor.addField("rssi", WiFi.RSSI());
    light_update = light_update/400;
    sensor.addField("light",light_update);
    // Print what are we exactly writing
    Serial.print("Writing: ");
    Serial.println(sensor.toLineProtocol());

    // Write point
    if (!client.writePoint(sensor)) {
      Serial.print("InfluxDB write failed: ");
      Serial.println(client.getLastErrorMessage());
    }

    Serial.println("Wait 20s");
    light_update = 0;
  }
  else{
    updateflag++;
  }
  return light_update;
}