%% parameters
% for the fixed sigmas scale-space
O    =     3; % number of levels
nscales     =     10; % number of scales
sigma0      =     1; % initial sigma

%%Inputs for non-linear scale space
perc=0.7;
psi_c=2;
%% calculate sigmas
k = 1;
sigmas = zeros(1,O*nscales); 
for o = 0:O-1
    % calculate the given equation
    sigmas(k:(k+nscales-1)) = sigma0.*pow2(((0:(nscales-1))/nscales) + o);
    k = k+nscales;
end
%% read some image and convert to gray and double [0,1]
ima = im2double(imread('peppers.png'));
if size(ima,3) > 1
ima = rgb2gray(ima);
end
%% create Gaussian Scale-space
SS=NonLinear13(ima, sigmas, perc, psi_c);
%% show Gaussian scale space
figure(1)
[nr,nc] = size(SS{1});
for j = 1:1:(nscales*O)
    figure(1),clf
    %% show jth scale in the scale-space
    % image
    subplot_tight(2,2,1),imshow(SS{j});
    %%MODIFICATION
    title(sprintf('Scale: %.2f ',sigmas(j)));
    % surf
    subplot_tight(2,2,3),surf(SS{j},'EdgeColor','None');colormap(gray);
    ax = gca; ax.XDir = 'reverse';axis tight; 
    view(-165,75)
    %% create kernel <<used>> at the jth scale (for visualization purposes only)
    %%Modification
    g   = fspecial('gaussian',[nr,nc],sigmas(j));
    %% show kernel <<used>> at the jth scale (for visualization purposes only)
    % image
    subplot_tight(2,2,2),imagesc(g*g');colormap(gray),axis off;
    %%Modification
    title(sprintf('Gaussian Kernel (sigma: %.2f)',sigmas(j)));
    % surf
    subplot_tight(2,2,4),surf(g*g','EdgeColor','None');colormap(gray);
    axis([1,size(SS{j},2),1,size(SS{j},1)]); ax = gca; ax.XDir = 'reverse';
    view(-165,75)
    %%
    pause(0.7);
end 
