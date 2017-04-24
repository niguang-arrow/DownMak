# Why the RGB to YCbCr

Medical research proved that the human eye has different sensitivity to color and brightness. Thus there came about the transformation of RGB to YCbCr.

  Y:  Luminance;  亮度

Cb: Chrominance-Blue;  色度， 色差

Cr: Chorminance-Red;

Luminance is very similar to the grayscale version of the original image. Cb is strong in case of parts of the image containing the sky(blue), both Cb and Cr are weak in case of a color like green, and Cr is strong in places of occurrence of reddish colors.

## Equation

```
Y  =  (77/256)R + (150/256)G +  (29/256)B
Cb = ‐(44/256)R ‐  (87/256)G + (131/256)B + 128
Cr = (131/256)R ‐ (110/256)G ‐  (21/256)B + 128
```

## Reference

+ [Why the RGB to YCbCr](https://makarandtapaswi.wordpress.com/2009/07/20/why-the-rgb-to-ycbcr/)

