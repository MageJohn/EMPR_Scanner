from PIL import Image
from math import sqrt
from subprocess import call
import os
import os.path

call(['python3', 'processing-dan-ind.py'])

newsize = (250, 250)

#GET KNOWN IMAGE DATA
shocked_face = Image.open('emot_detect/known_emotes/emoticon.png', 'r')
thinking_face = Image.open('emot_detect/known_emotes/thinking.png', 'r')
smile_face = Image.open('emot_detect/known_emotes/money-mouth-face.png', 'r')
cold_face = Image.open('emot_detect/known_emotes/coldemoji.png', 'r')
tongue_face = Image.open('emot_detect/known_emotes/emoji_tongue_out.png', 'r')
sick_face = Image.open('emot_detect/known_emotes/sickface.png', 'r')
angry_face = Image.open('emot_detect/known_emotes/angryemoji.png', 'r')
dissapointed_face = Image.open('emot_detect/known_emotes/hot-face.png', 'r')
laughing_face = Image.open('emot_detect/known_emotes/laughing.png', 'r')
nervous_face = Image.open('emot_detect/known_emotes/rolling.png', 'r')

#RESIZE IMAGES
shocked_face = shocked_face.resize(newsize)
thinking_face = thinking_face.resize(newsize)
smile_face = smile_face.resize(newsize)
cold_face = cold_face.resize(newsize)
tongue_face = tongue_face.resize(newsize)
sick_face = sick_face.resize(newsize)
angry_face = angry_face.resize(newsize)
dissapointed_face = dissapointed_face.resize(newsize)
laughing_face = laughing_face.resize(newsize)
nervous_face = nervous_face.resize(newsize)

#ROTATE KNOWN IMAGES TO ALLOW ROTATIONS 90 DEGREES

shocked_face_90 = shocked_face.rotate(90)
thinking_face_90 = thinking_face.rotate(90)
smile_face_90 = smile_face.rotate(90)
cold_face_90 = cold_face.rotate(90)
tongue_face_90 = tongue_face.rotate(90)
sick_face_90 = sick_face.rotate(90)
angry_face_90 = angry_face.rotate(90)
dissapointed_face_90 = dissapointed_face.rotate(90)
laughing_face_90 = laughing_face.rotate(90)
nervous_face_90 = nervous_face.rotate(90)

#ROTATE 180
shocked_face_180 = shocked_face.rotate(180)
thinking_face_180 = thinking_face.rotate(180)
smile_face_180 = smile_face.rotate(180)
cold_face_180 = cold_face.rotate(180)
tongue_face_180 = tongue_face.rotate(180)
sick_face_180 = sick_face.rotate(180)
angry_face_180 = angry_face.rotate(180)
dissapointed_face_180 = dissapointed_face.rotate(180)
laughing_face_180 = laughing_face.rotate(180)
nervous_face_180 = nervous_face.rotate(180)

#ROTATE 270
shocked_face_270 = shocked_face.rotate(270)
thinking_face_270 = thinking_face.rotate(270)
smile_face_270 = smile_face.rotate(270)
cold_face_270 = cold_face.rotate(270)
tongue_face_270 = tongue_face.rotate(270)
sick_face_270 = sick_face.rotate(270)
angry_face_270 = angry_face.rotate(270)
dissapointed_face_270 = dissapointed_face.rotate(270)
laughing_face_270 = laughing_face.rotate(270)
nervous_face_270 = nervous_face.rotate(270)

#TURN IMAGE TO PIXELS
shocked_pixels = list(shocked_face.getdata())
thinking_pixels = list(thinking_face.getdata())
smile_pixels = list(smile_face.getdata())
cold_pixels = list(cold_face.getdata())
tongue_pixels = list(tongue_face.getdata())
sick_pixels = list(sick_face.getdata())
angry_pixels = list(angry_face.getdata())
dissapointed_pixels = list(dissapointed_face.getdata())
laughing_pixels = list(laughing_face.getdata())
nervous_pixels = list(nervous_face.getdata())

shocked_90_pixels = list(shocked_face_90.getdata())
thinking_90_pixels = list(thinking_face_90.getdata())
smile_90_pixels = list(smile_face_90.getdata())
cold_90_pixels = list(cold_face_90.getdata())
tongue_90_pixels = list(tongue_face_90.getdata())
sick_90_pixels = list(sick_face_90.getdata())
angry_90_pixels = list(angry_face_90.getdata())
dissapointed_90_pixels = list(dissapointed_face_90.getdata())
laughing_90_pixels = list(laughing_face_90.getdata())
nervous_90_pixels = list(nervous_face_90.getdata())

