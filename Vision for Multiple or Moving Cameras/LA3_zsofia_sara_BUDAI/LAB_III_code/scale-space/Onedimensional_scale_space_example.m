%% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% VIDEO FOR MULTIPLE AND MOVING CAMERAS (VMMC)
% IPCV @ UAM
% Marcos Escudero-Viñolo (marcos.escudero@uam.es)
%% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
clear all
close all
clc
%% parameters (example)
t0      =          1; % initial scale (and step).
nscales =        200; % number of scales in the scale-space representation 
%% e.g. read some image and convert it to gray and double [0,1]
ima = im2double(imread('peppers.png'));
if size(ima,3) > 1
ima = rgb2gray(ima);
end
% obtain the histogram  and get the probability mass function 
% (for convenience) to create a one-dimensional signal
[f,ix] = imhist(ima);
f=f./sum(f);
%% create Gaussian Low-pass pyramid
SS=doScaleSpace(f,nscales,t0);
%% show one-dimensional scale space
figure(1)
for j = 1:1:nscales
    figure(1),
    grid on
    hold on, plot3(ix,j.*ones(1,numel(ix)),SS{j},'-r')
    title(sprintf('Scale: %.2f (~sigma: %.2f)',j.*t0,sqrt(j.*t0)));
    
    ax = gca;
    ax.XDir = 'reverse';

    xlabel('x')
    ylabel('Scale')
    zlabel('f')
    view(180,-0)
end
