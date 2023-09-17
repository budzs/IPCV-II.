%% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% VIDEO FOR MULTIPLE AND MOVING CAMERAS (VMMC)
% IPCV @ UAM
% Marcos Escudero-Viñolo (marcos.escudero@uam.es)
%% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
clear all
close all
clc
%% Matlab function impyramid performs simmilarly.
%% parameters (example)
factor  = 2;    % downscaling  / pooling factor 
a       = 0.375;
g       = [.25-a/2 .25 a .25 .25-a/2]; % gaussian filter 
nlevels = 8;   % number of (desired) levels in the Pyramid 
%% read some image and convert to gray scale and double [0,1]
%% read some image and conver to double [0,1]
imargb  = im2double(imread('peppers.png'));
%% convert to gray scale  
ima     = rgb2gray(imargb);
%% create Gaussian Low-pass pyramid
PG=doGaussianPyramid(ima,factor,nlevels,g);
%% show Gaussian Low-pass pyramid
nlevels = numel(PG);
% note that the maximum number of levels is constrained by the image size 
figure(1)
figure(2)
for lv = 1:1:nlevels
    figure(1)
    subplot_tight(2,ceil(nlevels/2),lv),imshow(PG{lv});
    title(sprintf('Level: %d, resolution: %d x %d',lv, size(PG{lv})));
    figure(2)
    [X,Y] = meshgrid(1:size(PG{lv},2),1:size(PG{lv},1));
    figure(2), hold on
    scatter3(X(:),Y(:),lv.*15.*ones(numel(X),1),10,PG{lv}(:),'filled')
end
figure(2), 
ax = gca;
ax.YDir = 'reverse';
axis tight
axis off
colormap gray
view(15,50);