shocked_180_pixels = list(shocked_face_180.getdata())
thinking_180_pixels = list(thinking_face_180.getdata())
smile_180_pixels = list(smile_face_180.getdata())
cold_180_pixels = list(cold_face_180.getdata())
tongue_180_pixels = list(tongue_face_180.getdata())
sick_180_pixels = list(sick_face_180.getdata())
angry_180_pixels = list(angry_face_180.getdata())
dissapointed_180_pixels = list(dissapointed_face_180.getdata())
laughing_180_pixels = list(laughing_face_180.getdata())
nervous_180_pixels = list(nervous_face_180.getdata())

shocked_270_pixels = list(shocked_face_270.getdata())
thinking_270_pixels = list(thinking_face_270.getdata())
smile_270_pixels = list(smile_face_270.getdata())
cold_270_pixels = list(cold_face_270.getdata())
tongue_270_pixels = list(tongue_face_270.getdata())
sick_270_pixels = list(sick_face_270.getdata())
angry_270_pixels = list(angry_face_270.getdata())
dissapointed_270_pixels = list(dissapointed_face_270.getdata())
laughing_270_pixels = list(laughing_face_270.getdata())
nervous_270_pixels = list(nervous_face_270.getdata())

shocked_pixel_data = []
thinking_pixel_data = []
smile_pixel_data = []
cold_pixel_data = []
tongue_pixel_data = []
sick_pixel_data = []
angry_pixel_data = []
dissapointed_pixel_data = []
laughing_pixel_data = []
nervous_pixel_data = []

shocked_90_pixel_data = []
thinking_90_pixel_data = []
smile_90_pixel_data = []
cold_90_pixel_data = []
tongue_90_pixel_data = []
sick_90_pixel_data = []
angry_90_pixel_data = []
dissapointed_90_pixel_data = []
laughing_90_pixel_data = []
nervous_90_pixel_data = []

shocked_180_pixel_data = []
thinking_180_pixel_data = []
smile_180_pixel_data = []
cold_180_pixel_data = []
tongue_180_pixel_data = []
sick_180_pixel_data = []
angry_180_pixel_data = []
dissapointed_180_pixel_data = []
laughing_180_pixel_data = []
nervous_180_pixel_data = []

shocked_270_pixel_data = []
thinking_270_pixel_data = []
smile_270_pixel_data = []
cold_270_pixel_data = []
tongue_270_pixel_data = []
sick_270_pixel_data = []
angry_270_pixel_data = []
dissapointed_270_pixel_data = []
laughing_270_pixel_data = []
nervous_270_pixel_data = []

known_data = {}

# MAKE PIXELS LISTS AND APPEND TO DICTIONARY
for pixel in shocked_pixels:
    shocked_pixel_list = list(pixel)
    if shocked_pixel_list == [0,0,0,0]:
        shocked_pixel_list = [255, 255, 255]
    if len(shocked_pixel_list) == 4:
        shocked_pixel_list.pop()
    shocked_pixel_data.append(shocked_pixel_list)

#print(shocked_pixel_data)


for pixel in thinking_pixels:
    thinking_pixel_list = list(pixel)
    if thinking_pixel_list == [0,0,0,0]:
        thinking_pixel_list = [255, 255, 255]
    if len(thinking_pixel_list) == 4:
        thinking_pixel_list.pop()
    thinking_pixel_data.append(thinking_pixel_list)

for pixel in smile_pixels:
    smile_pixel_list = list(pixel)
    if smile_pixel_list == [0,0,0,0]:
        smile_pixel_list = [255, 255, 255]
    if len(smile_pixel_list) == 4:
        smile_pixel_list.pop()
    smile_pixel_data.append(smile_pixel_list)

for pixel in cold_pixels:
    cold_pixel_list = list(pixel)
    if cold_pixel_list == [0,0,0,0]:
        cold_pixel_list = [255, 255, 255]
    if len(cold_pixel_list) == 4:
        cold_pixel_list.pop()
    cold_pixel_data.append(cold_pixel_list)

