from serial import Serial
import time
from struct import pack, unpack

def send_location(ser):
    x = int(input("Enter a number from 0 to 980 for x axis: "))
    y = int(input("Enter a number from 0 to 850 for y axis: "))
    z = int(input("Enter a number from 0 to 7000 for z axis: "))

    location = pack("<3H", x, y, z)
    ser.write(location)

def read_crgb(ser):
    crgb = ser.read(8)
    colours = unpack("<4H", crgb)
    return colours

def crgb_2_rgb(raw):
    GAMMA = 2.2
    return tuple((int(((val/raw[0]) ** GAMMA) * 255) for val in raw[1:]))

if __name__ == "__main__":
    ser = Serial("/dev/ttyACM0", 9600)

    while True:
        send_location(ser)
        raw = read_crgb(ser)
        print(crgb_2_rgb(raw))
