import numpy as np
import argparse
import glob
import cv2
from PIL import Image
from scipy import misc
from pprint import pprint
from struct import pack
from serial import Serial
import time
from turtle import *
from struct import pack, unpack

def locate_pixel_start(image_array):
    for r in range(len(image_array)):
        for c in range(len(image_array[0])):
            if image_array[r][c] > 0:
                return r,c
    return 0


def next_pixel(pixel, image_array):
    x_at = pixel[0]
    y_at = pixel[1]
    
    if x_at == len(image_array - 1) or y_at == len(image_array[0] - 1):
        return 0
    elif image_array[x_at][y_at - 1] > 0:
        return x_at, y_at - 1
    elif image_array[x_at][y_at + 1] > 0:
        return x_at, y_at + 1
    elif image_array[x_at - 1][y_at] > 0:
        return x_at - 1, y_at
    elif image_array[x_at + 1][y_at] > 0:
        return x_at + 1, y_at
    else:
        return 0


class Move:
    def __init__(self):
        self.z = 1100
        self.pixel = (0, 0)

    def move(self, pixel):
        self.pixel = pixel
        return self._create_cmd()

    def up(self):
        self.z = 1500
        return self._create_cmd()

    def down(self):
        self.z = 1100
        return self._create_cmd()

    def _create_cmd(self):
        return (*self.pixel, self.z)

#mbed will recieve one of three control bytes. Lift pen, drop pen and move pen.
def generate_control_sequence(image_array):
    move = Move()
    sequence = []
    pixel = locate_pixel_start(image_array)
    move.move(pixel)
    sequence.append(move.down())
    image_array[pixel] = 0

    while True:
        if next_pixel(pixel, image_array) == 0:
            sequence.append(move.up())
            pixel = locate_pixel_start(image_array)
            if pixel == 0:
                break
            sequence.append(move.move(pixel))
            sequence.append(move.down())
            image_array[pixel] = 0
        else:
            pixel = next_pixel(pixel, image_array)
            if pixel == 0:
                break
            image_array[pixel] = 0
            sequence.append(move.move(pixel))
            
    return sequence

def send_control_sequence(movements, ser):
    send_sequence = []
    for moves in movements:
        send_sequence.append(pack("<3H", moves[0], moves[1], moves[2]))
    for seq in send_sequence:
        print(unpack("<3H", seq))
        #time.sleep(1)
        ser.write(seq)
        a = ser.read(1)


if __name__ == "__main__":
    ser = Serial("/dev/ttyACM0", 9600)
    img = Image.open("refactored.png")
    x = np.array(img, dtype="uint8")
    for r in range(len(x)):
        for c in range(len(x[0])):
            if x[r][c] > 0:
                x[r][c] = 255
            else:
                x[r][c] = 0

    seq = generate_control_sequence(x)
    send_control_sequence(seq, ser)
    
    """
    for instruction in seq:
        if instruction == "down":
            pd()
        elif instruction == "up":
            pu()
        else:
            if type(instruction[0]) is not int or type(instruction[1]) is not int:
                print(instruction)
            setpos(instruction[0],instruction[1])
    done()
    """