for pixel in tongue_pixels:
    tongue_pixel_list = list(pixel)
    if tongue_pixel_list == [0,0,0,0]:
        tongue_pixel_list = [255, 255, 255]
    if len(tongue_pixel_list) == 4:
        tongue_pixel_list.pop()
    tongue_pixel_data.append(tongue_pixel_list)

for pixel in sick_pixels:
    sick_pixel_list = list(pixel)
    if sick_pixel_list == [0,0,0,0]:
        sick_pixel_list = [255, 255, 255]
    if len(sick_pixel_list) == 4:
        sick_pixel_list.pop()
    sick_pixel_data.append(sick_pixel_list)

for pixel in angry_pixels:
    angry_pixel_list = list(pixel)
    if angry_pixel_list == [0,0,0,0]:
        angry_pixel_list = [255, 255, 255]
    if len(angry_pixel_list) == 4:
        angry_pixel_list.pop()
    angry_pixel_data.append(angry_pixel_list)

for pixel in dissapointed_pixels:
    dissapointed_pixel_list = list(pixel)
    if dissapointed_pixel_list == [0,0,0,0]:
        dissapointed_pixel_list = [255, 255, 255]
    if len(dissapointed_pixel_list) == 4:
        dissapointed_pixel_list.pop()
    dissapointed_pixel_data.append(dissapointed_pixel_list)

for pixel in laughing_pixels:
    laughing_pixel_list = list(pixel)
    if laughing_pixel_list == [0,0,0,0]:
        laughing_pixel_list = [255, 255, 255]
    if len(laughing_pixel_list) == 4:
        laughing_pixel_list.pop()
    laughing_pixel_data.append(laughing_pixel_list)

for pixel in nervous_pixels:
    nervous_pixel_list = list(pixel)
    if nervous_pixel_list == [0,0,0,0]:
        nervous_pixel_list = [255, 255, 255]
    if len(nervous_pixel_list) == 4:
        nervous_pixel_list.pop()
    nervous_pixel_data.append(nervous_pixel_list)

for pixel in shocked_90_pixels:
    shocked_90_pixel_list = list(pixel)
    if shocked_90_pixel_list == [0,0,0,0]:
        shocked_90_pixel_list = [255, 255, 255]
    if len(shocked_90_pixel_list) == 4:
        shocked_90_pixel_list.pop()
    shocked_90_pixel_data.append(shocked_90_pixel_list)

for pixel in thinking_90_pixels:
    thinking_90_pixel_list = list(pixel)
    if thinking_90_pixel_list == [0,0,0,0]:
        thinking_90_pixel_list = [255, 255, 255]
    if len(thinking_90_pixel_list) == 4:
        thinking_90_pixel_list.pop()
    thinking_90_pixel_data.append(thinking_90_pixel_list)

for pixel in smile_90_pixels:
    smile_90_pixel_list = list(pixel)
    if smile_90_pixel_list == [0,0,0,0]:
        smile_90_pixel_list = [255, 255, 255]
    if len(smile_90_pixel_list) == 4:
        smile_90_pixel_list.pop()
    smile_90_pixel_data.append(smile_90_pixel_list)

for pixel in cold_90_pixels:
    cold_90_pixel_list = list(pixel)
    if cold_90_pixel_list == [0,0,0,0]:
        cold_90_pixel_list = [255, 255, 255]
    if len(cold_90_pixel_list) == 4:
        cold_90_pixel_list.pop()
    cold_90_pixel_data.append(cold_90_pixel_list)

for pixel in tongue_90_pixels:
    tongue_90_pixel_list = list(pixel)
    if tongue_90_pixel_list == [0,0,0,0]:
        tongue_90_pixel_list = [255, 255, 255]
    if len(tongue_90_pixel_list) == 4:
        tongue_90_pixel_list.pop()
    tongue_90_pixel_data.append(tongue_90_pixel_list)

for pixel in sick_90_pixels:
    sick_90_pixel_list = list(pixel)
    if sick_90_pixel_list == [0,0,0,0]:
        sick_90_pixel_list = [255, 255, 255]
    if len(sick_90_pixel_list) == 4:
        sick_90_pixel_list.pop()
    sick_90_pixel_data.append(sick_90_pixel_list)

for pixel in angry_90_pixels:
    angry_90_pixel_list = list(pixel)
    if angry_90_pixel_list == [0,0,0,0]:
        angry_90_pixel_list = [255, 255, 255]
    if len(angry_90_pixel_list) == 4:
        angry_90_pixel_list.pop()
    angry_90_pixel_data.append(angry_90_pixel_list)

