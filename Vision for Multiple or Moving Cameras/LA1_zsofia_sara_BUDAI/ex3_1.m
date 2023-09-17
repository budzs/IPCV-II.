clear;
close all;

img = imread("perspective_pattern.bmp");
ref_img = imread('reference_pattern.bmp');


xy_origin = get_user_points_vmmc(img);
xy_target = get_user_points_vmmc(ref_img);

H = homography_solve_vmmc(xy_origin, xy_target);

tform = maketform( 'projective', H');
tr_img = imtransform(img,tform,'XData',[1 size(img,2)], 'YData',[1 size(img,1)]);

energy = get_error_energy_vmmc(ref_img, tr_img);

figure(1)
subplot(1,2,1)
imshow(img)
title("original")
subplot(1,2,2)
imshow(tr_img)
title("transformed with"+ energy + " energy")