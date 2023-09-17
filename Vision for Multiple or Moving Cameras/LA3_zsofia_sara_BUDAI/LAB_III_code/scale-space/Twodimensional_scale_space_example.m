%% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% VIDEO FOR MULTIPLE AND MOVING CAMERAS (VMMC)
% IPCV @ UAM
% Marcos Escudero-Viñolo (marcos.escudero@uam.es)
%% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
clear all
clf
clc
%% parameters (example)
t0      =           5; % initial scale.
nscales =         500; % number of scales in the scale-space representation 
%% read some image and convert to gray and double [0,1]
ima = im2double(imread('ima_T1.jpg'));
if size(ima,3) > 1
ima = rgb2gray(ima);
end
%% create Gaussian Scale-space
SS=doScaleSpace(ima,nscales,t0);
%% show Gaussian scale space
figure(1)
[nr,nc] = size(SS{1});
for j = 1:1:nscales   
    figure(1),clf
    %% show jth scale in the scale-space
    % image
    subplot_tight(2,2,1),imshow(SS{j});
    title(sprintf('Scale: %.2f ',j.*t0));
    % surf
    subplot_tight(2,2,3),surf(SS{j},'EdgeColor','None');colormap(gray);
    ax = gca; ax.XDir = 'reverse';axis tight; 
    view(-165,75)
    %% create kernel <<used>> at the jth scale (for visualization purposes only)
    g   = fspecial('gaussian',[nr,nc],sqrt(j.*t0));
    %% show kernel <<used>> at the jth scale (for visualization purposes only)
    % image
    subplot_tight(2,2,2),imagesc(g*g');colormap(gray),axis off;
    title(sprintf('Gaussian Kernel (sigma: %.2f)',sqrt(j.*t0)));
    % surf
    subplot_tight(2,2,4),surf(g*g','EdgeColor','None');colormap(gray);
    axis([1,size(SS{j},2),1,size(SS{j},1)]); ax = gca; ax.XDir = 'reverse';
    view(-165,75)
    %%
    pause(0.01);
end 