for pixel in dissapointed_90_pixels:
    dissapointed_90_pixel_list = list(pixel)
    if dissapointed_90_pixel_list == [0,0,0,0]:
        dissapointed_90_pixel_list = [255, 255, 255]
    if len(dissapointed_90_pixel_list) == 4:
        dissapointed_90_pixel_list.pop()
    dissapointed_90_pixel_data.append(dissapointed_90_pixel_list)

for pixel in laughing_90_pixels:
    laughing_90_pixel_list = list(pixel)
    if laughing_90_pixel_list == [0,0,0,0]:
        laughing_90_pixel_list = [255, 255, 255]
    if len(laughing_90_pixel_list) == 4:
        laughing_90_pixel_list.pop()
    laughing_90_pixel_data.append(laughing_90_pixel_list)

for pixel in nervous_90_pixels:
    nervous_90_pixel_list = list(pixel)
    if nervous_90_pixel_list == [0,0,0,0]:
        nervous_90_pixel_list = [255, 255, 255]
    if len(nervous_90_pixel_list) == 4:
        nervous_90_pixel_list.pop()
    nervous_90_pixel_data.append(nervous_90_pixel_list)

for pixel in shocked_180_pixels:
    shocked_180_pixel_list = list(pixel)
    if shocked_180_pixel_list == [0,0,0,0]:
        shocked_180_pixel_list = [255, 255, 255]
    if len(shocked_180_pixel_list) == 4:
        shocked_180_pixel_list.pop()
    shocked_180_pixel_data.append(shocked_180_pixel_list)

for pixel in thinking_180_pixels:
    thinking_180_pixel_list = list(pixel)
    if thinking_180_pixel_list == [0,0,0,0]:
        thinking_180_pixel_list = [255, 255, 255]
    if len(thinking_180_pixel_list) == 4:
        thinking_180_pixel_list.pop()
    thinking_180_pixel_data.append(thinking_180_pixel_list)

for pixel in smile_180_pixels:
    smile_180_pixel_list = list(pixel)
    if smile_180_pixel_list == [0,0,0,0]:
        smile_180_pixel_list = [255, 255, 255]
    if len(smile_180_pixel_list) == 4:
        smile_180_pixel_list.pop()
    smile_180_pixel_data.append(smile_180_pixel_list)

for pixel in cold_180_pixels:
    cold_180_pixel_list = list(pixel)
    if cold_180_pixel_list == [0,0,0,0]:
        cold_180_pixel_list = [255, 255, 255]
    if len(cold_180_pixel_list) == 4:
        cold_180_pixel_list.pop()
    cold_180_pixel_data.append(cold_180_pixel_list)

for pixel in tongue_180_pixels:
    tongue_180_pixel_list = list(pixel)
    if tongue_180_pixel_list == [0,0,0,0]:
        tongue_180_pixel_list = [255, 255, 255]
    if len(tongue_180_pixel_list) == 4:
        tongue_180_pixel_list.pop()
    tongue_180_pixel_data.append(tongue_180_pixel_list)

for pixel in sick_180_pixels:
    sick_180_pixel_list = list(pixel)
    if sick_180_pixel_list == [0,0,0,0]:
        sick_180_pixel_list = [255, 255, 255]
    if len(sick_180_pixel_list) == 4:
        sick_180_pixel_list.pop()
    sick_180_pixel_data.append(sick_180_pixel_list)

for pixel in angry_180_pixels:
    angry_180_pixel_list = list(pixel)
    if angry_180_pixel_list == [0,0,0,0]:
        angry_180_pixel_list = [255, 255, 255]
    if len(angry_180_pixel_list) == 4:
        angry_180_pixel_list.pop()
    angry_180_pixel_data.append(angry_180_pixel_list)

for pixel in dissapointed_180_pixels:
    dissapointed_180_pixel_list = list(pixel)
    if dissapointed_180_pixel_list == [0,0,0,0]:
        dissapointed_180_pixel_list = [255, 255, 255]
    if len(dissapointed_180_pixel_list) == 4:
        dissapointed_180_pixel_list.pop()
    dissapointed_180_pixel_data.append(dissapointed_180_pixel_list)

