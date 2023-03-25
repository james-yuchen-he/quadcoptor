// This module demonstrates reading PWM signals from the radio receiver

#include <EEPROM.h>

#define EEPROM_DATA_SIZE 35
#define BAUD_RATE 57600
#define NUM_RADIO_CHANNELS 4

uint32_t current_time = 0;
uint8_t setup_data[EEPROM_DATA_SIZE];
uint8_t channel_1_prev_state = LOW, channel_2_prev_state = LOW, channel_3_prev_state = LOW, channel_4_prev_state = LOW;
uint32_t channel_1_timer = 0, channel_2_timer = 0, channel_3_timer = 0, channel_4_timer = 0;
uint32_t receiver_input[NUM_RADIO_CHANNELS];

void setup() {
// put your setup code here, to run once:
  for (uint8_t i = 0; i < EEPROM_DATA_SIZE; i++)
  {
    setup_data[i] = EEPROM.read(i);
    PCICR |= (1 << PCIE0);
    PCMSK0 |= (1 << PCINT0);
    PCMSK0 |= (1 << PCINT1);
    PCMSK0 |= (1 << PCINT2);
    PCMSK0 |= (1 << PCINT3);
    Serial.begin(BAUD_RATE);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("channel_1=");
  Serial.print(receiver_input[0]);
  Serial.print(" channel_2=");
  Serial.print(receiver_input[1]);
  Serial.print(" channel_3=");
  Serial.print(receiver_input[2]);
  Serial.print(" channel_4=");
  Serial.println(receiver_input[3]);
}

ISR(PCINT0_vect)
{
  current_time = micros();

  // channel 1
  if ((PINB & (1 << PINB0)) && (channel_1_prev_state == LOW))
  {
    // a low to high transition just happened
    channel_1_timer = current_time;
    channel_1_prev_state = HIGH;
  }
  if (!(PINB & (1 << PINB0)) && (channel_1_prev_state == HIGH))
  {
    // end of a high pulse
    receiver_input[0] = current_time - channel_1_timer;
    channel_1_prev_state = LOW;
  }

  // channel 2
  if ((PINB & (1 << PINB1)) && (channel_2_prev_state == LOW))
  {
    // a low to high transition just happened
    channel_2_timer = current_time;
    channel_2_prev_state = HIGH;
  }
  if (!(PINB & (1 << PINB1)) && (channel_2_prev_state == HIGH))
  {
    // end of a high pulse
    receiver_input[1] = current_time - channel_2_timer;
    channel_2_prev_state = LOW;
  }

  // channel 3
  if ((PINB & (1 << PINB2)) && (channel_3_prev_state == LOW))
  {
    // a low to high transition just happened
    channel_3_timer = current_time;
    channel_3_prev_state = HIGH;
  }
  if (!(PINB & (1 << PINB2)) && (channel_3_prev_state == HIGH))
  {
    // end of a high pulse
    receiver_input[2] = current_time - channel_3_timer;
    channel_3_prev_state = LOW;
  }

  // channel 4
  if ((PINB & (1 << PINB3)) && (channel_4_prev_state == LOW))
  {
    // a low to high transition just happened
    channel_4_timer = current_time;
    channel_4_prev_state = HIGH;
  }
  if (!(PINB & (1 << PINB3)) && (channel_4_prev_state == HIGH))
  {
    // end of a high pulse
    receiver_input[3] = current_time - channel_4_timer;
    channel_4_prev_state = LOW;
  }
}