# Aria's Thesis Blog :partying_face:

*Basically a place where I will dump all my thoughts about the process and my feelings about thesis-life*

# 10.01.22

*Finalizing code for Jump Flooding Algorithm*

I've been trying to run the code for the JFA and have run into a number of small bugs that I have fixed:

- [x] Better prompts for user input and allowing for resizing of input images that aren't `N x N` squares
- [x] For the seed dictionary entries need to be tuples, not lists since you cannot hash a mutable object
- [x] Making sure that when I loop through the neighbors I don't go out of bounds on the image

The main issue that I'm facing is that this algorithm is *super slow*. It does have  O(N^2 log2(N)) complexity. But I think I am implementing something wrong and have some extra loops because this is taking way too long. 

## 09.30.22

Working on basic image operations! Converting an image to grayscale and implementing the Jump Flooding Algorithm.**

Grayscaling an image was easy to implement in `js` and in `python` (having a bit of trouble with `pyscript` due to the `pyodide` module not downloading). 

When I started trying to implement the jump flooding algorithm in `js` I realized that it would be somewhat tricky / expensive. This is because the pixels need to hold more information that just their `r, g, b` values (what their seed pixel is). Since we can't edit the pixels that we get when iterating over the image as an array my idea was to create a dictionary like so:

```
    pix_map = {}
    pix_map[[c,r]] = [r_s, c_s]
```
Each `[c, r]` pair that represents a pixel is mapped to the `[r_s, c_s]` pair that represents its seed pixel. However js has no dictionary type. I could create a custom object however that would defeat the purpose of a dictionary having `O(1)` lookup time. So I think for my thesis using `pyscript` will be the way to go. 