for pixel in laughing_180_pixels:
    laughing_180_pixel_list = list(pixel)
    if laughing_180_pixel_list == [0,0,0,0]:
        laughing_180_pixel_list = [255, 255, 255]
    if len(laughing_180_pixel_list) == 4:
        laughing_180_pixel_list.pop()
    laughing_180_pixel_data.append(laughing_180_pixel_list)

for pixel in nervous_180_pixels:
    nervous_180_pixel_list = list(pixel)
    if nervous_180_pixel_list == [0,0,0,0]:
        nervous_180_pixel_list = [255, 255, 255]
    if len(nervous_180_pixel_list) == 4:
        nervous_180_pixel_list.pop()
    nervous_180_pixel_data.append(nervous_180_pixel_list)

for pixel in shocked_270_pixels:
    shocked_270_pixel_list = list(pixel)
    if shocked_270_pixel_list == [0,0,0,0]:
        shocked_270_pixel_list = [255, 255, 255]
    if len(shocked_270_pixel_list) == 4:
        shocked_270_pixel_list.pop()
    shocked_270_pixel_data.append(shocked_270_pixel_list)

for pixel in thinking_270_pixels:
    thinking_270_pixel_list = list(pixel)
    if thinking_270_pixel_list == [0,0,0,0]:
        thinking_270_pixel_list = [255, 255, 255]
    if len(thinking_270_pixel_list) == 4:
        thinking_270_pixel_list.pop()
    thinking_270_pixel_data.append(thinking_270_pixel_list)

for pixel in smile_270_pixels:
    smile_270_pixel_list = list(pixel)
    if smile_270_pixel_list == [0,0,0,0]:
        smile_270_pixel_list = [255, 255, 255]
    if len(smile_270_pixel_list) == 4:
        smile_270_pixel_list.pop()
    smile_270_pixel_data.append(smile_270_pixel_list)

for pixel in cold_270_pixels:
    cold_270_pixel_list = list(pixel)
    if cold_270_pixel_list == [0,0,0,0]:
        cold_270_pixel_list = [255, 255, 255]
    if len(cold_270_pixel_list) == 4:
        cold_270_pixel_list.pop()
    cold_270_pixel_data.append(cold_270_pixel_list)

for pixel in tongue_270_pixels:
    tongue_270_pixel_list = list(pixel)
    if tongue_270_pixel_list == [0,0,0,0]:
        tongue_270_pixel_list = [255, 255, 255]
    if len(tongue_270_pixel_list) == 4:
        tongue_270_pixel_list.pop()
    tongue_270_pixel_data.append(tongue_270_pixel_list)

for pixel in sick_270_pixels:
    sick_270_pixel_list = list(pixel)
    if sick_270_pixel_list == [0,0,0,0]:
        sick_270_pixel_list = [255, 255, 255]
    if len(sick_270_pixel_list) == 4:
        sick_270_pixel_list.pop()
    sick_270_pixel_data.append(sick_270_pixel_list)

for pixel in angry_270_pixels:
    angry_270_pixel_list = list(pixel)
    if angry_270_pixel_list == [0,0,0,0]:
        angry_270_pixel_list = [255, 255, 255]
    if len(angry_270_pixel_list) == 4:
        angry_270_pixel_list.pop()
    angry_270_pixel_data.append(angry_270_pixel_list)

for pixel in dissapointed_270_pixels:
    dissapointed_270_pixel_list = list(pixel)
    if dissapointed_270_pixel_list == [0,0,0,0]:
        dissapointed_270_pixel_list = [255, 255, 255]
    if len(dissapointed_270_pixel_list) == 4:
        dissapointed_270_pixel_list.pop()
    dissapointed_270_pixel_data.append(dissapointed_270_pixel_list)

for pixel in laughing_270_pixels:
    laughing_270_pixel_list = list(pixel)
    if laughing_270_pixel_list == [0,0,0,0]:
        laughing_270_pixel_list = [255, 255, 255]
    if len(laughing_270_pixel_list) == 4:
        laughing_270_pixel_list.pop()
    laughing_270_pixel_data.append(laughing_270_pixel_list)

for pixel in nervous_270_pixels:
    nervous_270_pixel_list = list(pixel)
    if nervous_270_pixel_list == [0,0,0,0]:
        nervous_270_pixel_list = [255, 255, 255]
    if len(nervous_270_pixel_list) == 4:
        nervous_270_pixel_list.pop()
    nervous_270_pixel_data.append(nervous_270_pixel_list)

    
