# senior-thesis

This is the repository for all my senior thesis work :cowboy_hat_face:

I have an unofficial [thesis-blog](https://github.com/AriaKillebrewBruehl/senior-thesis/blob/main/thesis-blog.md) that houses brain-dump regarding my thoughts and feelings about the process!

## Thesis Motivation

The goal of my thesis is to write a program that can render photographs as stipples drawings like the [hedcuts in the Wall Street Journal](https://www.wsj.com/articles/whats-in-a-hedcut-depends-how-its-made-11576537243).These images are small, typically only three by five inches, but creating just one portrait can take an illustrator up to five hours to complete. In 2019 the WSJ had a team of only five artists who would spend all day generating these images. Although nothing can ever replace the look of a hand-drawn image, digital rendering can significantly speed up the process and make this art form more accessible.

I will render these images without the use of AI or ML, in part because I have no AI or ML experience and in part because this will avoid the question of [the ethics of AI generated art](https://jamiearpinricci.medium.com/the-ethics-of-ai-generated-art-57fb04b71646).

## Process

The main paper I will be following for this work is the 2010 Kim et al. paper [Automated Hedcut Illustration Using Isophotes](https://link.springer.com/chapter/10.1007/978-3-642-13544-6_17). I have diverged from this paper in a few ways, most notibly in the distance transform calculation for which I use the 2009 Felzenszwalb and Huttenlocher paper [Distance Transforms of Sampled Functons](https://cs.brown.edu/people/pfelzens/papers/dt-final.pdf).
