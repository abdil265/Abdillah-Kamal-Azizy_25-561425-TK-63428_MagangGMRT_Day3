#include <Wire.h>
#include <ESP32Servo.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

//Abdillah Kamal Azizy
//25/561425/TK/63428

Adafruit_MPU6050 mpu;

Servo servo1, servo2, servo3, servo4, servo5;

const int pirPin = 33;
bool motionDetected = false;


void setup() {
  // setup code, berjalan sekali saat start:
  Serial.begin(115200);
  Wire.begin();

  // Inisialisasi MPU6050
  if (!mpu.begin()) {
    Serial.println("MPU6050 tidak terdeteksi!");
    while (1);
  }

  // Koneksi servo ke pin
  servo1.attach(13);
  servo2.attach(12);
  servo3.attach(14);
  servo4.attach(27);
  servo5.attach(26);

  // Set posisi awal servo ke 90 (tengah)
  servo1.write(90);
  servo2.write(90);
  servo3.write(90);
  servo4.write(90);
  servo5.write(90);

  // PIR sensor
  pinMode(pirPin, INPUT);

  Serial.println("Sistem siap!");
}

void loop() {
  // loop code, berjalan terus-menerus setelah setup:
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  float roll = g.gyro.x;
  float pitch = g.gyro.y;
  float yaw = g.gyro.z;

  // Deteksi gerakan PIR 
  if (digitalRead(pirPin) == HIGH) {
    motionDetected = true;
  }

  // Jika ada gerakan eksternal 
  if (motionDetected) {
    Serial.println("Gerakan terdeteksi! Semua servo bergerak serentak...");
    for (int i = 0; i < 5; i++) {
      int pos = 45 + i * 10; // posisi bebas
      switch (i) {
        case 0: servo1.write(pos); break;
        case 1: servo2.write(pos); break;
        case 2: servo3.write(pos); break;
        case 3: servo4.write(pos); break;
        case 4: servo5.write(pos); break;
      }
    }
    delay(1000);
    // Kembali ke posisi awal
    servo1.write(90);
    servo2.write(90);
    servo3.write(90);
    servo4.write(90);
    servo5.write(90);
    delay(1000);
    motionDetected = false;
  }

  // --- Roll: servo 1 & 2 berlawanan arah ---
  if (abs(roll) > 0) {
    servo1.write(90 - roll * 25);
    servo2.write(90 - roll * 25);
  }
  
  else {
    servo1.write(90);
    servo2.write(90);
  }

  // --- Pitch: servo 3 & 4 searah ---
  if (abs(pitch) > 0) {
    servo3.write(90 + pitch * 25);
    servo4.write(90 + pitch * 25);
  }
  
  else {
    servo3.write(90);
    servo4.write(90);
  }

  // --- Yaw: servo 5 mengikuti arah rotasi ---
  if (abs(yaw) > 0) {
    servo5.write(90 + yaw * 25);
    delay(1000); //diam 1 detik
    servo5.write(90); //kembali ke posisi awal
  }

  delay(200); // mempercepat simulasi
}