#BIND ALL LISTS TOGETHER
known_data['Shocked'] = shocked_pixel_data
known_data['Thinking'] = thinking_pixel_data
known_data['Money'] = smile_pixel_data
known_data['Cold'] = cold_pixel_data
known_data['Tongue'] = tongue_pixel_data
known_data['Sick'] = sick_pixel_data
known_data['Angry'] = angry_pixel_data
known_data['Hot'] = dissapointed_pixel_data
known_data['Laughing'] = laughing_pixel_data
known_data['Rolling'] = nervous_pixel_data

known_data['Shocked 90'] = shocked_90_pixel_data
known_data['Thinking 90'] = thinking_90_pixel_data
known_data['Money 90'] = smile_90_pixel_data
known_data['Cold 90'] = cold_90_pixel_data
known_data['Tongue 90'] = tongue_90_pixel_data
known_data['Sick 90'] = sick_90_pixel_data
known_data['Angry 90'] = angry_90_pixel_data
known_data['Hot 90'] = dissapointed_90_pixel_data
known_data['Laughing 90'] = laughing_90_pixel_data
known_data['Rolling 90'] = nervous_90_pixel_data

known_data['Shocked 180'] = shocked_180_pixel_data
known_data['Thinking 180'] = thinking_180_pixel_data
known_data['Money 180'] = smile_180_pixel_data
known_data['Cold 180'] = cold_180_pixel_data
known_data['Tongue 180'] = tongue_180_pixel_data
known_data['Sick 180'] = sick_180_pixel_data
known_data['Angry 180'] = angry_180_pixel_data
known_data['Hot 180'] = dissapointed_180_pixel_data
known_data['Laughing 180'] = laughing_180_pixel_data
known_data['Rolling 180'] = nervous_180_pixel_data

known_data['Shocked 270'] = shocked_270_pixel_data
known_data['Thinking 270'] = thinking_270_pixel_data
known_data['Money 270'] = smile_270_pixel_data
known_data['Cold 270'] = cold_270_pixel_data
known_data['Tongue 270'] = tongue_270_pixel_data
known_data['Sick 270'] = sick_270_pixel_data
known_data['Angry 270'] = angry_270_pixel_data
known_data['Hot 270'] = dissapointed_270_pixel_data
known_data['Laughing 270'] = laughing_270_pixel_data
known_data['Rolling 270'] = nervous_270_pixel_data

#START FUNCTIONS

#print(thinking_pixel_data)
emoji_pixel_data = []

def get_key(val): 
    for key, value in known_data.items(): 
         if val == value: 
             return key 
  
    return False

def image_to_take(im):
    emoji = Image.open(im, 'r')
    emoji = emoji.convert("RGBA")
    emoji = emoji.resize(newsize)
    emoji_pixels = list(emoji.getdata())
    #print(emoji_pixels)
    for pixel in emoji_pixels:
        emoji_pixel_list = list(pixel)
        if emoji_pixel_list == [0,0,0,0]:
            emoji_pixel_list = [255, 255, 255]
        if len(emoji_pixel_list) == 4:
            emoji_pixel_list.pop()
        emoji_pixel_data.append(emoji_pixel_list)
    return emoji_pixel_data


#K Nearest Neighbour Algorithm TAKES IN LIST FOR INPUT_IMAGE AND LIST OF LIST FOR KNOWN_IMAGES TAKES NEAREST 2 pixels either side
#and the corresponding pixel
distances = {}
full_data = []

def euclidean_distance(input_image, known_images):
    for i in known_images.values():
        indexn = 1
        indexm = -1
        indexx = 2
        indexy = -2
        key = get_key(i)
        total_distance = 0
        for input_pixel, known_pixel in zip(input_image, i):
            a = abs(input_pixel[0] - known_pixel[0])
            b = abs(input_pixel[1] - known_pixel[1])
            c = abs(input_pixel[2] - known_pixel[2])
            distance = sqrt(a**2 + b**2 + c**2)
            total_distance += distance
        for input_pixel in input_image:
            if indexn < 62500:
                a = abs(input_pixel[0] - i[indexn][0])
                b = abs(input_pixel[1] - i[indexn][1])
                c = abs(input_pixel[2] - i[indexn][2])
                distance = sqrt(a**2 + b**2 + c**2)
                total_distance += distance
                indexn += 1
        for input_pixel in input_image:
            if indexm < 62500 and indexm != -1:
                a = abs(input_pixel[0] - i[indexm][0])
                b = abs(input_pixel[1] - i[indexm][1])
                c = abs(input_pixel[2] - i[indexm][2])
                distance = sqrt(a**2 + b**2 + c**2)
                total_distance += distance
                indexm += 1

        for input_pixel in input_image:
            if indexx < 62500:
                a = abs(input_pixel[0] - i[indexx][0])
                b = abs(input_pixel[1] - i[indexx][1])
                c = abs(input_pixel[2] - i[indexx][2])
                distance = sqrt(a**2 + b**2 + c**2)
                total_distance += distance
                indexx += 1

        for input_pixel in input_image:
            if indexy < 62500 and indexy >= 0:
                a = abs(input_pixel[0] - i[indexy][0])
                b = abs(input_pixel[1] - i[indexy][1])
                c = abs(input_pixel[2] - i[indexy][2])
                distance = sqrt(a**2 + b**2 + c**2)
                total_distance += distance
                indexy += 1
        distances[key] = total_distance
    return distances
