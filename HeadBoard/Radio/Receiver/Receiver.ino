// RFM9x Receiver code
#include <SPI.h>
#include <RH_RF95.h>
// JSON Handler code
#include <Arduino_JSON.h>

// Define pins and frequecy
#define RFM95_CS    8
#define RFM95_INT   3
#define RFM95_RST   4

#define RF95_FREQ 915.0

// Instantiate
RH_RF95 rf95(RFM95_CS, RFM95_INT);

void setup() {
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  // Start serial
  Serial.begin(115200);
  delay(100);

  // Start radio
  Serial.println("Starting radio...");
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  while (!rf95.init()) {
    Serial.println("Radio init failed");
    while (1);
  }
  Serial.println("Radio init OK!");

  // Changes frequency from default to 915.0MHz
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);

  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then
  // you can set transmitter powers from 5 to 23 dBm:
  rf95.setTxPower(23, false);
}

void loop() {
  delay(1000); // Wait 1 second between transmits, could also 'sleep' here!
  JSONVar test_packet;
  test_packet["command"] = "R_T";
  test_packet["parameter"] = (int) 1111111;
  String test_packet_string = JSON.stringify(test_packet);
  Serial.print(test_packet_string);

  Serial.println("Transmitting basic message..."); // Send a message to rf95_server

  // char radiopacket[20] = "Basic send";
  Serial.print("Sending:"); 
  Serial.println(test_packet_string);

  delay(10);
  rf95.send((uint8_t *)test_packet_string.c_str(), test_packet_string.length());
}
