# 7--Quadtree-Encoding-of-a-Binary-Image-in-C
Image Processing in C
Introduction
Silhouettes contain much of the shape information necessary to recognise simple objects. For this reason they have been used widely in machine vision systems that check the shape of industrial parts or provide grasping information to robots. The digital version of a silhouette is a binary image i.e. an array of 1's and 0's. Figure 1 shows a digital image. In a binary image version of this, 0 represents a dark area and 1 represents a light area. The positions of pixels are defined with respect to the coordinate system shown in the figure. 
A significant problem of image processing is the large amount of storage needed for image data and the consequent computational load of performing calculations on large numbers of pixels. There has therefore been much effort in developing data representations or codings which are more compact but still capture the relevant features of the data. One promising data structure is the quadtree. 

Input parameters 

The program must read the image data from a text file which contains coded information on the size of the image and the number and position of black pixels in the image. The image file contains 
integers and the file has the following format: image width in pixels 
number of black pixels
x y location of each black pixel 

Input file example:
8 15 41 51 22 32 42 52 23 33 43 53 44 54 64 45 55 

Output data 

Black terminal node at position (2,2) with size 2
     Black terminal node at position (4,1) with size 1
     Black terminal node at position (5,1) with size 1
     Black terminal node at position (4,2) with size 2
     Black terminal node at position (4,4) with size 2
     Black terminal node at position (6,4) with size 
     
