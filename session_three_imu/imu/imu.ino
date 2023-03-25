#include <EEPROM.h>
#include <Wire.h>

#define setup_data_SIZE 36
#define BAUD_RATE 57600
#define GYRO_ADDR_INDEX 32
#define LED_PIN 12
#define CAL_SAMPLES 2000

#define MPU6050_PWR_MGMT_1 0x6B
#define MPU6050_CONFIG 0x1A
#define MPU6050_GYRO_CONFIG 0x1B
#define MPU6050_ACCEL_CONFIG 0x1C

uint8_t setup_data[setup_data_SIZE];
uint8_t gyro_addr = 0;
uint16_t temperature;
uint32_t loop_start_mark = 0;
uint32_t calibration_sample_cnt = 0;

int16_t raw_gyro_data[3];
int16_t gyro_pitch_raw = 0, gyro_roll_raw = 0, gyro_yaw_raw = 0;
int32_t gyro_offset_raw[3];

int16_t raw_acc_data[3];
int16_t acc_x_raw = 0, acc_y_raw = 0, acc_z_raw = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(BAUD_RATE);
  Wire.begin();
  for (uint8_t i = 0; i < setup_data_SIZE; i++)
  {
    setup_data[i] = EEPROM.read(i);
  }
  gyro_addr = setup_data[GYRO_ADDR_INDEX];
  imu_init();
  validate_mpu();
  loop_start_mark = micros();
}

void loop() {
  // put your main code here, to run repeatedly:

}

void imu_init()
{
  // wake up the mpu
  Wire.beginTransmission(gyro_addr);
  Wire.write(MPU6050_PWR_MGMT_1);
  Wire.write(0x0);
  Wire.endTransmission(gyro_addr);

  // configure the digital low-pass filter for gyro and accel
  Wire.beginTransmission(gyro_addr);
  Wire.write(MPU6050_CONFIG);
  Wire.write(0x03);
  Wire.endTransmission(gyro_addr);

  // select the gyro full-scale range as (+/-)500 deg/sec
  Wire.beginTransmission(gyro_addr);
  Wire.write(MPU6050_GYRO_CONFIG);
  Wire.write(0x08);
  Wire.endTransmission(gyro_addr);

  // select the accelerator full-scale range as (+/-) 8g
  Wire.beginTransmission(gyro_addr);
  Wire.write(MPU6050_ACCEL_CONFIG);
  Wire.write(0x10);
  Wire.endTransmission(gyro_addr);

  Serial.println("MPU-6050 init sequence complete");
}

void validate_mpu()
{
  Wire.beginTransmission(gyro_addr);
  Wire.write(MPU6050_GYRO_CONFIG);
  Wire.endTransmission();

  // for some reason the compiler is not happy with gyro_addr as an
  // uint8_t even the API exists. Casting as a workaround.
  Wire.requestFrom((int)gyro_addr, 1);
  while (Wire.available() < 1);
  if (Wire.read() != 0x08)
  {
    Serial.println("MPU6050 setup validation failed");
    digitalWrite(LED_PIN, HIGH);
    while (1) delay(10);
  }
}

void read_imu_data()
{
  Wire.beginTransmission(gyro_addr);
  Wire.write(0x3B);
  Wire.endTransmission();
  Wire.requestFrom((int)gyro_addr, 14);
  while (Wire.available() < 14);

  raw_acc_data[0] = Wire.read()<<8 | Wire.read();                   
  raw_acc_data[1] = Wire.read()<<8 | Wire.read();
  raw_acc_data[2] = Wire.read()<<8 | Wire.read();
  temperature = Wire.read()<<8 | Wire.read();
  raw_gyro_data[0] = Wire.read()<<8 | Wire.read();
  raw_gyro_data[1] = Wire.read()<<8 | Wire.read();
  raw_gyro_data[2] = Wire.read()<<8 | Wire.read();

  //Only compensate if the calibration was performed.
  if(calibration_sample_cnt == CAL_SAMPLES)
  {
    raw_gyro_data[0] -= gyro_offset_raw[0];
    raw_gyro_data[1] -= gyro_offset_raw[1];
    raw_gyro_data[2] -= gyro_offset_raw[2];
  }

  gyro_roll_raw = raw_gyro_data[setup_data[28] & 0b00000011];           //Set gyro_roll to the correct axis that was stored in the EEPROM.
  if(setup_data[28] & 0b10000000) gyro_roll_raw *= -1;               //Invert gyro_roll if the MSB of EEPROM bit 28 is set.
  gyro_pitch_raw = raw_gyro_data[setup_data[29] & 0b00000011];          //Set gyro_pitch to the correct axis that was stored in the EEPROM.
  if(setup_data[29] & 0b10000000) gyro_pitch_raw *= -1;              //Invert gyro_pitch if the MSB of EEPROM bit 29 is set.
  gyro_yaw_raw = raw_gyro_data[setup_data[30] & 0b00000011];            //Set gyro_yaw to the correct axis that was stored in the EEPROM.
  if(setup_data[30] & 0b10000000) gyro_yaw_raw *= -1;                //Invert gyro_yaw if the MSB of EEPROM bit 30 is set.

  acc_x_raw = raw_acc_data[setup_data[29] & 0b00000011];                //Set acc_x to the correct axis that was stored in the EEPROM.
  if(setup_data[29] & 0b10000000) acc_x_raw *= -1;                   //Invert acc_x if the MSB of EEPROM bit 29 is set.
  acc_y_raw = raw_acc_data[setup_data[28] & 0b00000011];                //Set acc_y to the correct axis that was stored in the EEPROM.
  if(setup_data[28] & 0b10000000) acc_y_raw *= -1;                   //Invert acc_y if the MSB of EEPROM bit 28 is set.
  acc_z_raw = raw_acc_data[setup_data[30] & 0b00000011];                //Set acc_z to the correct axis that was stored in the EEPROM.
  if(setup_data[30] & 0b10000000) acc_z_raw *= -1;                   //Invert acc_z if the MSB of EEPROM bit 30 is set.  
}

