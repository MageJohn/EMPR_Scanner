from struct import unpack
from serial import Serial
from blessed import Terminal


GAMMA = 2.2
MAX_VALUE = 0xFFFF


def set_bg(text, rgb):
    r, g, b = rgb
    return f"\x1b[48;2;{r};{g};{b}m{text}\x1b[0m"


def scale(raw):
    return tuple((int(val/MAX_VALUE * 255) for val in raw[1:]))


def compress_gamma(raw):
    return tuple((int(((val/raw[0]) ** (1/GAMMA)) * 255) for val in raw[1:]))


def expand_gamma(raw):
    return tuple((int(((val/raw[0]) ** GAMMA) * 255) for val in raw[1:]))

def scaled_compress_gamma(raw):
    return tuple((int(((val/MAX_VALUE) ** (1/GAMMA)) * 255) for val in raw[1:]))

def scaled_expand_gamma(raw):
    return tuple((int(((val/MAX_VALUE) ** GAMMA) * 255) for val in raw[1:]))

def main(ser, term):
    ser.read_until(b"start")
    print("start")
    while True:
        raw_bytes = ser.read(8)

        raw_colour = unpack("<4H", raw_bytes)

        masked_colour = tuple((val & MAX_VALUE for val in raw_colour))
        print(tuple(hex(val) for val in raw_bytes))
        print(str(raw_colour).ljust(50))
        print(str(masked_colour).ljust(50))

        funcs = (scale, compress_gamma,
                 expand_gamma, scaled_compress_gamma,
                 scaled_expand_gamma)
        for f in funcs:
            if masked_colour[0] == 0:
                colour = (0, 0, 0)
            else:
                colour = f(masked_colour)
            print((set_bg(f"{colour}", colour) + f" {f.__name__}").ljust(50))
        print(term.move_up(len(funcs) + 3) + term.move_x(0), end="")


if __name__ == "__main__":
    ser = Serial("/dev/ttyACM0", 9600)
    term = Terminal()
    main(ser, term)
