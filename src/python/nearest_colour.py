from read_raster import main
from serial import Serial
from pprint import pprint

flags ={3776: "Iceland", -12: "Syria", 14930: "Burkino Faso", 20333: "France", 18306: "Burundi", 5010679: "North Macedonia", 15563: "Sudan", 
        500: "UK", 164: "Czechia", 708: "Bulgaria", 796: "CAR", 636: "Belarus"}

black = 15000
white = 55000
#deviance a percentage boundry -- abritrary really
#given the raw rgbc values. return 0 for white, 1 for black and 2 for colour 
def detect_shade(values, deviance):
    colours = values[1::]
    shade = values[0]
    max_col = max(colours)
    min_col = min(colours)

    if (shade > 65000 and max_col > 60000):
        return "white"

    elif((min_col + deviance) >= max_col):
        if(shade <= black):
            return "black"
        else:
            return "white"
           
    
    else:
        colour = colours.index(max(colours))
        if colour == 0:
            return "red"
        elif colour == 1:
            return "green"
        else:
            return "blue"

def rotations(data):
    angle_90 = [[],[],[]]
    angle_180 = []
    angle_270 = []

    for row in data:
        angle_180.insert(0, row[::-1])
        angle_90[0].insert(0, row[0])
        angle_90[1].insert(0, row[1])
        angle_90[2].insert(0, row[2])
    
    for row in angle_90:
        angle_270.insert(0, row[::-1])

    return [data, angle_90, angle_180, angle_270]

def hash_single_row(row):
    hash_value = 0
    sub = -1
    for cols in row:
        tmp = 0
        for vals in cols:
            tmp += vals
        sub *= -1
        hash_value += (tmp * sub)
    
    return hash_value


def simple_hash(flag_iden_matrix):
    final_hash = 0
    for orientations in flag_iden_matrix:
        for row in orientations:
            final_hash += hash_single_row(row)

    return final_hash 

def closest_flag(dict, search_key):
    res = dict.get(search_key) or dict[min(dict.keys(), key = lambda key: abs(key - search_key))]
    return res


if __name__ == "__main__":
    ser = Serial("/dev/ttyACM0", 9600)
    data = main(ser)
    data = data[1]
    rot = rotations(data)
    pprint(rot)
    
    flag_iden = rot

    print(simple_hash(flag_iden))
    
    search_key = simple_hash(flag_iden)
    country = closest_flag(flags, search_key)
    
    print(country)
