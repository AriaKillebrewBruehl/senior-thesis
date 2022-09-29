## Meeting Notes



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

-  get to feature map step by hand
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

  