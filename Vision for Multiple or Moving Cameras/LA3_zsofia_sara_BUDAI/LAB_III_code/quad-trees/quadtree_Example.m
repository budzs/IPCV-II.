%% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% VIDEO FOR MULTIPLE AND MOVING CAMERAS (VMMC)
% IPCV @ UAM
% Marcos Escudero-Viñolo (marcos.escudero@uam.es)
%% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
clear all
clf,
clc
%% parameters (example)
K     = 8;                             % maximum number of 'levels'
alpha = 0.005;                         % threshold / condition
fun   = @(x) (var(reshape(x,[],size(x,3)))) > alpha; % driving function: variance > alpha
%% read some image and conver to double [0,1]
imargb  = im2double(imread('peppers.png'));
%% resize to 2.^K, 2.^K
imargb = imresize(imargb,[2.^K,2.^K]);
%% convert to gray scale  
ima     = rgb2gray(imargb);
%% quadtree decomposition
S       = qtdecomp(ima,fun);
%% show image and quatree decomposition (both edges and average values)
% init resources
leaves   = repmat( double(0),size(S));
blocks   = repmat( double(0),size(S));
avg_gray = repmat(double(0),size(S));
avg_rgb  = repmat(double(0),size(imargb));
% travel the tree
dims = 2.^(K:-1:0);
for j=1:numel(dims)
  dim = dims(j);
  numblocks     = length(find(S==dim));    
  
  if (numblocks > 0)   
   if j < numel(dims)  
        % fill edge image (blocks)
        values                = repmat(double(1),[dim dim numblocks]);
        values(2:dim,2:dim,:) = 0;
        blocks                = qtsetblk(blocks,S,dim,values);

        % fill average image (avg_gray)
        [blockValues, Sind]   = qtgetblk(ima, S, dim);
        avg_values            = repmat(reshape(mean(reshape(blockValues,[],size(blockValues,3))),1,1,[]),[dim dim]);
        avg_gray              = qtsetblk(avg_gray,S,dim,avg_values);

        % fill average image (rgb)
        for ch=1:size(imargb,3)
        [blockValues, Sind]   = qtgetblk(imargb(:,:,ch), S, dim);
        avg_values            = repmat(reshape(mean(reshape(blockValues,[],size(blockValues,3))),1,1,[]),[dim dim]);
        avg_rgb(:,:,ch)       = qtsetblk(avg_rgb(:,:,ch),S,dim,avg_values);
        end
    
    % fill leaves (last layer)
    else
        values                = repmat(double(1),[dim dim numblocks]);
        values(2:dim,2:dim,:) = 0;
        leaves                = qtsetblk(leaves,S,dim,values);

        % fill average image (avg_gray)
        [blockValues, Sind]   = qtgetblk(ima, S, dim);
        avg_values            = repmat(blockValues,[dim dim]);
        avg_gray              = qtsetblk(avg_gray,S,dim,avg_values);

        % fill average image (rgb)
        for ch=1:size(imargb,3)
        [blockValues, Sind]   = qtgetblk(imargb(:,:,ch), S, dim);
         avg_values           = repmat(blockValues,[dim dim]);
        avg_rgb(:,:,ch)       = qtsetblk(avg_rgb(:,:,ch),S,dim,avg_values);
        end
    
    end
     
    
  end
  
end

% image extrema are  boundaries!
blocks(end,1:end) = 1;
blocks(1:end,end) = 1;

% show results
leaves_image        =  zeros(size(imargb),'double');
leaves_image(:,:,2) =  0.8.*double(leaves);
compose_image       =  double(imargb.*double(repmat(~leaves,[1,1,3])) + leaves_image);

subplot_tight(2,2,1),imshow(compose_image), title('Image with leaves (in green)')
subplot_tight(2,2,2),imshow(blocks,[]), title('QT edges on gray image')
subplot_tight(2,2,3),imshow(avg_gray,[]), title('QT regions: average gray values')
subplot_tight(2,2,4),imshow(avg_rgb,[]), title('QT regions: average color values')