import imageio.v3 as iio

# read in an image
im = iio.imread("images/butterfly-line.jpg")
# get it's shape 
# height, width, channel (3 = 3 color bits)
print(im.shape)


