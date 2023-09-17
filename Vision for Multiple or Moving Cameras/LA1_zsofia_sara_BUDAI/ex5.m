clear
close all;

i1 = imread('mountain1.jpg');
i2 = imread('mountain2.jpg');
i3 = imread('mountain3.jpg');
i4 = imread('mountain4.jpg');
th = 0.1;
display=1;
H = homography_auto_vmmc_2( i1, i2, th, display);
im_TH = maketform('projective',H');   
[im_stitchedH, stitched_maskH, im1TH, im2TH] = stitch_vmmc(i2, i1, im_TH);      

H = homography_auto_vmmc_2( im_stitchedH, i3, th, display);
im_TH = maketform('projective',H');   
[im_stitchedH, stitched_maskH, im1TH, im2TH] = stitch_vmmc(i3, im_stitchedH, im_TH);      

H = homography_auto_vmmc_2(  i4,im_stitchedH, th, display);
im_TH = maketform('projective',H');   
[im_stitchedH, stitched_maskH, im1TH, im2TH] = stitch_vmmc(im_stitchedH,i4, im_TH);      


figure
imshow(im_stitchedH)