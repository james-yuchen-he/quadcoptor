#include <EEPROM.h>
#include <Wire.h>

#define SETUP_DATA_SIZE 36
#define BAUD_RATE 57600
#define GYRO_ADDR_INDEX 32
#define LED_PIN 12
#define CAL_SAMPLES 2000
#define GYRO_RAW_2_DEG_PER_FOUR_MS 0.0000611

#define MPU6050_PWR_MGMT_1 0x6B
#define MPU6050_CONFIG 0x1A
#define MPU6050_GYRO_CONFIG 0x1B
#define MPU6050_ACCEL_CONFIG 0x1C

uint8_t setup_data[SETUP_DATA_SIZE];
int gyro_addr = 0;
uint16_t temperature;
uint32_t loop_start_mark = 0;
uint32_t calibration_sample_cnt = 0;
uint32_t loop_counter = 0;

int32_t raw_gyro_data[4];
int32_t gyro_pitch_raw = 0, gyro_roll_raw = 0, gyro_yaw_raw = 0;
double gyro_yaw_rad = 0;
double gyro_offset_raw[4];

double pitch = 0, roll = 0, yaw = 0;

int32_t raw_acc_data[4];
int32_t acc_x_raw = 0, acc_y_raw = 0, acc_z_raw = 0;
int32_t acc_total_vector = 0;
double pitch_acc = 0, roll_acc = 0;
bool first_angle = true;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(BAUD_RATE);
  Wire.begin();
  for (uint8_t i = 0; i < SETUP_DATA_SIZE; i++)
  {
    setup_data[i] = EEPROM.read(i);
  }
  gyro_addr = setup_data[GYRO_ADDR_INDEX];
  imu_init();
  validate_mpu();
  calibrate_gyro();
  loop_start_mark = micros();
}

