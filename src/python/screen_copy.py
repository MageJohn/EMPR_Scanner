import sys
import sdl2
import sdl2.ext
from random import random
from random import randint


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
            if pmaxx > self.maxx:
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
    sys.exit(run())