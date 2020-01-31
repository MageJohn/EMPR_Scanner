from struct import unpack
from serial import Serial


GAMMA = 2.2
MAX_VALUE = 1024


def set_bg(text, rgb):
    r, g, b = rgb
    return f"\x1b[48;2;{r};{g};{b}m{text}\x1b[0m"


def main():
    ser = Serial("/dev/ttyACM0", 9600)
    while True:
        raw_bytes = ser.read(8)

        raw_colour = unpack("<4H", raw_bytes)

        scaled = tuple((val/MAX_VALUE * 255 for val in raw_colour))

        gamma_8bit_compressed = tuple(((val/raw_colour[0] ^ (1/2.2)) * 255 for val in raw_colour[1:]))
        gamma_8bit_expanded = tuple(((val/raw_colour[0] ^ 2.2) * 255 for val in raw_colour[1:]))

        print(
            set_bg("   ", scaled[1:]),
            set_bg("   ", gamma_8bit_compressed),
            set_bg("   ", gamma_8bit_expanded),
            end="\r"
        )


if __name__ == "__main__":
    main()
