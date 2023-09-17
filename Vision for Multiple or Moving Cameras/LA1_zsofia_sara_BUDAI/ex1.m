clear
close all;

ima = imread("Hopper.JPG");
xy_origin = [41 484 586 72; 124 30 299 414];
xy_target = [32 632 632 32; 25 25 468 468];

tform_1 = maketform( 'projective', xy_origin', xy_target');

H = homography_solve_vmmc(xy_origin, xy_target);
tform_2 = maketform( 'projective', H');

tr_ima_1 = imtransform(ima,tform_1,'XData',[1 size(ima,2)], 'YData',[1 size(ima,1)]);
tr_ima_2 = imtransform(ima,tform_2,'XData',[1 size(ima,2)], 'YData',[1 size(ima,1)]);

energy_1 = get_error_energy_vmmc(ima, tr_ima_1);
energy_2 = get_error_energy_vmmc(ima, tr_ima_2);
figure(1)
subplot(1,3,1)
imshow(ima)
title("original")
subplot(1,3,2)
imshow(tr_ima_1)
title("transformed_1")
subplot(1,3,3)
imshow(tr_ima_2)
title("transformed_2")

