% Exercises for N views
% N view reconstruction. Initial reconstruction using the F matrix
% between 2 cameras. Resection for the rest of cameras and bundle adjustment
% Euclidean reconstruction is obtained using the knowledge of
% the inf plane and scene structure

clear, close all,

% include ACT_lite path
ACT_path = './ACT_lite/';
addpath(genpath(ACT_path));
% include extra funs
extra_funs_path = './extra_funs/';
addpath(genpath(extra_funs_path));

warning off
disp('************************************* START')

load('data_BA_lab.mat')

% ------------------------------------------------------------------------
% 0. Select projected points (noisy version)
% ------------------------------------------------------------------------
q_data = q_r; % Uncomment this one to use noisy points

% ------------------------------------------------------------------------
% 1. Visualize results. Compare ideal (q) and noisy projections (q_r).
% ------------------------------------------------------------------------
% Projected points
draw_scene(Q, K(:,:,:), R(:,:,:), t(:,:));
draw_3D_cube_segments(Q);
title('Ground truth scene')

draw_projected_cube_noise(q,q_data)

% ------------------------------------------------------------------------
% 2. Compute the fundamental matrix using the first and last cameras
% of the camera set (N cameras)
% ------------------------------------------------------------------------
q_2cams(:,:,1)=q_data(:,:,1);
q_2cams(:,:,2)=q_data(:,:,ncam);

[F, P_2cam_est,Q_2cam_est,q_2cam_est] = MatFunProjectiveCalib(q_2cams);

disp(['Residual reprojection error. 8 point algorithm   = ' num2str( ErrorRetroproy(q_2cams,P_2cam_est,Q_2cam_est)/2 )]);
draw_reproj_error(q_2cams,P_2cam_est,Q_2cam_est);


% ------------------------------------------------------------------------
% 3. Resection. Obtain the projection matrices of the rest of cameras using the PDLT_NA function
% ------------------------------------------------------------------------


P_cams(:,:,:)=zeros(3,4,9);
P_cams(:,:,1)=P_2cam_est(:,:,1);
P_cams(:,:,ncam)=P_2cam_est(:,:,2);

for i= 2:ncam-1
    P_cams(:,:,i)=PDLT_NA(q_data(:,:,i),Q_2cam_est,0,1);
end


% ------------------------------------------------------------------------
% 4. Compute the statistics of the reprojection error for the initial projective reconstruction
% ------------------------------------------------------------------------
disp(['Residual reprojection error, initial projective reconstruction  = ' num2str( ErrorRetroproy(q_data,P_cams,Q_2cam_est)/2 )]);
draw_reproj_error(q_data,P_cams,Q_2cam_est);
% ------------------------------------------------------------------------
% 5. Proceed as in previous lab to extract the metric reconstruction from
% the initial N-view reconstruction
% ------------------------------------------------------------------------

[P_euc,Q_euc]=euclidean_reconstruction(q_data,P_cams,ncam,Q_2cam_est);
draw_3Dcube(Q_euc);

disp(['Reprojection error, After Euclidean reconstruction  = ' num2str( ErrorRetroproy(q_data,P_euc,Q_euc)/2 )]);
draw_reproj_error(q_data,P_euc, Q_euc);

% ------------------------------------------------------------------------
% 6a. Projective Bundle Adjustment. Use BAProjectiveCalib function
% Coordinates of 3D and 2D points are given in homogeneus coordinates
% ------------------------------------------------------------------------
% auxiliary matrix that indicates that all points are visible in all the cameras
vp = ones(npoints,ncam);
% ...
[P_BA,X3d_BA,xc]=BAProjectiveCalib(q_data,P_cams,Q_2cam_est,vp);

% ------------------------------------------------------------------------
% 6b. Compute the statistics of the reprojection error for the improved projective reconstruction
% ------------------------------------------------------------------------

disp(['Reprojection error, improved projective reconstruction  = ' num2str( ErrorRetroproy(q_data,P_BA,X3d_BA)/2 )]);
draw_reproj_error(q_data,P_BA,X3d_BA);
draw_3Dcube(X3d_BA);

% ------------------------------------------------------------------------
% 7. Proceed as in previous lab to extract the metric reconstruction from
% the optimized projective reconstruction (bundle adjustment)
% ------------------------------------------------------------------------

[P__euc_BA,Q_euc_BA]=euclidean_reconstruction(xc,P_BA,ncam,X3d_BA);
draw_3Dcube(Q_euc_BA);

disp(['Reprojection error, after bundle projective reconstruction  = ' num2str( ErrorRetroproy(q_data,P__euc_BA,Q_euc_BA)/2 )]);
draw_reproj_error(q_data,P__euc_BA,Q_euc_BA);
