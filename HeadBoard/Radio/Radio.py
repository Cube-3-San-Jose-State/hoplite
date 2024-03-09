import board            # interfaces with rpi board ports
import busio            # interfaces with i2c on radio
import adafruit_rfm9x   # library for radio functions
import time             # used to sleep
from digitalio import DigitalInOut, Direction, Pull     # allows for digital input/output to radio
import os               # used for creating pipe
import signal
import sys
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

class Pipe:
    def __init__(self, path):
        self.status = None
        self.path = path
        self.outgoing = [''] * 256
        self.incoming = [''] * 256
        
        # Create a FIFO pipe if it does not exist
        if not os.path.exists(self.path):
            os.mkfifo(self.path)
outgoing_main = Pipe("/tmp/radio_to_main_pipe")

# Close pipes on Ctrl-C
def signal_handler(sig, frame):
    print("\nRadio Handler: Exit detected. Closing pipes...\n")
    os.close(outgoing_main.status.fileno())
    sys.exit(0)
signal.signal(signal.SIGINT, signal_handler)

# Open Radio-CMD Handler pipe
print("Radio: waiting for CMD Handler end...")
outgoing_main.status = open(outgoing_main.path, "w")
print("Radio: CMD Handler pipe connected!")

# Listen for radio, send up to command handler
while True:
    # Detect local rfm9x
    rfm9x = None
    try:
        rfm9x = adafruit_rfm9x.RFM9x(spi, CS, RESET, 915.0)
    except RuntimeError as error:
        print("Radio: RFM9x not found :( Sleeping for 3 seconds:  ", error)
        time.sleep(3)
    
    # Listen for anything
    packet = rfm9x.receive() or None
    if packet is None:
        print("Radio: Waiting for packet...")
    else:
        packet_text = packet.decode()
        print("Radio: Received" + packet_text + ". Sending to CMD Handler")
        os.write(outgoing_main.status.fileno(), packet)
    
    time.sleep(0.1)
