%% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% VIDEO FOR MULTIPLE AND MOVING CAMERAS (VMMC)
% IPCV @ UAM
% Marcos Escudero-Viñolo (marcos.escudero@uam.es)
% assumes factor = 2;
%% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function [PB,range]=doBandPassPyramid(PG,g)
%% obtain number of levels
nlevels     = numel(PG)-1;
%% initialise pyramid
PB          = cell(1, nlevels);
%% define 2D Gaussian filter
g       = g'*g;
%% Create pyramid
% note that imfilter can be used instead of conv due to the symmetry of g
range = [Inf, -Inf];
for lv = 1:1:nlevels   
    AUX      = ExpandLevel(PG{lv+1},g,size(PG{lv}));
    PB{lv}   = PG{lv} - AUX;
    range(1) = min(range(1),min(PB{lv}(:))); % to show results later
    range(2) = max(range(2),max(PB{lv}(:))); % to show results later
end

end    

function Iout  = ExpandLevel(Iin,g,sizeout)

Iout = zeros(sizeout);
% expand [Iin] with 4 kernels
g     = 4.*g;
[M] = floor(size(g,1)./2);
[N] = floor(size(g,2)./2);
ker00 = g(1:2:end,1:2:end);
ker01 = g(1:2:end,2:2:end);
ker10 = g(2:2:end,1:2:end); 
ker11 = g(2:2:end,2:2:end); 

img00  = imfilter(Iin,ker00,'replicate','same');
img01  = imfilter(Iin,ker01,'replicate','same');
img10  = imfilter(Iin,ker10,'replicate','same');
img11  = imfilter(Iin,ker11,'replicate','same');
Iout(1:2:end,1:2:end) = img00;
Iout(2:2:end,1:2:end) = img10;
Iout(1:2:end,2:2:end) = img01;
Iout(2:2:end,2:2:end) = img11;


end