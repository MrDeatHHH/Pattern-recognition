import numpy as np
from matplotlib import pyplot as plt
import matplotlib as mpl
import imageio as im
from PIL import Image

# Function which converts rgb into gray-scale
def rgb2gray(rgb):
    return np.dot(rgb[...,:3], [0.2989, 0.5870, 0.1140])

# Defining H function
def H(L, R, y, d):
    return (L[y] - R[(y - d) if (y - d >= 0) else 0])**2

def calculateMap(L, R, maxD, alpha):

    n = L.shape[0]
    m = L.shape[1]
    disp = np.arange(maxD + 1)

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

    # Iterating over all rows
    for i in range(n):

        if ((n > 100) and (i % int((n / 10)) == 0)):
            print('%d%%' % (i / (n / 100)))

        # Going throw all pixels in the row
        it = np.nditer(L[i], flags=['f_index'], op_flags=['readonly'])
        while not it.finished:

            # Counting all f_i(d_i)
            it1 = np.nditer(f[it.index], flags=['f_index'], op_flags=['writeonly'])
            
            if (it.index == 0):
                # For the first pixel
                while not it1.finished:
                
                    it1[0] = H(L[i], R[i], it.index, it1.index)
                    it1.iternext()
            else:
                # For other pixels
                while not it1.finished:
                    
                    it2 = np.nditer(ftemp, flags=['f_index'], op_flags=['writeonly'])
                    
                    while not it2.finished:

                        it2[0] = f[it.index - 1][it2.index] + g[it1.index][it2.index]
                        it2.iternext()
                        
                    it1[0] = H(L[i], R[i], it.index, it1.index) + ftemp.min()
                    it1.iternext()

            it.iternext()

        # Counting all d* for the row in the reverse order
        it = np.nditer(dmap[i], flags=['f_index'], op_flags=['writeonly'])
        while not it.finished:

            if (it.index == 0):

                # For the last
                it[0] = f[-1].argmin()
            else:

                # For others
                it1 = np.nditer(ftemp, flags=['f_index'], op_flags=['writeonly'])
                    
                while not it1.finished:

                    it1[0] = f[m - 1 - it.index][it1.index] + g[dmap[i][it.index - 1]][it1.index]
                    it1.iternext()

                it[0] = ftemp.argmin()

            it.iternext()

    print('100%')

    return dmap

def Test():
    L = np.array([[0, 0, 0, 255], [0, 0, 0, 255]])
    R = np.array([[0, 255, 0, 0], [0, 255, 0, 0]])
    maxD = 2
    alpha = 1
    dmap = calculateMap(L, R, maxD, alpha)
    assert (np.all(dmap == 2)), 'Not passed'
    print("Test passed")

Test()

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

if max(initSize) <= 250:

    scale = 1.
    
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

dmap = calculateMap(L, R, maxD, alpha)

# Upscaling image and flipping dmap along the second axis
imout = Image.fromarray(np.flip(dmap, 1))
imout = imout.resize(initSize, projection)

# Showing and saving the result
plt.imshow(np.array(imout), cmap = 'gray')
plt.savefig(pathSave)
plt.show()
