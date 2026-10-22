#include <Arduino.h>
#include <Adafruit_TinyUSB.h>
#include "he_sensors.h"
#include "imu.h"

void setup()
{
	Serial.begin(115200);

	InitHESensors();
	InitIMU();

	Serial.println("time_ms,thumb,index,middle,pitch_deg,roll_deg");
	//Serial.println("time_ms,pitch_deg,roll_deg");
}

void loop() {
	//__WFE();

	unsigned long time = millis();

	HEData data = ReadHESensors();

	//Serial.printf("%lu,%d,%d,%d,%.2f,%.2f\n", time, data.thumb, data.index, data.middle, imu_data.pitch, imu_data.roll);

	if (UpdateIMU())
	{
		IMUData imu_data = ReadIMU();
		//Serial.printf("%lu,%.2f,%.2f\n", time, imu_data.pitch, imu_data.roll);
		Serial.printf("%lu,%d,%d,%d,%.2f,%.2f\n", time, data.thumb, data.index, data.middle, imu_data.pitch, imu_data.roll);
	}
}