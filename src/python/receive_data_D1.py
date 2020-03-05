import serial
from struct import unpack

with serial.Serial('/dev/ttyACM0', 9600) as ser:
    #ser.write('1')
    while True:
        x_y_z = ser.read(6)
        r_g_b = ser.read(8)
        x_y_z_vals = unpack("<3H", x_y_z)
        r_g_b_vals = unpack("<4H", r_g_b)

        r_g_b_final = [int((float(chan) / 700) * 255) for chan in r_g_b_vals]
        
        print("Coord=" + str(x_y_z_vals))
        print("RGB=" + str(r_g_b_final))
        
        

