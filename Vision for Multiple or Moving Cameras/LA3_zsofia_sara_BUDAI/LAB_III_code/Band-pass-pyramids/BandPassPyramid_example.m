%% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% VIDEO FOR MULTIPLE AND MOVING CAMERAS (VMMC)
% IPCV @ UAM
% Marcos Escudero-Viñolo (marcos.escudero@uam.es)
%% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
clear all
close all
clc
%% parameters (example)
factor  = 2;    % downscaling  / pooling factor 
a       = 0.375;
g       = [.25-a/2 .25 a .25 .25-a/2]; % gaussian filter 
nlevels = 7;   % number of (desired) levels in the Pyramid 
%% read some image and convert to gray scale and double [0,1]
ima = imread('ima_T1.jpg');
ima = im2double(rgb2gray(imresize(ima,[512 512])));

%% create Gaussian Low-pass pyramid
PG=doGaussianPyramid(ima,factor,nlevels,g);
%% create Band-pass pyramid
[PB,range]=doBandPassPyramid(PG,g);
%% show Band-pass pyramid
nlevels = numel(PG);
% note that the maximum number of levels is constrained by image size 
figure(1)
figure(2)
for lv = 1:1:nlevels-1
    figure(1)
    subplot_tight(2,ceil(nlevels/2),lv),imshow(PB{lv},range);colorbar
    title(sprintf('Level: %d, resolution: %d x %d',lv, size(PB{lv})));
    figure(2)
    [X,Y] = meshgrid(1:size(PB{lv},2),1:size(PB{lv},1));
    figure(2), hold on
    scatter3(X(:),Y(:),lv.*15.*ones(numel(X),1),10,PB{lv}(:),'filled')
end
figure(2), 
ax = gca;
ax.YDir = 'reverse';
axis tight
axis off
colormap gray
view(15,50);