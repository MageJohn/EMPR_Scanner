import numpy as np
from PIL import Image
from data.raw_data100511 import data
from os import path
from datetime import datetime

d = data[3]
for i in range(len(d)):
   if i % 2 == 1:
        d[i] = d[i][::-1]
a = np.asarray(d, dtype="uint8")
#a = a.transpose((1, 0, 2))
im = Image.fromarray(a)
now = datetime.now()
im.save(path.expanduser(f"~/Pictures/Flag{now.strftime('%H%M%S')}.png"))
