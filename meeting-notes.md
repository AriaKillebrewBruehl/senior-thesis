## Meeting Notes

# 04.07.2023

- Is the mapping of grey to stipple dot correct?
  - Gamma correction 
- I need to figure out the raid of dots as a function of tone and density 
  - If the final image was blurred we could see grayscale differences 
- I want the radius of the dot to be the square or square root of things
  - Have dot radius also be a factor of density 
- Try stippling a bunch of different levels of pure gray
  - Then compute the avg gray of original image and output image 
    - Sum up all white and divide by the number of pixels 
  - Then play with different stipple raidi and see how this 
  - Fixed density calibrate radi to get tone 
    - Density versus radi 
      - Want uniform tone across this 
- For sag we need a text file that describes the center of a all the circles and their sizes 
- Play with density things 
  - Density vs tone and radi vs tone 
- Have component that prints out dot centers and radi to a .txt 

For eye detail try binning and then making pure black / white / mix (half toning) and then add that in 

- So selecting some things that just get halftones 
- Play around with number of iterations
  - Add these image to thesis 
  - For different densities of dots do we need to iterate longer 
  - 



# 03.10.2023

Questions for today's meeting:

- [ ] What should be in the NPR section
  - The motivation for NPR section 
    - Humans have decided that there is a way to represent the world, as graphics ppl we have tried to mimic this 
    - Something problematic about us trying to asses our renderings and others 
- [ ] What should the related work section look like 
  - Its all good rn 
- [ ] What should be in the intro versus the background 
  - Into stays non technical so its good rn 
- [ ] Analyzing results 
  - In 2 and 3 there was the vanilla tecnique, but I found that there were things that needed to be changed, here are options to make it better, this is how I got this image versus that image, show different preprocessed versions of Sydney and how the vanilla tool reacts, this saves me from having to perfectly present every little tweak, just kind of talk about the things that I recognized needed to be tweaked, all the changes can go in 4 
  - Just use core qualities to be able to check off boxes 
  - Areas of expansion 
    - American and European rendering in print, who they thought was important to render what white dudes, who is on the printed money that this is modeled after 
    - Think about what the artistic techniques for rendering ppl would be if the WSJ stuff came from a completely different part of the world, engraving works artistically in its basic form bc the original ppl had white skin and we were trying to depict marble sculptures, IT GOES ALL THE WAY TO THE TOP 
    - There are biases built into machine learning bc of training data, biases built into our techniques because of the choices we are making in choices to render, biases in the world of illustration and WSJ because of who they choose to depict 
    - Try technique on seal 



## qualities of hedcuts

- What are the feature lines
  - Hair 
  - chins
  - Outlines 
- Mainly just stipples other than clothing 
- Combination of feature lines 
  - There are computer graphics papers that try to lay out what the feature lines are that artists should choose 



- To fix not placing dots in the highlights
  - There is an x axis of 0-255 and a y axis 
  - Use the posterization thing that is done is isophotes and avoid the white sections 
  - Try to tweak to make thing under the cheek bone dark and what is above the cheek bone darker
    - Do this in photoshop as a preprocess 
- Also try to get final outline around image 
  - In fig d of process there is sone inkiness that makes it look better 



- 4.3 here is what we never got to, here is what I don't like about other pols work 
- 4.4 these things should be fixed, here are maybe some ideas 
- Conclusion is just 1 page 



# 03.03.2023

- **writing**
  - I have a lot of the nitty gritty things but I don't have an overview 
  - Need to make this overview 
  - Related work section 
  - Overview 
    - No outside reader is going to want to slog through things if they don't know where it is going 
    - Thesis is about me presenting what I did , to know what I did they need to know what I now know which is in chapter 1 
    - Do this in the introduction 
      - There are hedcuts
        - Why are hedcuts the way that they are 
        - Do a deep dive here
      - There is a community that is interested in making them by and and with a computer 
      - There are recent AI / ML approaches 
      - Other just use certain intuitions 
        - This is what my thesis is doing
      - In this thesis I show you a prototype of a system I have been building that attempts to mimic aspects of hedcut rendering 
        - This is how it does it
        - We followed the work of so and so 
        - Here is the overall approach 
          - These figures show that process and the work that we are mimicking 
          - Show examples of what the literature does and that we are working to mimic and tweak this 
      - Then the first chapter gives introduciton
      - The second chapter does this
      - The third chapter does that 
      - In general this should give ppl a good idea of what the steps are and what the method is 
  - Relating this to other peoples work can happen at the beginning or the end 
  - Also the non photo realistic rendering part should be flushed out more, do this with Jim 
    - This should be done at the very end and should be steered in the direction of what the goals were 
    - Someting about illustration the tgets the point across by selecting details, this is something that humans have developed to communicate 
      - What is the point of the hedcut rather than just a photograph 
        - Characterizing a person in a timeless way 
  - In edge detection talk about why it is useful to have stipples follow contours and such 
