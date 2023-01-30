/*
 * This module demonstrates reading PWM signals from the remote control and
 * normalizing the signals to PWM waves between 1000ms to 2000ms, centered
 * around 1500ms.
 */

#include <EEPROM.h>                        //Include the EEPROM.h library so we can store information onto the EEPROM

unsigned long timer_1, timer_2, timer_3, timer_4, current_time;
byte last_channel_1, last_channel_2, last_channel_3, last_channel_4;
int receiver_input[5];
volatile int receiver_input_channel_1 = 0, receiver_input_channel_2 = 0, receiver_input_channel_3 = 0, receiver_input_channel_4 = 0;
byte eeprom_data[36];
int start;

void setup() {
    for(start = 0; start <= 35; start++)eeprom_data[start] = EEPROM.read(start);
    PCICR |= (1 << PCIE0);                                                    //Set PCIE0 to enable PCMSK0 scan.
    PCMSK0 |= (1 << PCINT0);                                                  //Set PCINT0 (digital input 8) to trigger an interrupt on state change.
    PCMSK0 |= (1 << PCINT1);                                                  //Set PCINT1 (digital input 9)to trigger an interrupt on state change.
    PCMSK0 |= (1 << PCINT2);                                                  //Set PCINT2 (digital input 10)to trigger an interrupt on state change.
    PCMSK0 |= (1 << PCINT3);                                                  //Set PCINT3 (digital input 11)to trigger an interrupt on state change.
    Serial.begin(57600);
}

void loop() {
    receiver_input_channel_1 = convert_receiver_channel(1);                 //Convert the actual receiver signals for pitch to the standard 1000 - 2000us.
    receiver_input_channel_2 = convert_receiver_channel(2);                 //Convert the actual receiver signals for roll to the standard 1000 - 2000us.
    receiver_input_channel_3 = convert_receiver_channel(3);                 //Convert the actual receiver signals for throttle to the standard 1000 - 2000us.
    receiver_input_channel_4 = convert_receiver_channel(4);                 //Convert the actual receiver signals for yaw to the standard 1000 - 2000us.

    Serial.print("channel_1=");
    Serial.print(receiver_input_channel_1);
    Serial.print(" channel_2=");
    Serial.print(receiver_input_channel_2);
    Serial.print(" channel_3=");
    Serial.print(receiver_input_channel_3);
    Serial.print(" channel_4=");
    Serial.println(receiver_input_channel_4);
}

////////////////////////////////////////////////////////////////////////////////
// This routine is called every time input 8, 9, 10 or 11 changed state.
// This is used to read the receiver signals.
// More information about this subroutine can be found in this video:
// https://youtu.be/bENjl1KQbvo
////////////////////////////////////////////////////////////////////////////////
ISR(PCINT0_vect){
    current_time = micros();
    //Channel 1=========================================
    if(PINB & B00000001){                                                     //Is input 8 high?
        if(last_channel_1 == 0){                                                //Input 8 changed from 0 to 1.
            last_channel_1 = 1;                                                   //Remember current input state.
            timer_1 = current_time;                                               //Set timer_1 to current_time.
        }
    }
    else if(last_channel_1 == 1){                                             //Input 8 is not high and changed from 1 to 0.
        last_channel_1 = 0;                                                     //Remember current input state.
        receiver_input[1] = current_time - timer_1;                             //Channel 1 is current_time - timer_1.
    }
    //Channel 2=========================================
    if(PINB & B00000010 ){                                                    //Is input 9 high?
        if(last_channel_2 == 0){                                                //Input 9 changed from 0 to 1.
            last_channel_2 = 1;                                                   //Remember current input state.
            timer_2 = current_time;                                               //Set timer_2 to current_time.
        }
    }
    else if(last_channel_2 == 1){                                             //Input 9 is not high and changed from 1 to 0.
        last_channel_2 = 0;                                                     //Remember current input state.
        receiver_input[2] = current_time - timer_2;                             //Channel 2 is current_time - timer_2.
    }
    //Channel 3=========================================
    if(PINB & B00000100 ){                                                    //Is input 10 high?
        if(last_channel_3 == 0){                                                //Input 10 changed from 0 to 1.
            last_channel_3 = 1;                                                   //Remember current input state.
            timer_3 = current_time;                                               //Set timer_3 to current_time.
        }
    }
    else if(last_channel_3 == 1){                                             //Input 10 is not high and changed from 1 to 0.
        last_channel_3 = 0;                                                     //Remember current input state.
        receiver_input[3] = current_time - timer_3;                             //Channel 3 is current_time - timer_3.
    }
    //Channel 4=========================================
    if(PINB & B00001000 ){                                                    //Is input 11 high?
        if(last_channel_4 == 0){                                                //Input 11 changed from 0 to 1.
            last_channel_4 = 1;                                                   //Remember current input state.
            timer_4 = current_time;                                               //Set timer_4 to current_time.
        }
    }
    else if(last_channel_4 == 1){                                             //Input 11 is not high and changed from 1 to 0.
        last_channel_4 = 0;                                                     //Remember current input state.
        receiver_input[4] = current_time - timer_4;                             //Channel 4 is current_time - timer_4.
    }
}

//This part converts the actual receiver signals to a standardized 1000 – 1500 – 2000 microsecond value.
//The stored data in the EEPROM is used.
int convert_receiver_channel(byte function){
  byte channel, reverse;                                                       //First we declare some local variables
  int low, center, high, actual;
  int difference;

  channel = eeprom_data[function + 23] & 0b00000111;                           //What channel corresponds with the specific function
  if(eeprom_data[function + 23] & 0b10000000)reverse = 1;                      //Reverse channel when most significant bit is set
  else reverse = 0;                                                            //If the most significant is not set there is no reverse

  actual = receiver_input[channel];                                            //Read the actual receiver value for the corresponding function
  low = (eeprom_data[channel * 2 + 15] << 8) | eeprom_data[channel * 2 + 14];  //Store the low value for the specific receiver input channel
  center = (eeprom_data[channel * 2 - 1] << 8) | eeprom_data[channel * 2 - 2]; //Store the center value for the specific receiver input channel
  high = (eeprom_data[channel * 2 + 7] << 8) | eeprom_data[channel * 2 + 6];   //Store the high value for the specific receiver input channel

  if(actual < center){                                                         //The actual receiver value is lower than the center value
    if(actual < low)actual = low;                                              //Limit the lowest value to the value that was detected during setup
    difference = ((long)(center - actual) * (long)500) / (center - low);       //Calculate and scale the actual value to a 1000 - 2000us value
    if(reverse == 1)return 1500 + difference;                                  //If the channel is reversed
    else return 1500 - difference;                                             //If the channel is not reversed
  }
  else if(actual > center){                                                                        //The actual receiver value is higher than the center value
    if(actual > high)actual = high;                                            //Limit the lowest value to the value that was detected during setup
    difference = ((long)(actual - center) * (long)500) / (high - center);      //Calculate and scale the actual value to a 1000 - 2000us value
    if(reverse == 1)return 1500 - difference;                                  //If the channel is reversed
    else return 1500 + difference;                                             //If the channel is not reversed
  }
  else return 1500;
}