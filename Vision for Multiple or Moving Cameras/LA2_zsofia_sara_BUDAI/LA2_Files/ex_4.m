clear
close all;
name = {'iPhoneXs','iPhoneSE','SamsungS21fe'};

load InternalParameters_iPhoneXs.mat
load InternalParametersiPhoneSE.mat
load InternalParametersSamsungS21fe.mat

[coords, ima_pattern] = get_real_points_checkerboard_vmmc(9, 320, 1);

A{1} = InternalParameters_iPhoneXs;
A{2} = InternalParametersiPhoneSE;
A{3} = InternalParametersSamsungS21fe;


for phone = 1:3
    cam = imread(['cam_' name{phone} '.jpg']);
    
    points_cam= get_user_points_vmmc(cam);
    h_cam = homography_solve_vmmc(coords',points_cam);
    [h_cam,~] = homography_refine_vmmc(coords',points_cam, h_cam);
    Hccell{1} = h_cam;
    [R T] = external_parameters_solve_vmmc(A{phone}, Hccell)
    save(['ex4_phone_' name{phone} '.mat'],'R',"T");
end

%% 
load('ex4_phone_iPhoneSE.mat')
R_SE = R;
T_SE = T;
load('ex4_phone_iPhoneXs.mat')
R_XS = R;
T_XS = T;
load('ex4_phone_SamsungS21fe.mat')
R_FE = R;
T_FE = T;

unit = 1;

t_cam_1 = T_SE{1}-T_SE{1};
t_cam_2 = T_XS{1}-T_SE{1};
t_cam_3 = T_FE{1}-T_SE{1};

[r_se]=matrot_vmmc(R_SE{1},unit);
[r_xs]=matrot_vmmc(R_XS{1},unit);
[r_fe]=matrot_vmmc(R_FE{1},unit);

r_se_relative = r_se-r_se;
r_xs_relative = r_xs-r_se;
r_fe_relative = r_fe-r_se;


%% 
T_FE{1} = T_FE{1} - T_SE{1};
T_XS{1} = T_XS{1} - T_SE{1};




