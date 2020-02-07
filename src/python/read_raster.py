from colours_test import compress_gamma
from serial import Serial
from struct import unpack

GAMMA = 2.2
MAX_VALUE = 0xFFFF

def main(ser):
    data = []
    data_x_y = ser.read(4)
    raw_x_y = unpack("<2H", data_x_y)
    """
    loop_no = unpack("<2H",data_x_y)[0]
    num = unpack("<2H", data_x_y)[1]
    for i in range(loop_no):
        temp = []
        raw_bytes = ser.read(2*num)
        raw_data = unpack("<"+str(num)+"H", raw_bytes)
        #masked_colour = tuple((val & MAX_VALUE for val in raw_colour))
        #colour = compress_gamma(masked_colour)
        temp.append(raw_data)
    data.append(temp)
    """
    return (raw_x_y)

if __name__ == "__main__":
    ser = Serial("/dev/ttyACM0", 9600)
    print(main(ser))


