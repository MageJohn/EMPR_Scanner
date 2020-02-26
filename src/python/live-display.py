import os
import ctypes
from serial import Serial
from threading import Thread
import sdl2
import sdl2.ext
from processing import DataProcessing, parser as dp_parser
from struct import pack


SCANNER_NEW_PIXEL = 1


class Scanner(Thread):
    def __init__(self, args, event, factory, name=None):
        super().__init__(name=name, daemon=True)
        self.args = args
        self.event = event
        if args.from_file is not None:
            output = None
            inp = args.from_file
        else:
            output = open(args.raw_out, 'wb')
            inp = Serial("/dev/ttyACM0", 9600)
            inp.write(1)
        if args.invert_gamma:
            args.gamma = 1/args.gamma
        self.dp = DataProcessing(
            inp, args.gamma, args.integ_cycles, output, args.track
        )
        self.size = self.dp.get_size()
        print("image size:", self.size)

        self.sprite = factory.create_sprite(size=self.size)

    def run(self):
        self.running = True
        array = sdl2.ext.pixels2d(self.sprite)
        print("array dtype: ", array.dtype)
        try:
            for i, pix in enumerate(self.dp.data_reader()):
                array.flat[i] = int.from_bytes(pack("3B", *self.dp.raw_to_rgb(pix, self.args.max_type)), 'big')
                self.event.user.code = SCANNER_NEW_PIXEL
                sdl2.SDL_PushEvent(ctypes.byref(self.event))
                if self.running == False:
                    break
        except Exception as e:
            if self.dp.output is not None:
                os.remove(self.dp.output.name)
            raise e

    def join(self):
        self.running = False
        super().join()
        if self.dp.output is not None:
            self.dp.output.close()
        self.dp.ser.close()


def run(args):
    sdl2.ext.init()
    window = sdl2.ext.Window("Live scan", (800, 600),
                             flags=(sdl2.SDL_WINDOW_SHOWN
                                    | sdl2.SDL_WINDOW_RESIZABLE))

    scanner_event_type = sdl2.SDL_RegisterEvents(1)
    if scanner_event_type == -1:
        raise RuntimeError()
    scanner_event = sdl2.SDL_Event()
    scanner_event.type = scanner_event_type

    factory = sdl2.ext.SpriteFactory(sdl2.ext.SOFTWARE)
    scanner = Scanner(args, scanner_event, factory)
    scanner.start()

    renderer = sdl2.ext.Renderer(window, logical_size=scanner.sprite.size)

    window.size = renderer.logical_size[0] * 20, renderer.logical_size[1] * 20

    running = True
    tex = None
    while running:
        for event in sdl2.ext.get_events():
            if event.type == sdl2.SDL_QUIT:
                running = False
                break
            if event.type == scanner_event_type:
                tex = sdl2.SDL_CreateTextureFromSurface(
                    renderer.sdlrenderer, scanner.sprite.surface).contents
                renderer.copy(tex)
                renderer.present()
            if event.type == sdl2.SDL_WINDOWEVENT:
                win_ev = event.window
                if win_ev.event == sdl2.SDL_WINDOWEVENT_SIZE_CHANGED:
                    renderer.clear()
                    if tex is not None:
                        renderer.copy(tex)
                    renderer.present()
            window.refresh()
    scanner.join()


if __name__ == "__main__":
    args = dp_parser.parse_args()
    run(args)
