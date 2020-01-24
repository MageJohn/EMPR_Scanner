import serial
from exceptions import *

port = '/dev/ttyACM0'
baud = 9600
ser = serial.Serial(port,baud)
func_codes = {} # e.g dico = {'funcname': b'\x01'}
func_params = {} # e.g dico = {'funcname': [b'param1', b'param2', ...]}

def check_func_param_match(funcname, params):
    if (func_params[funcname] == params):
        return True
    else:
        return False
        
def remote_function_call(funcname, *args):
    func_code = func_codes[funcname]
    try:
        if (check_func_param_match(funcname,args)):
            ser.write(func_code)
            for param in args:
                ser.write(param)
        else:
            raise NoFuncParamMatch
    except:
        print("The parametres did not match the function")
  
    
