from colours_test import compress_gamma
from serial import Serial
from struct import unpack

GAMMA = 2.2
MAX_VALUE = 0xFFFF

def main(ser):
    data = []
    
    data_x_y = ser.read(4)
    raw_x_y = unpack("<2H", data_x_y)
    
    bytes_to_read = raw_x_y[0] * (8 * raw_x_y[1])
    unpack_bytes = int(bytes_to_read/8)
    
    raw_unpack_bytes = "<"+str(unpack_bytes)+"Q"
    
    array_vals_from_scan = ser.read(bytes_to_read)
    raw_array_vals_from_scan = unpack(raw_unpack_bytes, array_vals_from_scan)

    temp = []
    for i in range(len(raw_array_vals_from_scan)):
        if len(temp) == raw_x_y[1]:
            data.append(temp)
            temp = []
        temp.append(raw_array_vals_from_scan[i])
    data.append(temp)
    print(len(raw_array_vals_from_scan))
    print(raw_array_vals_from_scan)
    return [(raw_x_y),data]

if __name__ == "__main__":
    ser = Serial("/dev/ttyACM0", 9600)
    print(main(ser))


