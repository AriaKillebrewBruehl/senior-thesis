### basic alg 

- start w N x N grid 

```javascript
k = 2
while (N / k < 1) {
        for p in image.values() {
            for (i = -k, i <= k, i++) {
                if p.value is undef and q.value is colored {
                        p.value = q.value 
                        if p.value is colored and q.value is colored {
                            if dist(p, s_p) > dist(p, s_q) {
                                p.value = q.value
                            }
                        }
                }
            }
        }
        k *= 2
}
``` 

tracking the seed pixels:
\
do initial pass to set up seed dictionary 
```javascript

for p in image.values() {

}
```