from read_raster import main
from serial import Serial
from pprint import pprint

flags ={12: "Iceland", -12: "Syria", 780: "Burkino Faso", 120: "France", 1276: "Burundi", 1116: "North Macedonia", 752: "Sudan", 
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

def hash_single_row(hash_vals, row_colours):
    r = ("red", 0)
    g = ("green", 1)
    b = ("blue", 2)
    bl = ("black", 3)
    wh = ("white", 4)

    refer = [r,g,b,bl,wh]

    hash_value = 0
    
    for colour in row_colours:
        for datum in refer:
            if colour == datum[0]:
                hash_value += hash_vals[datum[1]]
    
    return hash_value


def simple_hash(hash_vals, flag_iden_matrix):

    tmp  = []
    for rows in flag_iden_matrix:
        tmp.append(hash_single_row(hash_vals, rows))
    
    final_hash = 0
    for vals in tmp:
        final_hash += vals

    return final_hash

def closest_flag(dict, search_key):
    res = dict.get(search_key) or dict[min(dict.keys(), key = lambda key: abs(key - search_key))]
    return res


if __name__ == "__main__":
    ser = Serial("/dev/ttyACM0", 9600)
    data = main(ser)
    data = data[1]
    #print(data)
    rot = rotations(data)
    pprint(rot)
    
    flag_iden = []
    for angles in rot:
        for rows in angles:
            temp = []
            for cols in rows:
                temp.append(detect_shade(cols,10000))
            flag_iden.append(temp)

    print(flag_iden)

    print(simple_hash([-1,50,1,-10,10], flag_iden))
    search_key = simple_hash([-1,50,1,-10,10], flag_iden)
    country = closest_flag(flags, search_key)
    print(country)
    #flag_iden = tuple(flag_iden)
    #print(flags[flag_iden])
