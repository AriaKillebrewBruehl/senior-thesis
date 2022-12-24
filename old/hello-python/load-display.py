import cv2 as cv 
import sys

img = cv.imread("./grayscale/images/flowers.jpg")
if img is None:
    sys.exit("Could not read the image.")
cv.imwrite("starry_night.png", img)