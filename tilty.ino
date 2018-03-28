
#include <TinyWireM.h>
#include <DigiKeyboard.h>

const int MPU_addr=0x68;  // I2C address of the MPU-6050
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;

boolean was_turned = false;

void mpu_init() {
  TinyWireM.beginTransmission(MPU_addr);
  TinyWireM.send(0x6B);  // PWR_MGMT_1 register
  TinyWireM.send(0);     // set to zero (wakes up the MPU-6050)
  TinyWireM.endTransmission();
}

void mpu_read() {
  TinyWireM.beginTransmission(MPU_addr);
  TinyWireM.send(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  TinyWireM.endTransmission();
  TinyWireM.requestFrom(MPU_addr, 14);  // request a total of 14 registers
  AcX=TinyWireM.receive()<<8|TinyWireM.receive();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  AcY=TinyWireM.receive()<<8|TinyWireM.receive();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=TinyWireM.receive()<<8|TinyWireM.receive();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp=TinyWireM.receive()<<8|TinyWireM.receive();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX=TinyWireM.receive()<<8|TinyWireM.receive();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY=TinyWireM.receive()<<8|TinyWireM.receive();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ=TinyWireM.receive()<<8|TinyWireM.receive();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
}

void setup() {
  TinyWireM.begin();
  pinMode(1, OUTPUT); //LED on Model A
  mpu_init();
}

void loop() {
  mpu_read();
  boolean is_turned = AcX > 0;
  
  if(is_turned != was_turned) {
    was_turned = is_turned;
    DigiKeyboard.sendKeyStroke(0);
    
    if(is_turned) {
      digitalWrite(1, HIGH);
      DigiKeyboard.print("1");
    } else {
      digitalWrite(1, LOW); 
      DigiKeyboard.print("0");
    }
  }
  DigiKeyboard.delay(100);
}
