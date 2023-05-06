#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>
#include <password.h>
#include <WiFiMulti.h>

WiFiMulti wifiMulti;

uint32_t updateflag =0;

// InfluxDB client instance with preconfigured InfluxCloud certificate
InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN, InfluxDbCloud2CACert);

// Data point
Point sensor("Light");

void InfluxDB_Init();
float Influxdb_Writting(int light_value);