- What is being fed is not right 
  - What we want is something begin the result of their ETF stuff 
  - Try to find something that will be okay immediately when it gets fed to the program 
  - What I am getting is just the one contour but instead I want the series of these 
    - Try to make my image be a gradient version of a 
  - Try images from graphics that are USGS elevation data 

# 02.24.2023

T assigns a vector to every spot in the image and X is a location in the image, associated with the vector is a direction

normalize can mean either getting a weighted average or compute a vector and then divide by the magnitude so you get a unit vector
not sure which one it is
could also be suppose you compute the gradient throughout the whole grid
keep the range of the gradient magnitudes between 0 and 1, this might be correct for g_hat (can't be the unit one for g-hat otherwise we'd always have 0)
also unclear what tanh is

- they take a collection of old vectors, take a weighted sum of them
- are we including ourselves in our neighborhood

- ws is spatial weight of 0/1, limits the neighborhood

- wm compares the gradient at those two locations

get pretty image by getting x and y sobel filter and then

- g-hat is the magnitude of the vector g once it is normalized

  - so then when have have g-hat - g-hat those are just two scalara

- approximate gradient comepenents in x and y using sobel filters
- then compute the magnitude of the two vectors
- then find min and max of these
- then normalize things so that they are between 0 and 1 (this would be the normalized gradient magnitude)
- then if t is not unit then k is the sum in eq 1 missing the tcur(y)

- they seem to say if you don't update t then g never evolves, if you iteravly apply their update then you can get a change to g

- initial t is the gradient but then the magnitude is between 0 and 1
- mathematical way of taking the gradient over the height field, then are a lot of different height fields that have the same gradients

- initial t is just the normalized gradient, then you do the wieghting to refigure the t, then we have a smoother version of the t, then we have something like an ETF

  - but how do we update the gradient of the image, inverting the sobel filter?

- to get the pictures they use the C and L 1993 technique, can we find a working implementation of that?

- compute normalized gradient in x and y using sobel filter
- then take sqrt of sum of square (magnitude) and then scale everything form 0 to 1 and then make that the u and v for anne then see what png i get out
- see if this looks like the flow of our images
- this would give initial normalized gradient vector field, then do some step of t and look at new output image and see if that works

# 02/10

writing:
in color space seciton include something that isn't just 0s and 1s
code:
possibly at the end so things with antialiasing

# 01/27

- dots
  -vornoi diagram dots
  for each dot compute weighted avg of all pixels in cell for that dot
- that's the new center
- repeat
- stop when weighted avg center is the dot

- for writing ab weighted transform creation

  - i'm reimplementing this one paper's approach
    - that's a paper someone else could read
    - but i can take ppl through how this works and talk about some of these steps
    - i can choose what to focus on - there is a known thing but we found a new thing that works faster
    - maybe just a pseudo code of my code
    - pretend i'm just telling a friend
    - what are ppl trying to solve w this, show pseudo code / how it works, what is cool ab it

- ch 1 overview of the whole process
- somewhere put a sketch out of the different pieces of the puzzle
  - somewhere have the flow chart of the process and explain how i'll guide ppl through it, what i'll do in each chapter

# 12/06

big F is lookuptable for all the rows that we made by doing the thing over all of the columns

change everything to lookups tables

# 11/29

- s gives where the parabolas centered at q and r cross

# 11/22

