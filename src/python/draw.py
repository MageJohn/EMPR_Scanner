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
from struct import pack, unpack
from processing import DataProcessing, parser as dp_parser

def auto_canny(image, sigma = 0.33):
    v = np.median(image)
    lower = int(max(0, (1.0 - sigma) * v))
    upper = int(min(255, (1.0 + sigma) * v))
    edged = cv2.Canny(image, lower, upper)

    dim = (50,50)
    resized = cv2.resize(edged, dim, interpolation=cv2.INTER_AREA)
    cv2.imwrite("edged.png", resized)

    img = Image.open("edged.png")
    x = np.array(img, dtype="uint8")

    for r in range(len(x)):
        for c in range(len(x[0])):
            if x[r][c] > 0:
                x[r][c] = 255
            else:
                x[r][c] = 0

    s = Image.fromarray(x)
    s.save("refactored.png")

def locate_pixel_start(image_array):
    for r in range(len(image_array)):
        for c in range(len(image_array[0])):
            if image_array[r][c] > 0:
                return r,c
    return 0


def next_pixel(pixel, image_array):
    x_at = pixel[0]
    y_at = pixel[1]
    
    if x_at == len(image_array) - 1 or y_at == len(image_array[0]) -1:
        return 0
    elif image_array[x_at + 1][y_at] > 0:
        return x_at + 1, y_at
    elif image_array[x_at + 1][y_at + 1] > 0:
        return x_at + 1, y_at + 1
    elif image_array[x_at][y_at + 1] > 0:
        return x_at, y_at + 1
    else:
        return 0


class Move:
    def __init__(self):
        self.z = 400
        self.pixel = (0, 0)

    def move(self, pixel):
        self.pixel = pixel
        return self._create_cmd()

    def up(self):
        self.z = 1000
        return self._create_cmd()

    def down(self):
        self.z = 400
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

def edit_control_sequence(sequence, tolerance, up_val):
    sequence_string = ""
    sub_string = "d" * tolerance
    for move in sequence:
        if move[2] == up_val:
            sequence_string += "u"
        else:
            sequence_string += "d"
    replacement = "x" * tolerance
    out = sequence_string.replace(sub_string, replacement)
    new_sequence = []
    for i in range(len(sequence)):
        if out[i] != "d":
            new_sequence.append(sequence[i])
    return new_sequence

def send_control_sequence(movements, ser):
    send_sequence = []
    for moves in movements:
        send_sequence.append(pack("<3H", moves[0], moves[1], moves[2]))
    for seq in send_sequence:
        print(unpack("<3H", seq))
        #time.sleep(1)
        ser.write(seq)
        a = ser.read(1)

def contrast_image(image_array):
    x = image_array
    for r in range(len(x)):
        for c in range(len(x[0])):
            if x[r][c] < 255:
                x[r][c] = 0
            else:
                x[r][c] = 255

    return x

if __name__ == "__main__":
    """
    args = dp_parser.parse_args()
    ser = Serial("/dev/ttyACM0", 9600)
    dp = DataProcessing(ser)
    ser.write(1)
    dp.get_size()
    print(dp.size)
    
    im = dp.read_into_array("image")
    
    im = dp.makeim(im, args.contrast, flip_odd=False)
    im.save("test.png")
    """
    img = Image.open("test.png")
    im = np.array(img, dtype="uint8") 


    auto_canny(im, sigma = 0.33)
    """
    img = Image.open("refactored.png")
    x = np.array(img, dtype="uint8")
    seq = generate_control_sequence(x)
    new_seq = edit_control_sequence(seq,4,1000)
    inp = input("Press any key when pencil is in place: ")
    ser.write(1)
    send_control_sequence(new_seq, ser)
    """
