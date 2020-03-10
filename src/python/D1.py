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
    x = 0
    y = 0
    colour = (0,0,0)
    ser.write(1)
    size = read_size(ser)
    print(size)
    while True:
        for i in range(size[1]):
            colour = read_crgb(ser)
            colour = crgb_2_rgb(colour)
            y += 1
            print("Pixel: "+str((x,y))+". RGB: "+str(colour), end="\r")
        x += 1
        y = 0


        
        