- maybe change how i decide what is a seed and what is not
- create a separate map that has 0,1 initially to inidicate if something is a seed or now
- somehow try to capture the weirdness with a second type of undefined
- basically there is numeric instability that leads to weird things

- picking pixel
  - just randomly select 1/m pixel but not ones on the feature lines
  - if distance between dots is d then if random value is less than 1 / d^2 then include it

# 11 / 15

We have wide feature lines and a distance away from said feature line.

- label all offsets that are the same distance away with the same id

  - so on the line is id offset 0, the id offset 1

- each line has its w0 thickness

- for jfa check approximations and for a bug
  - go back to jfa and do basic lines to try and find off cases

## 11 / 10

- for distance map do sin(w \* d) and as w changes the waves are larger of the lines are farther spaced apart

- walk in a certain direction and regularly space lines and a regular frequency
- we already know the distance so we just apply dividing up the distance by a certain interval

# writing

- can alternate between going deep and then stepping back
- talk ab why ppl have studied it
- definitions that talk ab technique and the thinking
- present specific technique
- then a different one
- then speculate about the goal is

- standard stuff that can be found can either be elaborated on or just point to a reference

- give enough info in the beginning to be able to talk about what you want to talk about later

- this can be an opportunity to really talk about all the things that i have learned

I. the problem (WSJ), context (NPR), addressing DL (this thesis shows an approach that relies on traditional image processing techniques that augments them for hedcuts, it's also possible to apply DL but we won't do that), outline the document

II. back ground for these results to support III, here is what a flood is, this is what people do

- color space
- DoG
- floods
- opencv

III. my approach and results

IV. assesement of approach and suggestions or tweaks

- these are the other ideas that i have now that i have done the thing

# 11 / 01

normalizing everytinh compute min dn max and rescale

using jfa for distance how do we do priority

do flood on isophote and edge you know what isophote / edge you are closest too
then for each pixel see if it is closer to its edge or its isophote based on the priority function
basically flood then find minimum

figure out how to do voronoi diagram in opencv

## 10 / 25

2. Why does CIE LAB look like that
   - convert to CIELab space
   - so then do binning based on L
   - then convert back to RBG ? just to see if things look good
3. What do I do about the connected components thing just in general being able to remove components

- look into labels image

# 10 / 13

- removing blobs based on area is okay if we know the general width
- when i start tuning things do 200 x 200 images
  - want to have "printable" resolution
  - scale at which i care about features
- see about spines or blobs to find their length?
- label pixel with shortest linear distance from outer pixel?
- pick radius, find all the ports where you can place a circle with that radius along the blob

## 10 / 04

- prototype in python then do in c++
  - write python not so pythonic bc I have to rewrite
  - do small images in python
  - larger things in c++ / c
  - compiled c++ that python calls (are there ways to do this)
  - or do on GPU
- opencv seems like a good thing because it has compatibility for c++ and python

- for wasm use something that a lot of ppl work with

- jump flood: inner loop is not the full range!!! just 3 values!

- paper about lum quantization

- maybe to make headcuts better adjust stroke size on the outside, blend two isophote papers (stipling and line thickness)

  - for hashing paper use this too

- next steps:

  - work on DoG using opencv

    - port existing python to use opencv

  - isophote calculation

## 09 / 27

#### Prep

- need algorithm for finding isophote curves
- (4.2) what is "scattered data interpolation"
  - they adopt multi-level b-spline interpolation
    - https://saga-gis.sourceforge.io/saga_tool_doc/2.2.5/grid_spline_4.html
    - Lee paper?
- need to implement jump flooding algorithm for distance
  - https://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.101.8568&rep=rep1&type=pdf
- whole W window part is really confusing to me
- talk about where to find code

- **isophote**
  - weighted distance computed offset map
  - then get stipples
  - isophots, Kim, Woo et al.
- **structure grid**
  - Son, Lee et al.
- constrained Lloyd relaxation algorithm
- kang-egsr explains lloyd and jump flooding?
- how do you come up with flow lines
- how do we modify the dots so they live nicely along the lines

- steps for coding

  1. read and image and write an image
  2. way of reasoning about the pixels of the image
  3. then will need geometric calculations overtop of that for placing points

  - image loading has t be done asychronously
  - main system is pulling from the event that the image has been loaded
  - image data will probably be a single dimensional array of rib values

