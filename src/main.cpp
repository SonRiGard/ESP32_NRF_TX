/*
 * See documentation at https://nRF24.github.io/RF24
 * See License information at root directory of this library
 * Author: Brendan Doherty (2bndy5)
 */

/**
 * A simple example of sending data from 1 nRF24L01 transceiver to another.
 *
 * This example was written to be used on 2 devices acting as "nodes".
 * Use the Serial Monitor to change each node's behavior.
 */
#include <SPI.h>
#include "printf.h"
#include "RF24.h"

#define CE_PIN 2
#define CSN_PIN 4
// instantiate an object for the nRF24L01 transceiver
RF24 radio(CE_PIN, CSN_PIN);

// Let these addresses be used for the pair
uint8_t address[][6] = {"1Node", "2Node"};
// It is very helpful to think of an address as a path instead of as
// an identifying device destination

// to use different addresses on a pair of radios, we need a variable to
// uniquely identify which address this radio will use to transmit
bool radioNumber = 0; // 0 uses address[0] to transmit, 1 uses address[1] to transmit

float payload_TX[5]={1,2,3,4,5};
void NRF_Send_Data (void);
void TX_RF_Init(void);
void setup()
{
  Serial.begin(115200);
  TX_RF_Init();
  // initialize the transceiver on the SPI bus
} // setup

void loop()
{
  NRF_Send_Data();
} // loop

void NRF_Send_Data (void){
  unsigned long start_timer = micros();               // start the timer
  bool report = radio.write(&payload_TX, sizeof(payload_TX)); // transmit & save the report
  unsigned long end_timer = micros();                 // end the timer

  if (report)
  {
    Serial.print(F("Transmission successful! ")); // payload was delivered
    Serial.print(F("Time to transmit = "));
    Serial.print(end_timer - start_timer); // print the timer result
    Serial.print(F(" us. Sent: "));
    Serial.println(payload_TX[0]); // print payload sent
  }
  else
  {
    Serial.println(F("Transmission failed or timed out")); // payload was not delivered
  }
}

void TX_RF_Init(void){
    if (!radio.begin())
  {
    Serial.println(F("radio hardware is not responding!!"));
    while (1)
    {
    } // hold in infinite loop
  }
  radio.setChannel(23); 
  radio.setPALevel(RF24_PA_LOW);                   // RF24_PA_MAX is default.
  radio.setPayloadSize(20);                        // float datatype occupies 4 bytes
  radio.openWritingPipe(address[radioNumber]);     // always uses pipe 0
  radio.openReadingPipe(1, address[!radioNumber]); // using pipe 1
  // TX mode
  radio.stopListening(); // put radio in TX mode
}