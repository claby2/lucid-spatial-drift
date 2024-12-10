# convirt all jpg in current directory to png
import os
from PIL import Image

for file in os.listdir('.'):
    if file.endswith('.jpg'):
        img = Image.open(file)
        img.save(f'{file[:-4]}.png')


# print the name of all file under current directory
for file in os.listdir('.'):
    print(file)