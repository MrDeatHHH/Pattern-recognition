import numpy as np
from matplotlib import pyplot as plt
import matplotlib as mpl
import imageio as im
from PIL import Image

# Function which converts rgb into gray-scale
def rgb2gray(rgb):
    return np.dot(rgb[...,:3], [0.2989, 0.5870, 0.1140])

# Defining pass variables
pathL = 'view0.png'
pathR = 'view5.png'
pathSave = 'dispar.png'
# Defining disparity and alpha
maxD = 50
alpha = 100

im1 = Image.open(pathL)

initSize = im1.size

# Downscalng image, to decrease computational time
if max(initSize) > 1000:
    
    scale = 1. / 4.
    projection = Image.BICUBIC
else:
    
    scale = 1. / 2.
    projection = Image.BILINEAR
    
# Loading and resizing Left and Right images
im2 = im1.resize((int(im1.size[0] * scale), int(im1.size[1] * scale)), Image.NEAREST)
pix = np.array(im2)
L = rgb2gray(pix)

im1 = Image.open(pathR)

im2 = im1.resize((int(im1.size[0] * scale), int(im1.size[1] * scale)), Image.NEAREST)
pix = np.array(im2)
R = rgb2gray(pix)

# Downscaling disparity, to decrease computational time
maxD = int( float(maxD) * scale) + 1

n = L.shape[0]
m = L.shape[1]
disp = np.arange(maxD + 1)

# Defining H function
def H(r, y, d):
    return (L[r][y] - R[r][(y - d) if (y - d >= 0) else 0])**2

# Precalculating g function, which will be a matrix in our case
g = np.zeros((maxD + 1, maxD + 1), dtype = float)

it = np.nditer(g, flags=['multi_index'], op_flags=['writeonly'])
while not it.finished:
    it[0] = alpha * (it.multi_index[0] - it.multi_index[1])**2
    it.iternext()

# Initializing dmap, which is our result
dmap = np.zeros(L.shape, dtype = int)
f = np.zeros((m, maxD + 1), dtype = L.dtype)

ftemp = np.zeros(maxD + 1, dtype = L.dtype)
