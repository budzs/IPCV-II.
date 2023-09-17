
%% Scale-space gaussian pyramid
nlevels =     3; % number of octaves
nscales =     5; % number of scales
t0      =     2; % initial sigma.
factor  =     2;

%% e.g. read some image and convert to gray and double [0,1]
ima = im2double(imread('peppers.png'));
if size(ima,3) > 1
ima = rgb2gray(ima);
end

%% check if the required number of levels is plausible
[nr,nc]     = size(ima);
max_nlevels = floor(log2(min(nr,nc)));
nlevels     = min(max_nlevels,nlevels);
%% initialise pyramid use doScaleSpacePyramid function
PG = cell(1, nlevels);
PG{1}=doScaleSpace(ima,nscales,t0);

%% Create pyramid nscales number images on each level
for lv = 2:1:nlevels
    % subsampling the middle scale of the previous scale 

    ima_new=PG{lv-1}{round(nscales/2)}(1:factor:end,1:factor:end); 
    PG{lv} = doScaleSpace(ima_new, nscales, t0);
end
%% for displaying the image
for lv = 1:1:nlevels
    figure(lv)
    for s=1:1:nscales
    
    subplot_tight(2,ceil(nscales/2),s),imshow(PG{lv}{s});
    
    title(sprintf('Level: %.2f Scale: %.2f',lv, s));

    
    end
    
end