#print(emoji_pixel_data)

#CALL DETERMINE EMOJI TO GET THE PROGRAMS PREDICTION OFTHE EMOJI AND THE DATA WHICH TELLS THE FIRST GUESS SECOND ETC.

def determine_emoji(img):
    image_to_take(img)
    check_colour(emoji_pixel_data)
    euclidean_distance(emoji_pixel_data, known_data)
    in_order = {k: v for k, v in sorted(distances.items(), key=lambda item: item[1])}
    print(in_order)
    
    return min(distances, key=distances.get)


def check_colour(input_data):
    #CHECK FOR BLUE
    for pixel in input_data:
        if pixel[2] > 110 and pixel[0] < 80:
            
            del known_data['Thinking']
            del known_data['Thinking 90']
            del known_data['Thinking 180']
            del known_data['Thinking 270']
            del known_data['Money']
            del known_data['Money 90']
            del known_data['Money 180']
            del known_data['Money 270']
            del known_data['Tongue']
            del known_data['Tongue 90']
            del known_data['Tongue 180']
            del known_data['Tongue 270']
            del known_data['Sick']
            del known_data['Sick 90']
            del known_data['Sick 180']
            del known_data['Sick 270']
            del known_data['Angry']
            del known_data['Angry 90']
            del known_data['Angry 180']
            del known_data['Angry 270']
            del known_data['Rolling']
            del known_data['Rolling 90']
            del known_data['Rolling 180']
            del known_data['Rolling 270']
            return
            
        elif pixel[0] > 140 and pixel[1] < 30:
            del known_data['Thinking']
            del known_data['Thinking 90']
            del known_data['Thinking 180']
            del known_data['Thinking 270']
            del known_data['Money']
            del known_data['Money 90']
            del known_data['Money 180']
            del known_data['Money 270']
            del known_data['Cold']
            del known_data['Cold 90']
            del known_data['Cold 180']
            del known_data['Cold 270']
            del known_data['Sick']
            del known_data['Sick 90']
            del known_data['Sick 180']
            del known_data['Sick 270']
            del known_data['Laughing']
            del known_data['Laughing 90']
            del known_data['Laughing 180']
            del known_data['Laughing 270']
            del known_data['Shocked']
            del known_data['Shocked 90']
            del known_data['Shocked 180']
            del known_data['Shocked 270']
            del known_data['Rolling']
            del known_data['Rolling 90']
            del known_data['Rolling 180']
            del known_data['Rolling 270']
            return
        elif pixel[1] > 40 and pixel[0] < 20 and pixel[2] < 20:
            del known_data['Thinking']
            del known_data['Thinking 90']
            del known_data['Thinking 180']
            del known_data['Thinking 270']
            del known_data['Tongue']
            del known_data['Tongue 90']
            del known_data['Tongue 180']
            del known_data['Tongue 270']
            del known_data['Cold']
            del known_data['Cold 90']
            del known_data['Cold 180']
            del known_data['Cold 270']
            del known_data['Laughing']
            del known_data['Laughing 90']
            del known_data['Laughing 180']
            del known_data['Laughing 270']
            del known_data['Shocked']
            del known_data['Shocked 90']
            del known_data['Shocked 180']
            del known_data['Shocked 270']
            del known_data['Rolling']
            del known_data['Rolling 90']
            del known_data['Rolling 180']
            del known_data['Rolling 270']
            del known_data['Hot']
            del known_data['Hot 90']
            del known_data['Hot 180']
            del known_data['Hot 270']
            del known_data['Angry']
            del known_data['Angry 90']
            del known_data['Angry 180']
            del known_data['Angry 270']
            del known_data['Sick']
            del known_data['Sick 90']
            del known_data['Sick 180']
            del known_data['Sick 270']
            return
        elif pixel[1] > 120 and pixel[0] < 100 and pixel[2] < 70:
            del known_data['Thinking']
            del known_data['Thinking 90']
            del known_data['Thinking 180']
            del known_data['Thinking 270']
            del known_data['Tongue']
            del known_data['Tongue 90']
            del known_data['Tongue 180']
            del known_data['Tongue 270']
            del known_data['Cold']
            del known_data['Cold 90']
            del known_data['Cold 180']
            del known_data['Cold 270']
            del known_data['Laughing']
            del known_data['Laughing 90']
            del known_data['Laughing 180']
            del known_data['Laughing 270']
            del known_data['Shocked']
            del known_data['Shocked 90']
            del known_data['Shocked 180']
            del known_data['Shocked 270']
            del known_data['Rolling']
            del known_data['Rolling 90']
            del known_data['Rolling 180']
            del known_data['Rolling 270']
            del known_data['Hot']
            del known_data['Hot 90']
            del known_data['Hot 180']
            del known_data['Hot 270']
            del known_data['Angry']
            del known_data['Angry 90']
            del known_data['Angry 180']
            del known_data['Angry 270']
            del known_data['Money']
            del known_data['Money 90']
            del known_data['Money 180']
            del known_data['Money 270']
            return
    del known_data['Hot']
    del known_data['Hot 90']
    del known_data['Hot 180']
    del known_data['Hot 270']
    del known_data['Angry']
    del known_data['Angry 90']
    del known_data['Angry 180']
    del known_data['Angry 270']
    del known_data['Cold']
    del known_data['Cold 90']
    del known_data['Cold 180']
    del known_data['Cold 270']
    del known_data['Laughing']
    del known_data['Laughing 90']
    del known_data['Laughing 180']
    del known_data['Laughing 270']
    del known_data['Shocked']
    del known_data['Shocked 90']
    del known_data['Shocked 180']
    del known_data['Shocked 270']
    del known_data['Money']
    del known_data['Money 90']
    del known_data['Money 180']
    del known_data['Money 270']
    del known_data['Sick']
    del known_data['Sick 90']
    del known_data['Sick 180']
    del known_data['Sick 270']

    


