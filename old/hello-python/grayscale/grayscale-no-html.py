import string
from PIL import Image 
import sys 


def grayscale(img: str) -> None:
    """Converts an image (passed as a string to the file) into grayscale."""
    try:
        # import image
        input_image = Image.open(img)
    except:
        raise Exception("File " + img + " could not be opened.") 

    # extract pixel map 
    pixel_map = input_image.load()
    # get dimenstions
    width, height = input_image.size
    # make it grayscale!
    for i in range(width):
        for j in range(height):
            r, g, b = input_image.getpixel((i, j))

            avg = int((0.299*r + 0.587*g + 0.114*b))
            pixel_map[i, j] = (avg, avg, avg)
    # save new image
    input_image.save(img+".-grayscale.png", format = "png")


imgs = sys.argv[1:]
for img in imgs:
    grayscale(str(img))



