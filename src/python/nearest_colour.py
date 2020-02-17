from read_raster import main
from serial import Serial

flags ={("blue", "red", "blue"): "Iceland", ("blue", "white", "blue"): "Iceland", ("red", "white", "black"): "Syria", 
        ("black", "white", "red"): "Syria", ("white", "white","white"): "France", ("blue", "blue","blue"): "France",
        ("red", "red","red"): "France",("red", "white","blue"): "France", ("blue", "white","red"): "France",
        ("red","green","green"): "Burkina Faso", ("green","green","red"): "Burkina Faso", ("green","white","green"): "Burundi",
        ("red","white","red"): "Burundi",("red","white","white"): "Burundi",("green","white","white"): "Burundi", 
        ("white","white","green"): "Burundi"}

black = 15000
white = 55000
#deviance a percentage boundry -- abritrary really
#given the raw rgbc values. return 0 for white, 1 for black and 2 for colour 
def detect_shade(values, deviance):
    colours = values[1::]
    shade = values[0]
    max_col = max(colours)
    min_col = min(colours)

    if (shade > 65000 and max_col > 6500):
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
        angle_180.append(row[::-1])
        angle_90[0].append(row[0])
        angle_90[1].append(row[1])
        angle_90[2].append(row[2])
    
    angle_270.append(angle_90[::-1])

    return data, angle_90, angle_180, angle_270


if __name__ == "__main__":
    ser = Serial("/dev/ttyACM0", 9600)
    data = main(ser)
    data = data[1]
    print(data)
    print(rotations(data))
    
    flag_iden = []
    for rows in data:
        temp = []
        for cols in rows:
            temp.append(detect_shade(cols,10000))
        flag_iden.append(temp)

    print(flag_iden)
    #flag_iden = tuple(flag_iden)
    #print(flags[flag_iden])
