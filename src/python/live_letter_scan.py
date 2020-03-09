import ctypes
import io
from struct import pack
from threading import Thread

import PIL
import sdl2
import sdl2.ext
from serial import Serial

from letter_processing import DataProcessing
from letter_processing import parser as dp_parser

SCANNER_NEW_PIXEL = 1


class Scanner(Thread):
    def __init__(self, args, event, factory, name=None):
        super().__init__(name=name, daemon=True)
        self.args = args
        self.event = event
        self.event.user.code = SCANNER_NEW_PIXEL
        if args.from_file is not None:
            output = None
            inp = args.from_file
        else:
            output = open(args.raw_out, "wb")
            inp = Serial("/dev/ttyACM0", 9600, timeout=0.5)
            inp.write(1)
        if args.invert_gamma: 
            dp = DataProcessing(inp, args.gamma, args.integ_cycles, output, args.track)
            args.gamma = 1 / args.gamma
        self.dp = DataProcessing(inp, args.gamma, args.integ_cycles, output, args.track)
        self.size = self.dp.get_size()

        self.sprite = factory.create_sprite(size=self.size)
        self.completed_part = self.sprite.subsprite((0, 0, self.sprite.size[0], 0))

    def run(self):
        self.running = True
        array = sdl2.ext.pixels2d(self.sprite)
        for i in range(array.size):
            while self.running:
                try:
                    pix = self.dp.read_pixel()
                    array.flat[i] = int.from_bytes(
                        pack("3B", *self.dp.raw_to_rgb(pix, 'absolute')), "big"
                    )
                    sdl2.SDL_PushEvent(ctypes.byref(self.event))
                except io.BlockingIOError:
                    continue
                break
            self.completed_part = self.sprite.subsprite((0, 0, int(i / self.sprite.size[0]), self.sprite.size[1]))
            if self.running is False:
                break

    def join(self):
        self.running = False
        super().join()
        if self.dp.output is not None:
            self.dp.output.close()
        self.dp.ser.close()


def contrast_enhance(im, contrast=2.2):
    enh = PIL.ImageEnhance.Contrast(im)
    return enh.enhance(2.2)


def surface_to_pil(surf):
    bmp = io.BytesIO()
    sdl2.SDL_SaveBMP_RW(surf, sdl2.rw_from_object(bmp), False)
    return PIL.Image.open(bmp)


def run(args):
    sdl2.ext.init()
    window = sdl2.ext.Window(
        "Live scan",
        (800, 600),
        flags=(sdl2.SDL_WINDOW_SHOWN | sdl2.SDL_WINDOW_RESIZABLE),
    )

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
                    renderer.sdlrenderer, scanner.sprite.surface
                ).contents
                renderer.copy(tex)
                renderer.present()
            if event.type == sdl2.SDL_WINDOWEVENT:
                win_ev = event.window
                if win_ev.event == sdl2.SDL_WINDOWEVENT_SIZE_CHANGED:
                    renderer.clear()
                    if tex is not None:
                        renderer.copy(tex)
                    renderer.present()
            if event.type == sdl2.SDL_KEYDOWN:
                if event.key.keysym.sym == sdl2.SDLK_s:
                    im = contrast_enhance(surface_to_pil(scanner.completed_part.surface), args.contrast)
                    im.save(args.image_out)
            window.refresh()
    scanner.join()


if __name__ == "__main__":
    args = dp_parser.parse_args()
    run(args)
