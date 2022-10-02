import sys 
import math 
from PIL import Image

def get_seeds(input_image : Image) -> dict:
    # get dimensions
    width, height = input_image.size

    # seeds[(i, j)] holds the seed pixel for the pixel (i, j)
    seeds = {}

    for i in range(width):
        for j in range(height):
            r, g, b, a = input_image.getpixel((i, j))
            if r == g == b == 0:
                seeds[(i, j)] = None
            else:
                seeds[(i, j)] = (i, j)

    return seeds


def jmp_flood(img : str) -> None:
    try:
        # import image
        input_image = Image.open(img)
    except:
        raise Exception("File " + img + " could not be opened.") 

    # extract pixel map 
    pixel_map = input_image.load()
    # get dimensions
    width, height = input_image.size

    if width != height:
        print("Image must be a N x N square.")
        resz = input("Do you want to resize to " + str(width) + " x " +  str(width) + "? (Y/N) ")
        if resz == "N" or resz == "n":
            raise Exception("Image must be a N x N square.")
        else:
            input_image = input_image.resize((width, width))

    seeds = get_seeds(input_image)
    N = width
    k = 2

    while N // k > 1:
        print("N:", str(N))
        print("k:", str(k))
        print("N//k:", str(N//k))
        for i in range(width):
            for j in range(height):
                p = (i, j)
                pr, pg, pb, pa = input_image.getpixel(p)

                # iterate over the neighbors
                for qi in range(i-k, i+k, 1):
                    # make sure you don't go off the page
                    if qi < 0 or qi >= N:
                        continue
                    for qj in range(j-k, j+k, 1):
                        # make sure you don't go off the page
                        if qj < 0 or qj >= N: 
                            continue
                        q = (qi, qj)
                        qr, qg, qb, qa = input_image.getpixel(q)

                        # p is undefined but q is colored
                        if pr == pg == pb == 0 and not(qr == qg == qb == 0):
                            # set the pixel to be the color of its neighbor
                            pixel_map[i, j] = (qr, qg, qb)
                            # set p's seed
                            if q in seeds:
                                seeds[p] = seeds[q]
                            else:
                                raise Exception("Pixel " + str(q) + " is not in seeds!")

                        # p and q are colored
                        elif not(pr == pg == pb == 0) and not(qr == qg == qb == 0): 
                            p_seed = seeds[p]
                            q_seed = seeds[q]
                            # p is farther from its seed than from q's seed
                            if math.dist([i, j],p_seed) > math.dist([i, j], q_seed):
                                pixel_map[i, j] = (qr, qg, qb)
                                # update p's seed
                                if q in seeds:
                                    seeds[p] = seeds[q]
                                else:
                                    raise Exception("Pixel " + str(q) + " is not in seeds!")
        k *= 2

    # save new image
    input_image.save(img+".-jump-flood.png", format = "png")

imgs = sys.argv[1:]
for img in imgs:
    jmp_flood(str(img))