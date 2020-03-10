import serial
from struct import unpack

def read_crgb(ser):
    crgb = ser.read(8)
    colours = unpack("<4H", crgb)
    return colours

def crgb_2_rgb(raw):
    GAMMA = 2.2
    return tuple((int(((val/raw[0]) ** GAMMA) * 255) for val in raw[1:]))

def read_size(ser):
    size = ser.read(4)
    size = unpack("<2H", size)
    return size

if __name__ == "__main__":
    ser = serial.Serial("/dev/ttyACM0", 9600)
    size = read_size(ser)
    x = 0
    y = 0
    colour = (0,0,0)
    while True:
        for i in range(size[1]):
            colour = read_crgb
            colour = crgb_2_rgb(colour)
            y += 1
        x += 1


        
        