##shock_test = Image.open('fearfultest.jpg', 'r')
#shock_test_90 = shock_test.rotate(270)
#shock_test_90.save('shock_test270.jpg')

image_input = default=os.path.expanduser(f'~/Pictures/outputa.png')#'emot_detect/known_emotes/sickface.png'#default=os.path.expanduser(f'~/Pictures/hot.png')
ans = determine_emoji(image_input)

if ans == 'Cold' or ans == 'Cold 90' or ans == 'Cold 180' or ans == 'Cold 270':
    print('Cold')
elif ans == 'Laughing' or ans == 'Laughing 90' or ans == 'Laughing 180' or ans == 'Laughing 270':
    print('Laughing')
elif ans == 'Shocked' or ans == 'Shocked 90' or ans == 'Shocked 180' or ans == 'Shocked 270':
    print('Shocked')
elif ans == 'Nervous' or ans == 'Nervous 90' or ans == 'Nervous 180' or ans == 'Nervous 270':
    print('Nervous')
elif ans == 'Hot' or ans == 'Hot 90' or ans == 'Hot 180' or ans == 'Hot 270':
    print('Hot')
elif ans == 'Thinking' or ans == 'Thinking 90' or ans == 'Thinking 180' or ans == 'Thinking 270':
    print('Thinking')
elif ans == 'Money' or ans == 'Money 90' or ans == 'Money 180' or ans == 'Money 270':
    print('Money')
elif ans == 'Tongue' or ans == 'Tongue 90' or ans == 'Tongue 180' or ans == 'Tongue 270':
    print('Tongue')
elif ans == 'Sick' or ans == 'Sick 90' or ans == 'Sick 180' or ans == 'Sick 270':
    print('Sick')
elif ans == 'Angry' or ans == 'Angry 90' or ans == 'Angry 180' or ans == 'Angry 270':
    print('Angry')
