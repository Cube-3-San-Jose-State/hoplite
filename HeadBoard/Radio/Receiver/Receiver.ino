// RFM9x Receiver code
#include <SPI.h>
#include <RH_RF95.h>
// JSON Handler code
#include <ArduinoJson.h>

// Define pins and frequecy
#define RFM95_CS 8
#define RFM95_INT 3
#define RFM95_RST 4

#define RF95_FREQ 915.0

// Instantiate
RH_RF95 rf95(RFM95_CS, RFM95_INT);

void setup()
{
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

  while (!rf95.init())
  {
    Serial.println("Radio init failed");
    while (1)
      ;
  }
  Serial.println("Radio init OK!");

  // Changes frequency from default to 915.0MHz
  if (!rf95.setFrequency(RF95_FREQ))
  {
    Serial.println("setFrequency failed");
    while (1)
      ;
  }
  Serial.print("Set Freq to: ");
  Serial.println(RF95_FREQ);

  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then
  // you can set transmitter powers from 5 to 23 dBm:
  rf95.setTxPower(23, false);
}

/*
  Packet Structure:
  {
    String process; // "RADIO" or "SCHEDULER" or "ASTRAEUS"
    String dir; // "INCOMING" or "OUTGOING" (with respect to CMD Handler)
    String function; //"REQ_TEL" or "REQ_PAST_HUNDRED" or "REQ_PAST_HOUR" or "SCHED_ONCE" or "SCHED_INTVL" or "SCHED_LIST" or "SCHED_CANCEL"
    String[] arguments;
  }
*/

void loop()
{
  delay(1000);

  // Create json test packet
  JsonDocument test_packet;
  String test_packet_string;
  test_packet["process"] = "ASTRAEUS";
  test_packet["dir"] = "OUTGOING";
  test_packet["function"] = "REQ_TEL";
  test_packet["arguments"][0] = "11111111";
  serializeJson(test_packet, test_packet_string);

  Serial.println(test_packet_string);

  // Send serialized packet
  Serial.print("Sending:");
  Serial.println(test_packet_string);
  rf95.send((uint8_t *)test_packet_string.c_str(), test_packet_string.length());
}
