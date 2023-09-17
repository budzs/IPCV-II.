clear
close all;
img = imread("FixedCamera_Data\PatternImage_Orientation_1.bmp");
[coords, ima_pattern] = get_real_points_checkerboard_vmmc(9, 320, 1);
%%
Homographies = [];
for i= 1:5
    %string = "FixedCamera_Data\PatternImage_Orientation_" + i +".bmp";
    string = "Phone\"+i+".jpeg";
    img = imread(string);
    Homographies = [Homographies , get_user_points_vmmc(img)];
end 
%%
H={}; 
%if( size(coords) ~= size(Homographies(:, 1:9 )))
    coords = coords';
%end
for j = 0:4
    num = j*9+1;
    H{j+1} =homography_solve_vmmc(coords,Homographies(:, num:num+8 ));
end

%%
H_ref = {};
rperr = [];
for i=0:4
    num = i*9+1;
    [H_ref{i+1} , rperr(i+1)] = homography_refine_vmmc(coords, Homographies(:, num:num+8 ), H{i+1});
end 

%%

figure()
for j=1:5
    tform = maketform( 'projective', H_ref{j}');
    tr_ima = imtransform(ima_pattern,tform);
    subplot(1,5,j)
    imshow(tr_ima);
    hold on

end 
