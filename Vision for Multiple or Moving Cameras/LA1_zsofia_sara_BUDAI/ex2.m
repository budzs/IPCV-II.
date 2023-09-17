clear;
close all;
% 
% ima = imread("Hopper.JPG");
% ima_black = 0 * ima;
% 
% xy_origin = get_user_points_vmmc(ima);
% xy_target = get_user_points_vmmc(ima_black);
% 
% H = homography_solve_vmmc(xy_origin, xy_target);
% tform = maketform( 'projective', H');
% tr_ima = imtransform(ima,tform,'XData',[1 size(ima,2)], 'YData',[1 size(ima,1)]);
% 
% 
% figure(1)
% subplot(1,2,1)
% imshow(ima)
% title("original")
% subplot(1,2,2)
% imshow(tr_ima)
% title("transformed")
% 

img = imread("perspective_pattern.bmp");
ref_img = imread('reference_pattern.bmp');


xy_target = [4 604 604 4; 4 4 466 466];

% xy_origin = [18 475 599 41; 59 11 420 446];
xy_origin = get_user_points_vmmc(img);


H = homography_solve_vmmc(xy_origin, xy_target);

tform = maketform( 'projective', H');
tr_img = imtransform(img, tform,'XData',[1 size(img,2)], 'YData',[1 size(img,1)]);

energy = get_error_energy_vmmc(ref_img, tr_img);


figure(1)
subplot(1,2,1)
imshow(img)
title("original")
subplot(1,2,2)
imshow(tr_img)
title("transformed")

