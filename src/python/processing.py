from serial import Serial
from typing import Tuple
from struct import unpack
from datetime import datetime
import argparse
import numpy as np
from PIL import Image, ImageEnhance
import os
import os.path


class DataProcessing:
    def __init__(self, ser, gamma, integ_cycles=1, output=None, track=False):
        self.ser = ser
        self.absolute_max = integ_cycles * 1024
        self.gamma = gamma
        self.track = track
        self.size = None
        self.output = output
        self.image_max = -1

    def read_pixel(self):
        # one pixel is four 16 bit values
        pixel = self.ser.read(8)
        if self.output is not None:
            self.output.write(pixel)
        pixel = unpack("<4H", pixel)
        self.image_max = max(pixel[0], self.image_max)
        return pixel

    def get_size(self):
        """Should be called before most functions in this class"""
        # size is two 16 bit values
        size = self.ser.read(4)
        if self.output is not None:
            self.output.write(size)
        self.size = unpack("<2H", size)
        return self.size

    def data_reader(self):
        for pix in range(self.size[0] * self.size[1]):
            if self.track:
                print(f"\r{pix}/{self.size[0] * self.size[1] - 1}", end="")
            yield self.read_pixel()

    def read_into_array(self, max_type):
        a = list(self.data_reader())
        a = [self.raw_to_rgb(pix, max_type) for pix in a]
        a = np.array(a, dtype=np.uint8)
        return a.reshape((self.size[0], self.size[1], 3))

    def raw_to_rgb(self, raw_pix, max_type):
        if max_type == 'absolute':
            max_val = self.absolute_max
        elif max_type == 'image':
            max_val = self.image_max
        else:
            max_val = raw_pix[0]
        return tuple(
            (int(((channel/max_val) ** self.gamma) * 255) for channel in raw_pix[1:])
        )

    @staticmethod
    def makeim(data: np.ndarray,
               contrast: float,
               flip_odd: bool = True):
        if flip_odd:
            data[1::2] = np.flip(data[1::2], 1)
        im = Image.fromarray(data)
        enh = ImageEnhance.Contrast(im)
        print(contrast)
        return enh.enhance(contrast)


_timestamp = datetime.now().strftime('%H%M%S')
parser = argparse.ArgumentParser(description="Read data from serial, save the raw bytes to a file, and produce an image",
                                 formatter_class=argparse.ArgumentDefaultsHelpFormatter)
parser.add_argument("--from-file", '-f', type=argparse.FileType('rb'),
                    help="A file to read data from instead of the serial")
parser.add_argument('--raw-out', '-r', default=f'data/{_timestamp}.raw',
                    help="The file to write the raw bytes to as they're read.\
                    This file is not used if --from-file is specified.")
parser.add_argument('--image-out', default=os.path.expanduser(f'~/Pictures/Flag{_timestamp}.png'),
                    help="The location of the image file to produce.")
parser.add_argument("--max-type", '-m', choices=['absolute', 'image', 'dynamic'], default='image',
                    help="""How the maximum value for a pixel is determined. If a pixel is at this max, it's value will be 255.
                    absolute: the maximum value possible with the current number of integration cycles, set with --integ-cycles.
                    image: the maximum clear value in the image.
                    dynamic: the clear value of each pixel.""")
parser.add_argument("--integ-cycles", "-i", type=int, default=1,
                    help="The number of integration cycles being used by the scanner. Only relavant if --max-type is 'absolute'.")
parser.add_argument("--gamma", "-g", type=float, default=3.0,
                    help="The gamma to apply to the image")
parser.add_argument("--no-invert-gamma", dest="invert_gamma", action='store_false',
                    help="If specified, 1/gamma is used.")
parser.add_argument("--no-progress", dest="track", action='store_false',
                    help="Don't show the number of bytes read so far.")
parser.add_argument("--contrast", "-c", default=2.2, type=float,
                    help="The contrast adjustment to apply to the image. 1.0 is no enhancement.")
parser.add_argument("--no-row-flip", dest="row_flip", action="store_false",
                    help="Don't flip every other row.")


if __name__ == "__main__":
    args = parser.parse_args()
    if args.from_file is not None:
        output = None
        inp = args.from_file
    else:
        output = open(args.raw_out, 'wb')
        inp = Serial("/dev/ttyACM0", 9600)
        inp.write(1)
    if args.invert_gamma:
        args.gamma = 1/args.gamma
    dp = DataProcessing(inp, args.gamma, args.integ_cycles, output, args.track)
    try:
        print("Image resolution:", dp.get_size())
        im = dp.read_into_array(args.max_type)
    except Exception:
        if output is not None:
            os.remove(args.raw_out)
    finally:
        print()
        if output is not None:
            output.close()
        inp.close()
    print(f"Timestamp: {_timestamp}")
    print(f"Gamma: {args.gamma}")
    print(f"Pixel max type: {args.max_type}")
    if (args.max_type == 'absolute'):
        print(f"Max used: {dp.absolute_max} assuming {args.integ_cycles} integration cycles")
    elif (args.max_type == 'image'):
        print(f"Max used: {dp.image_max}")
    im = dp.makeim(im, args.contrast, flip_odd=args.row_flip)
    im.save(args.image_out)
