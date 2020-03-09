import numpy as np
import argparse
import glob
import cv2
from PIL import Image
from scipy import misc
from pprint import pprint

def auto_canny(image, sigma = 0.33):
    v = np.median(image)
    lower = int(max(0, (1.0 - sigma) * v))
    upper = int(min(255, (1.0 + sigma) * v))
    edged = cv2.Canny(image, lower, upper)

    dim = (50,50)
    resized = cv2.resize(edged, dim, interpolation=cv2.INTER_AREA)
    cv2.imwrite("edged.png", resized)

    img = Image.open("edged.png")
    x = np.array(img, dtype="uint8")

    for r in range(len(x)):
        for c in range(len(x[0])):
            if x[r][c] > 0:
                x[r][c] = 255
            else:
                x[r][c] = 0

    s = Image.fromarray(x)
    s.save("refactored.png")

if __name__ == "__main__":
    im = cv2.imread("emoticon.png")
    auto_canny(im)