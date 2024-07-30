import board            # interfaces with rpi board ports
import busio            # interfaces with i2c on radio
import adafruit_rfm9x   # library for radio functions
import time             # used to sleep
from digitalio import DigitalInOut, Direction, Pull     # allows for digital input/output to radio
import os               # used for creating pipe
import signal
import sys
import atexit
sys.path.insert(0, os.path.abspath('../lib/IPCQueue'))
from IPCQueue import IPCQueue

'''
    Responsiblities: 
        Receives commands from ground control.
    
    To be added: 
        Send telemtry back to ground control. 
        Handle and route commands from ground control.
'''

# Connect to radio pins
CS = DigitalInOut(board.CE1)
RESET = DigitalInOut(board.D25)
spi = busio.SPI(board.SCK, MOSI=board.MOSI, MISO=board.MISO)

cmd_handler = IPCQueue("radio")

print("Radio: OK!")
# Listen for radio, send up to command handler
while True:
    # Detect local rfm9x
    rfm9x = None
    try:
        rfm9x = adafruit_rfm9x.RFM9x(spi, CS, RESET, 915.0)
    except RuntimeError as error:
        print("Radio: RFM9x not found :( Sleeping for 3 seconds:  ", error)
        time.sleep(3)

    # Check if anything needs to be sent down
    downlink_buffer = cmd_handler.read()

    # Listen for comms from radio
    packet = rfm9x.receive() or None
    if packet is None:
        pass
    else:
        packet_text = packet.decode()
        print("Radio: Received: " + packet_text + ". Sending to CMD Handler")
        cmd_handler.write(packet)
    
    time.sleep(1)
