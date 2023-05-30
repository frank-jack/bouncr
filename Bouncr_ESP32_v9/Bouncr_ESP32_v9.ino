/*
     vcc = 3.3v 
     Gnd = gnd
     int = 5
     scl = a5
     sda = a4
*/
const int BUZZER = 13;
// BLYNK SETUP

//Max's Blynk
/*#define BLYNK_TEMPLATE_ID "TMPL2-AKaaB7"
#define BLYNK_DEVICE_NAME "Bouncr"
#define BLYNK_AUTH_TOKEN "k8-BHDDbDrB77xaAVKYt-erj64TwJa3J"*/

//Jack's Blynk
#define BLYNK_TEMPLATE_ID "TMPL6Pulgxk1"
#define BLYNK_DEVICE_NAME "Bouncr"
#define BLYNK_AUTH_TOKEN "nNBPrdOAuy2iYulg2RK_9Dm9Ejsl4qt3"

// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial

// Libraries
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "Wire.h"
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

char auth[] = BLYNK_AUTH_TOKEN;

// your WiFi credentials.
// set password to "" for open networks.
//Lab Wifi
//char ssid[] = "HP Setup";
//char pass[] = "detkin101";
//Simon's Hotspot
//char ssid[] = "iPhone (2)";
//char pass[] = "12345678";
//Air Penn
char ssid[] = "AirPennNet-Device";
char pass[] = "penn1740wifi";

int initButtonVal = 0;
int sensButtonVal = 0;

Adafruit_MPU6050 mpu;

int16_t ax, ay, az;
int16_t gx, gy, gz;

double valax;
double valay;
double valaz;

double ix;
double iy;
double iz;
bool state;
bool ready = false;
double thresh = 0.3;
bool moved = true;

BLYNK_WRITE(V0) {
  initButtonVal = param.asInt();
  if (initButtonVal == 1) {
    Serial.println("BUTTON PRESSED");
    ix = valax;
    iy = valay;
    iz = valaz;
    ready = true;
    moved = false;

    Serial.println(ix);
    Serial.println(iy);
    Serial.println(iz);
  } else {
    Serial.println("----");
    if (ready) {
      Blynk.virtualWrite(V2, 0);
    }
  }
}

BLYNK_WRITE(V1) {
  sensButtonVal = param.asInt();
  if (sensButtonVal == 1) {
    Serial.println("HIGH SENSITIVITY");
    thresh = 0.1;
  } else {
    Serial.println("LOW SENSITIVITY");
    thresh = 0.3;
  }
}

void setup(void) {
  Serial.begin(115200);
  pinMode(BUZZER, OUTPUT);
  while (!Serial)
    delay(10);  // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit MPU6050 test!");

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
    case MPU6050_RANGE_2_G:
      Serial.println("+-2G");
      break;
    case MPU6050_RANGE_4_G:
      Serial.println("+-4G");
      break;
    case MPU6050_RANGE_8_G:
      Serial.println("+-8G");
      break;
    case MPU6050_RANGE_16_G:
      Serial.println("+-16G");
      break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
    case MPU6050_RANGE_250_DEG:
      Serial.println("+- 250 deg/s");
      break;
    case MPU6050_RANGE_500_DEG:
      Serial.println("+- 500 deg/s");
      break;
    case MPU6050_RANGE_1000_DEG:
      Serial.println("+- 1000 deg/s");
      break;
    case MPU6050_RANGE_2000_DEG:
      Serial.println("+- 2000 deg/s");
      break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
    case MPU6050_BAND_260_HZ:
      Serial.println("260 Hz");
      break;
    case MPU6050_BAND_184_HZ:
      Serial.println("184 Hz");
      break;
    case MPU6050_BAND_94_HZ:
      Serial.println("94 Hz");
      break;
    case MPU6050_BAND_44_HZ:
      Serial.println("44 Hz");
      break;
    case MPU6050_BAND_21_HZ:
      Serial.println("21 Hz");
      break;
    case MPU6050_BAND_10_HZ:
      Serial.println("10 Hz");
      break;
    case MPU6050_BAND_5_HZ:
      Serial.println("5 Hz");
      break;
  }

  Serial.println("");

  Blynk.begin(auth, ssid, pass);
  Blynk.connectWiFi(ssid, pass);
  delay(100);

}

void loop() {
  Blynk.run();

  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  valax = g.gyro.x;
  valay = g.gyro.y;
  valaz = g.gyro.z;

  /*Serial.println("INTIALS");
  Serial.println(ix);
  Serial.println(iy);
  Serial.println(iz);
  Serial.println("FINALS");
  Serial.println(valax);
  Serial.println(valay);
  Serial.println(valaz);
  Serial.println("OTHERS");
  Serial.println(thresh);
  Serial.println(ready);
  Serial.println(moved);*/


  if((valax > ix+thresh || valax < ix-thresh || valay > iy+thresh || valay < iy-thresh || valaz > iz+thresh || valaz < iz-thresh) && ready && !moved) {
    Serial.println("ALERT");
    Blynk.virtualWrite(V2, 1);
    Blynk.logEvent("notify", "ALERT: YOUR DRINK HAS BEEN TAMPERED WITH!!!");
    moved = true;
    for(int i = 0; i<3; i++){
        digitalWrite(BUZZER, HIGH);
        delay(100);
        digitalWrite(BUZZER, LOW);
        delay(100);
      }

    Serial.println("ax: ");
    Serial.println(valax);
    Serial.println("ay:");
    Serial.println(valay);
    Serial.println("az: ");
    Serial.println(valaz);
    Serial.println("");
    Serial.println("");
    Serial.println("ix: ");
    Serial.println(ix);
    Serial.println("iy: ");
    Serial.println(iy);
    Serial.println("iz: ");
    Serial.println(iz);
  }



  // TEST serial output 
  //Serial.println("x: "+valax);
  //Serial.println("y: "+valay);
  //Serial.println("z: "+valaz);
  delay(500);
}