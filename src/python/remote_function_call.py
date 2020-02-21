import serial
from errors import *

port = '/dev/ttyACM0'
baud = 9600
ser = serial.Serial(port,baud)
func_codes = {'remote_set_led': 0} # e.g {'funcname': 0}
func_params = [1] # the number at index n is the number of parameter
                  # bytes taken by the function with index n


def check_func_param_match(func_index, params):
    return len(params) == func_params[func_index]
     

def remote_function_call(funcname, *params):
    func_index = func_codes[funcname]
    if check_func_param_match(func_index, params):
        call = [0xff, func_index, *params]
        while (len(call) < 7):
            call.append(0x00)
        print(bytes(call))
        ser.write(call)

if __name__ =="__main__":
    import sys
    remote_function_call('remote_set_led', int(sys.argv[1]))
