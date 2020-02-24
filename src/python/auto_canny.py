import numpy as np
import argparse
import glob
import cv2

def auto_canny(image, sigma = 0.33):
    v = np.median(image)
    lower = int(max(0, (1.0 - sigma) * v))
    upper = int(min(255, (1.0 + sigma) * v))
    edged = cv2.Canny(image, lower, upper)

    return edged

if __name__ == "__main__":
    im = cv2.imread("test_two.png")
    image = auto_canny(im)
    cv2.imwrite("edged.png", image)
   

