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


#mbed will recieve one of three control bytes. Lift pen, drop pen and move pen.
def generate_control_sequence(image_array):
    sequence = []
    pixel = locate_pixel_start(image_array)
    sequence.append("up")
    sequence.append(pixel)
    sequence.append(("down"))
    image_array[pixel[0]][pixel[1]] = 0
    count = 0
    while count < (len(image_array) * len(image_array[0])):
        if next_pixel(pixel, image_array) == 0:
            sequence.append(("up"))
            pixel = locate_pixel_start(image_array)
            if pixel == 0:
                break
            sequence.append(pixel)
            sequence.append(("down"))
            image_array[pixel[0]][pixel[1]] = 0
        else:
            pixel = next_pixel(pixel, image_array)
            if pixel == 0:
                break
            image_array[pixel[0]][pixel[1]] = 0
            sequence.append(pixel)
        
        count += 1
    
    return sequence

def send_control_sequence():
    pass

if __name__ == "__main__":
    img = Image.open("refactored.png")
    x = np.array(img, dtype="uint8")
    for r in range(len(x)):
        for c in range(len(x[0])):
            if x[r][c] > 0:
                x[r][c] = 255
            else:
                x[r][c] = 0

    seq = generate_control_sequence(x)

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


