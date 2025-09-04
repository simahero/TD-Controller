#include <M5StickCPlus2.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <MPU6886.h>

#include "creditals.h"

MPU6886 imu;

IPAddress destinationIP(192, 168, 1, 65);
const int destinationPort = 7000;
int refreshRate = 1 / 60 * 1000;

WiFiUDP udp;

float accel_x, accel_y, accel_z;
float gyro_x, gyro_y, gyro_z;
float pitch, roll, yaw;

const float gravity = 9.81;

float velocity[3] = {0.0, 0.0, 0.0};
float displacement[3] = {0.0, 0.0, 0.0};

unsigned long previousTime = 0;

void setup()
{

    Serial.begin(9600);
    M5.begin();
    M5.Lcd.setRotation(3);
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.setTextSize(2);

    WiFi.begin(WIFI_NAME, WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        M5.Lcd.println("Connecting to WiFi.");
    }
    M5.Lcd.println("WiFi connected");
    M5.Lcd.println("");
    M5.Lcd.println("IP: 192.168.1.65");
    M5.Lcd.println("PORT: 7000");
    M5.Lcd.print("Refresh: 60fps");
    udp.begin(37799);
    imu.Init();
}

void loop()
{
    unsigned long currentTime = millis();
    float deltaTime = (currentTime - previousTime) / 1000.0;

    imu.getAccelData(&accel_x, &accel_y, &accel_z);
    imu.getGyroData(&gyro_x, &gyro_y, &gyro_z);
    imu.getAhrsData(&pitch, &roll, &yaw);

    velocity[0] += accel_x * gravity * deltaTime;
    velocity[1] += accel_y * gravity * deltaTime;
    velocity[2] += accel_z * gravity * deltaTime;

    displacement[0] += velocity[0] * deltaTime;
    displacement[1] += velocity[1] * deltaTime;
    displacement[2] += velocity[2] * deltaTime;

    String udp_data = String(accel_x) + "," +
                      String(accel_y) + "," +
                      String(accel_z) + "," +
                      String(gyro_x) + "," +
                      String(gyro_y) + "," +
                      String(gyro_z) + "," +
                      String(pitch) + "," +
                      String(roll) + "," +
                      String(yaw) + "," +
                      String(displacement[0]) + "," +
                      String(displacement[1]) + "," +
                      String(displacement[2]) + "\n";

    udp.beginPacket(destinationIP, destinationPort);
    udp.write((const uint8_t *)udp_data.c_str(), udp_data.length());
    udp.endPacket();
    // }

    delay(refreshRate);
}