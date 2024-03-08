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

class Pipe:
    def __init__(self, path):
        self.file = None
        self.path = path
        self.outgoing = [''] * 256
        self.incoming = [''] * 256
        
        # Create a FIFO pipe if it does not exist
        if not os.path.exists(outgoing_main.path):
            os.mkfifo(outgoing_main.path)

outgoing_main = Pipe("/tmp/radio_to_main_pipe")

CS = DigitalInOut(board.CE1)
RESET = DigitalInOut(board.D25)
spi = busio.SPI(board.SCK, MOSI=board.MOSI, MISO=board.MISO)

print("Searching for RFM9x Radio!")
with open(outgoing_main.path, "w") as pipe:
    while True:
        rfm9x = None

        try:
            rfm9x = adafruit_rfm9x.RFM9x(spi, CS, RESET, 915.0)
            print("RFM9x Detected!")
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
            pipe.write(packet_text)
            print(packet_text)
            print("Sleeping for 1 second...")
            time.sleep(1)
        
        time.sleep(0.1)
