#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <BlynkSimpleEsp8266.h>
#include <ArduinoJson.h>

char auth[] = "Blynk Auth Token";

char ssid[] = "WIFI SSID";
char password[] = "WIFI PW";

int pm10, pm25, pm10_criterion, pm25_criterion, Warning;

void setup()
{
	pinMode(5, OUTPUT); //D1 pin

	WiFi.begin(ssid, password);

	Blynk.begin(auth);

	if (WiFi.status() == WL_CONNECTED) {
		HTTPClient http;

		http.begin("http://openapi.airkorea.or.kr/openapi/services/rest/ArpltnInforInqireSvc/getMsrstnAcctoRltmMesureDnsty?stationName=%EB%8F%99%EC%9E%91%EB%8C%80%EB%A1%9C%20%EC%A4%91%EC%95%99%EC%B0%A8%EB%A1%9C&dataTerm=month&pageNo=1&numOfRows=1&ServiceKey=[OpenAPI Service Key]&ver=1.3&_returnType=json");
		int httpcode = http.GET();

		if (httpcode > 0) {
			String payload = http.getString();

			DynamicJsonDocument doc(2000);

			DeserializationError error = deserializeJson(doc, payload);

			pm10 = doc["list"][0]["pm10Value"];
			pm25 = doc["list"][0]["pm25Value"];
		}
		http.end();
	}
}

void loop() {
	Blynk.run();

	if ((pm10 >= pm10_criterion) || (pm25 >= pm25_criterion)) {
		digitalWrite(5, HIGH);
		Serial.println("Warning");
	}
	else {
		digitalWrite(5, LOW);
		Serial.println("Safe");
	}
	Blynk.virtualWrite(V3, pm10);
	Blynk.virtualWrite(V4, pm25);
}

BLYNK_WRITE(V1) {
	pm10_criterion = param.asInt();
}

BLYNK_WRITE(V2) {
	pm25_criterion = param.asInt();
}