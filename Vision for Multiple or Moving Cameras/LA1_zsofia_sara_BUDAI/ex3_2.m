clear
close all;

ima = imread("Hopper.JPG");
ref_ima = imread('hopper_reference.bmp');

xy_origin = get_user_points_vmmc(ima);
xy_target = get_user_points_vmmc(ref_ima);

H = homography_solve_vmmc(xy_origin, xy_target);

tform = maketform( 'projective', H');
tr_ima = imtransform(ima,tform,'XData',[1 size(ima,2)], 'YData',[1 size(ima,1)]);
energy = get_error_energy_vmmc(ref_ima, tr_ima);

figure(1)
subplot(1,2,1)
imshow(ima)
title("original")
subplot(1,2,2)
imshow(tr_ima)
title("transformed with"+ energy + " energy")

hom = homography_auto_vmmc(ima, ref_ima);
tform = maketform( 'projective', hom');
tr_ima = imtransform(ima,tform,'XData',[1 size(ima,2)], 'YData',[1 size(ima,1)]);
new_energy = get_error_energy_vmmc(ref_ima, tr_ima);
