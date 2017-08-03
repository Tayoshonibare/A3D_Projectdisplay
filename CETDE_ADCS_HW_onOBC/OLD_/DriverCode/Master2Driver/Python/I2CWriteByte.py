import smbus
import time

bus = smbus.SMBus(1)
DEVICE_ADDRESS = 0x22

time.sleep(0.5)
bus.write_byte(DEVICE_ADDRESS,0x39)

 