void loop() {
  // make sure every loop lasts 4000 ms for the numerical integration to work
  // properly
  while(loop_start_mark + 4000 > micros());
  loop_start_mark = micros();

  read_imu_data();
  pitch += gyro_pitch_raw * GYRO_RAW_2_DEG_PER_FOUR_MS;
  roll += gyro_roll_raw * GYRO_RAW_2_DEG_PER_FOUR_MS;
  gyro_yaw_rad = radians(gyro_yaw_raw * GYRO_RAW_2_DEG_PER_FOUR_MS);

  //If the IMU has yawed transfer the roll angle to the pitch angel.
  pitch -= roll * sin(gyro_yaw_rad);
  //If the IMU has yawed transfer the pitch angle to the roll angel.
  roll += pitch * sin(gyro_yaw_rad);

  //Accelerometer angle calculations
  acc_total_vector = sqrt((acc_x_raw * acc_x_raw) + (acc_y_raw * acc_y_raw) + (acc_z_raw * acc_z_raw));

  pitch_acc = degrees(asin((float)acc_y_raw/acc_total_vector));                //Calculate the pitch angle.
  roll_acc = degrees(asin((float)acc_x_raw/acc_total_vector)) * -1;                //Calculate the roll angle.

  if(first_angle){
    pitch = pitch_acc;                                                 //Set the pitch angle to the accelerometer angle.
    roll = roll_acc;                                                   //Set the roll angle to the accelerometer angle.
    first_angle = false;
  }
  else{
    pitch = pitch * 0.9996 + pitch_acc * 0.0004;                 //Correct the drift of the gyro pitch angle with the accelerometer pitch angle.
    roll = roll * 0.9996 + roll_acc * 0.0004;                    //Correct the drift of the gyro roll angle with the accelerometer roll angle.
  }

  // can't print all the data at once as it takes to long and the numerical integration will be off.
  if(loop_counter == 0)Serial.print("Pitch(deg): ");
  if(loop_counter == 1)Serial.print(pitch ,2);
  if(loop_counter == 2)Serial.print(" Roll(deg): ");
  if(loop_counter == 3)Serial.print(roll ,2);
  if(loop_counter == 4)Serial.print(" Yaw(deg/sec): ");
  if(loop_counter == 5)Serial.println(gyro_yaw_raw / 65.5 ,2);

  loop_counter++;
  if(loop_counter == 60)loop_counter = 0;
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
    // Wire.beginTransmission(gyro_addr);                        //Start communication with the address found during search.
    // Wire.write(0x6B);                                            //We want to write to the PWR_MGMT_1 register (6B hex)
    // Wire.write(0x00);                                            //Set the register bits as 00000000 to activate the gyro
    // Wire.endTransmission();                                      //End the transmission with the gyro.

    // Wire.beginTransmission(gyro_addr);                        //Start communication with the address found during search.
    // Wire.write(0x1B);                                            //We want to write to the GYRO_CONFIG register (1B hex)
    // Wire.write(0x08);                                            //Set the register bits as 00001000 (500dps full scale)
    // Wire.endTransmission();                                      //End the transmission with the gyro

    // Wire.beginTransmission(gyro_addr);                        //Start communication with the address found during search.
    // Wire.write(0x1C);                                            //We want to write to the ACCEL_CONFIG register (1A hex)
    // Wire.write(0x10);                                            //Set the register bits as 00010000 (+/- 8g full scale range)
    // Wire.endTransmission();                                      //End the transmission with the gyro

    // //Let's perform a random register check to see if the values are written correct
    // Wire.beginTransmission(gyro_addr);                        //Start communication with the address found during search
    // Wire.write(0x1B);                                            //Start reading @ register 0x1B
    // Wire.endTransmission();                                      //End the transmission
    // Wire.requestFrom(gyro_addr, 1);                           //Request 1 bytes from the gyro
    // while(Wire.available() < 1);                                 //Wait until the 6 bytes are received
    // if(Wire.read() != 0x08){                                     //Check if the value is 0x08
    //   digitalWrite(12,HIGH);                                     //Turn on the warning led
    //   while(1)delay(10);                                         //Stay in this loop for ever
    // }

    // Wire.beginTransmission(gyro_addr);                        //Start communication with the address found during search
    // Wire.write(0x1A);                                            //We want to write to the CONFIG register (1A hex)
    // Wire.write(0x03);                                            //Set the register bits as 00000011 (Set Digital Low Pass Filter to ~43Hz)
    // Wire.endTransmission();                                      //End the transmission with the gyro
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
  Wire.requestFrom(gyro_addr, 14);
  while (Wire.available() < 14);

  raw_acc_data[1] = Wire.read()<<8 | Wire.read();
  raw_acc_data[2] = Wire.read()<<8 | Wire.read();
  raw_acc_data[3] = Wire.read()<<8 | Wire.read();
  temperature = Wire.read()<<8 | Wire.read();
  raw_gyro_data[1] = Wire.read()<<8 | Wire.read();
  raw_gyro_data[2] = Wire.read()<<8 | Wire.read();
  raw_gyro_data[3] = Wire.read()<<8 | Wire.read();

  //Only compensate if the calibration was performed.
  if(calibration_sample_cnt == CAL_SAMPLES)
  {
    raw_gyro_data[1] -= gyro_offset_raw[1];
    raw_gyro_data[2] -= gyro_offset_raw[2];
    raw_gyro_data[3] -= gyro_offset_raw[3];
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

void print_raw_accel()
{
  Serial.print("acc_x_raw=");
  Serial.print(acc_x_raw);
  Serial.print(" acc_y_raw= ");
  Serial.print(acc_y_raw);
  Serial.print(" acc_z_raw= ");
  Serial.println(acc_z_raw);
}

void calibrate_gyro()
{
  for (; calibration_sample_cnt < CAL_SAMPLES; calibration_sample_cnt++)
  {
    if(calibration_sample_cnt % 125 == 0)
    {
      // blink the LED to indicate calibration
      digitalWrite(12, !digitalRead(12));
    }

    read_imu_data();
    gyro_offset_raw[1] += raw_gyro_data[1];
    gyro_offset_raw[2] += raw_gyro_data[2];
    gyro_offset_raw[3] += raw_gyro_data[3];
    delay(3);
  }

  // Divide by 2000 to get the average gyro offset.
  gyro_offset_raw[1] /= CAL_SAMPLES;
  gyro_offset_raw[2] /= CAL_SAMPLES;
  gyro_offset_raw[3] /= CAL_SAMPLES;
}



