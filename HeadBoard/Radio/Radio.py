import board            # interfaces with rpi board ports
import busio            # interfaces with i2c on radio
import adafruit_rfm9x   # library for radio functions
import time             # used to sleep
from digitalio import DigitalInOut, Direction, Pull     # allows for digital input/output to radio
import os               # used for creating pipe
'''
    Responsiblities: 
        Receives commands from ground control.
    
    To be added: 
        Send telemtry back to ground control. 
        Handle and route commands from ground control.
'''

radio_pipe = {
    # Used to talk to other processes
    "file": None,
    "path": "/tmp/main_to_radio_pipe",
    "outgoing": [''] * 80,
    "incoming": [''] * 80
}

CS = DigitalInOut(board.CE1)
RESET = DigitalInOut(board.D25)
spi = busio.SPI(board.SCK, MOSI=board.MOSI, MISO=board.MISO)

print("Searching for RFM9x Radio!")
while True:
    rfm9x = None
    try:
        rfm9x = adafruit_rfm9x.RFM9x(spi, CS, RESET, 915.0)
        print("RFM9x Detected!")

    except FileNotFoundError:
        print("named pipe doesn't exist")
    except OSError as e:
        print(f"Failed to read data from pipe: {e}")
    except RuntimeError as error:
        print("RFM9x not found :( ", error)
        print("Sleeping for 3 seconds.")
        time.sleep(3)
    
    packet = rfm9x.receive() or None
    if packet is None:
        print("Waiting for packet...")
    else:
        prev_packet = packet
        packet_text = str(prev_packet, "utf-8")
        print(packet_text)
        print("Sleeping for 1 second...")
        time.sleep(1)
    
    time.sleep(0.1)
