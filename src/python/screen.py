import sys
import sdl2
import sdl2.ext
from random import random
from random import randint

from colours_test import compress_gamma, expand_gamma, scaled_compress_gamma, scaled_expand_gamma
from serial import Serial
from struct import unpack
from pprint import pprint
from PIL import Image
import numpy as np
from datetime import datetime
from os import path

GAMMA = 2.2
MAX_VALUE = 0xFFFF

WHITE = sdl2.ext.Color(255, 255, 255)
BLACK = sdl2.ext.Color(0,0,0)
RED = sdl2.ext.Color(255,0,0)
pixel_size = 5
r_val = 0
g_val = 0
b_val = 0

def gen_next_value(x, y):
    for i in range(x * y):
        yield sdl2.ext.Color(i, i, i)


def get_data(ser):
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

class SoftwareRenderer(sdl2.ext.SoftwareSpriteRenderSystem):
    def __init__(self, window):
        super(SoftwareRenderer, self).__init__(window)

    def render(self, components):
        sdl2.ext.fill(self.surface, WHITE)
        super(SoftwareRenderer, self).render(components)

class Drawer(sdl2.ext.Entity):
    def __init__(self, world, sprite, posx=0, posy=0):
        self.sprite = sprite
        self.sprite.position = posx, posy
        self.velocity = Velocity()
    def newline(self):
        self.line_number = 0

class Pixel(sdl2.ext.Entity):
    def __init__(self, world, sprite, posx=0, posy=0):
        self.sprite = sprite
        self.sprite.position = posx, posy

class Velocity(object):
    def __init__(self):
        super(Velocity, self).__init__()
        self.vx = 0
        self.vy = 0


class Movement(sdl2.ext.Applicator):
    def __init__(self, minx, miny, maxx, maxy):
        super(Movement, self).__init__()
        self.componenttypes = Velocity, sdl2.ext.Sprite
        self.draw = None
        self.minx = minx
        self.miny = miny
        self.maxx = maxx
        self.maxy = maxy

    def process(self, world, componentsets):
        #for val in gen_next_value(10,10):
            #sdl2.ext.fill(sdl2.ext.Sprite, val)
        
        for velocity, sprite in componentsets:
            swidth, sheight = sprite.size
            #print('Enter RGB Value') #Option to wait for rgb sensor to give data?
            #r = input()
            #g = input()
            #b = input()
            COLOR = sdl2.ext.Color(r_val,g_val,b_val)

            #COLOR = sdl2.ext.Color(randint(0, 255), randint(0, 255), randint(0, 255)) #COLOR TO BE RECEIVED BY RGB SENSOR
            

            factory = sdl2.ext.SpriteFactory(sdl2.ext.SOFTWARE)
            prev_pixel = factory.from_color(COLOR, size=(pixel_size, pixel_size))
            Pixel(world, prev_pixel, sprite.x, sprite.y)
            
            sprite.x += velocity.vx
            sprite.y += velocity.vy
            sprite.x = max(self.minx, sprite.x)
            sprite.y = max(self.miny, sprite.y)

            pmaxx = sprite.x + swidth
            pmaxy = sprite.y + sheight
            if pmaxx > self.maxx:     #CHECK WHEN END CONDTION OF THE SCANNER HAS BEEN REACHED AND MAKE NEW LINE
                prev_pixel = factory.from_color(COLOR, size=(pixel_size, pixel_size))
                Pixel(world, prev_pixel, sprite.x, sprite.y)
                self.draw.velocity.vx = -self.draw.velocity.vx
                sprite.y += pixel_size
                
            if sprite.x <= self.minx :
                prev_pixel = factory.from_color(COLOR, size=(pixel_size, pixel_size))
                Pixel(world, prev_pixel, sprite.x, sprite.y)
                self.draw.velocity.vx = -self.draw.velocity.vx
                sprite.y += pixel_size
                
            if pmaxy > self.maxy:
                #STOP
                velocity.vx = 0
                velocity.vy = 0
            


#for val in gen_next_value(10, 10):
#    print(val)

world = sdl2.ext.World()

def rgbvals(r, g, b):
    global r_val
    r_val = r
    global g_val
    g_val = g
    global b_val
    b_val = b

def run():
    sdl2.ext.init()
    window = sdl2.ext.Window("Screen for scanner", size=(1200, 800))
    window.show()
    
    factory = sdl2.ext.SpriteFactory(sdl2.ext.SOFTWARE)
    dot = factory.from_color(BLACK, size=(pixel_size, pixel_size))

    
    movement = Movement(50, 30, 1150, 770)

    spriterenderer = SoftwareRenderer(window)
    world.add_system(movement)
    world.add_system(spriterenderer)

    

    #dot = factory.from_color(BLACK, size=(10, 10))

    draw = Drawer(world, dot, 50, 30)
    draw.velocity.vx = pixel_size

    movement.draw = draw

    running = True
    while running:
        events = sdl2.ext.get_events()
        for event in events:
            if event.type == sdl2.SDL_QUIT:
                running = False
                break
        #REMOVE BELOW DELAY AFTER TEST
        #sdl2.SDL_Delay(10)
        world.process()
        






if __name__ == "__main__":
    ser = Serial("/dev/ttyACM0", 9600)
    data = get_data(ser)
    print(data)
    sys.exit(run())
