import cv2 as cv 
import sys 
import cython
import string



def grayscale(img: str) -> None:
    """Converts an image (passed as a string to the file) into grayscale."""
    try:
        # import image
        input_image = cv.imread(img)
    except:
        raise Exception("File " + img + " could not be opened.") 

    @cython.boundscheck(False) # do not check array index bounds for speedup

    cpdef unsigned char[:, :] threshold_fast(int T, unsigned char [:, :] image):
    # get dimenstions
    width = input_image.rows
    height = input_image.cols
    # make it grayscale!
    for i in range(width):
        for j in range(height):
            r, g, b = input_image.at((i, j))

            avg = int((0.299*r + 0.587*g + 0.114*b))
            pixel_map[i, j] = (avg, avg, avg)
    # save new image
    input_image.save(img+".-grayscale.png", format = "png")


imgs = sys.argv[1:]
for img in imgs:
    grayscale(str(img))