- write code that loads an image and handles it

  - look at 121 code
  - see about getting image as grey scale then compute an image of all the distances
  - given pixel centers compute the flood
  - this code is very language independent so it'll be okay

- basically need to load image, mess with it, and write it out

  - need access to raw rig data
  - gimp

- dig into lloyd and feature vector field thing

  - to crack this open better
  - blazer
  -

## 09/20

- allowed to preprocess and remove backgrounds for making things easier
- we could try to venture into deep learning
  - but we'd need really good tutorials
- with out deep learning we'd have to survey some things which are a lot of sifernet image techniques
- shift target from humans to some family of pictures to tackle
- or just read papers and then think of something we can make up
- JFA uses no neural things

  - from 2006
  - but result used in 2022

- get to feature map step by hand
- or interface the allows you to paint vector fields
- can we do clever optimization that leaves neural out of it
- wiggle things until there is a local optimal
- how do you read an image
- run vonoid library
- produce image
- blue noise
  - fill plain with dots to get certain level of brightness
  - make it so there is not obvious directionality to dots
- use this as guiding paper
  - https://hal.inria.fr/hal-01528484/file/Martin_2017_SDS.pdf
- no images, doen in 3d realtime stipling
- read Son paper and survey paper
  - ost grid paper
- look at peoples code!!!
- figure out what to replicate and what to twist for ourselves

## 09/16

- stylized renderings of images to make them look like a drawing
- half-toning / dithering :
  - take an image
- have an approach that i'd like to take
- how do you do headset style images
- look at what they did, take their ideas and come up with own approach
- goal is to go farther than the 2001 thesis

## Thesis Meeting 09/13

- replicate / mimic the spacetime constrains but with modern matter.js
- ai learns how to do thing (kinda)

- can we automate the player and have them do something that's optimal
- learning how physical being behave
- but also just inventing artificial thing
- minimize the complexity of the set up
  - even in a simple system can we get an artifact that works
- be able to record all positions of an object so that we can measure energy
  - HELLO WORLD:
  - ball clearing hurdle and gets into a bucket
    - then replace ball with creature that will try to get itself in
      - then we need to record energy performed by the musculature
        - integral over time of the change in angle?
- can you run matter.js without the animation
- function running in each frame can just record the position
- set up framework for recording trials
- do this and model inchworm and figure out musculature
- this is aa proof of concept
  - now we know we can use matter.js to do this optimization
- build up optimization

- next steps:
  - build hello world of ball into bucket
- look at some papers that cite spacetime constraints
  - see if there are better jobs of explaining what was done
- keeping working in matter
  - inch worm
    - get it to jump
  - hello world particle lover a hurdle simulation
  - instrument it and record
  - turn off animation?
- look at later citations of spacetime paper
- slowly learn about box-2d at some level

## Thesis Meeting 09/09

- look for paper (from 2018?) about current rendering techniques for movies

- 6-8 week project that i could prototype for the physics based one
- quick and dirty way to do space-time constraints paper after the year 2015
  - we can use a game engine that has one of these physical simulations to make things easier
- goal of a certain dynamic (maximizing bend over time)
  - attempt different initial conditions to maximize bend
  - you can just solve a physics problem and get this initial position
  - solve problem for initial conditions and minimize the energy needed to get from point a to point b
  - vary the schedule of muscle resting lengths to find the one that minimizes energy and meets the conditions
- take an existing physics engine and measure the energy expended, have controls that can vary lengths, figure out best resting lengths
  - ML: randomly assign weights, different initial conditions
    - change each condition slightly and see how that changes the performance
    - schedule of muscular activation, vary them, find the local optimal
- essentially a repeating of space time constrains using a game engine
  - a bunch of papers for this
  - then we can take it in different directions
    - interactive optimization
- tricky part how do we pick the figures for this
  - we want well chosen figure, activities, and goals
    - could just mimic luxo paper
- want to make sure we have angle springs
- try to find if matter.js has an angle spring

next steps

- start filling with matter.js

  - maybe make creatures

- know the principles behind the matter.js

  - read documentation, etc.

- tell jim what papers i found, see if he has an opinion about what to read together
