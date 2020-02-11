from colours_test import compress_gamma, expand_gamma, scaled_compress_gamma, scaled_expand_gamma
from serial import Serial
from struct import unpack
from pprint import pprint
from PIL import Image
import numpy as np

GAMMA = 2.2
MAX_VALUE = 0xFFFF

def main(ser):
    data = []

    data_x_y = ser.read(4)
    raw_x_y = unpack("<2H", data_x_y)

    bytes_to_read = raw_x_y[0] * (8  * raw_x_y[1])
    unpack_bytes = int(bytes_to_read/2)

    raw_unpack_bytes = "<"+str(unpack_bytes)+"H"

    array_vals_from_scan = ser.read(bytes_to_read)
    raw_array_vals_from_scan = unpack(raw_unpack_bytes, array_vals_from_scan)

    temp = []
    for i in range(0, len(raw_array_vals_from_scan), 4):
        if len(temp) == raw_x_y[1]:
            data.append(temp)
            temp = []
        temp.append((raw_array_vals_from_scan[i], raw_array_vals_from_scan[i+1], raw_array_vals_from_scan[i+2],raw_array_vals_from_scan[i+3]))
    data.append(temp)

    cg = []
    eg = []
    s_cg = []
    s_eg = []

    for row in data:
        tmp_cg = []
        tmp_eg = []
        tmp_s_cg = []
        tmp_s_eg = []
        for col in row:
            if col[0] == 0:
                tmp_cg.append((0, 0, 0))
                tmp_eg.append((0, 0, 0))
                tmp_s_cg.append((0, 0, 0))
                tmp_s_eg.append((0, 0, 0))
            else:
                tmp_cg.append(compress_gamma(col))
                tmp_eg.append(expand_gamma(col))
                tmp_s_cg.append(scaled_compress_gamma(col))
                tmp_s_eg.append(scaled_expand_gamma(col))
        cg.append(tmp_cg)
        eg.append(tmp_eg)
        s_cg.append(tmp_s_cg)
        s_eg.append(tmp_s_eg)

    return [(raw_x_y),data,cg,eg,s_cg,s_eg]


if __name__ == "__main__":
    ser = Serial("/dev/ttyACM0", 9600)
    data = main(ser)
    with open("raw_data.py", 'w') as f:
        print("data = ", file=f, end="")
        pprint(data, stream=f)
