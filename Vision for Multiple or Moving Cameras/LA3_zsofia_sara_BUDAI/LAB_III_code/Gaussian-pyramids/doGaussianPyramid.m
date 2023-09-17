%% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% VIDEO FOR MULTIPLE AND MOVING CAMERAS (VMMC)
% IPCV @ UAM
% Marcos Escudero-Viñolo (marcos.escudero@uam.es)
%% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function PG=doGaussianPyramid(ima,factor,nlevels,g)
%% check if the required number of levels is plausible
[nr,nc]     = size(ima);
max_nlevels = floor(log2(min(nr,nc)));
nlevels     = min(max_nlevels,nlevels);
%% initialise pyramid
PG          = cell(1, nlevels);
PG{1}       = ima;
%% Create pyramid
% note that imfilter can be used instead of conv due to the symmetry of g
for lv = 2:1:nlevels
    PG{lv} = imfilter(imfilter(PG{lv-1},g,'symmetric','same'),g','symmetric','same');
    PG{lv} = PG{lv}(1:factor:end,1:factor:end);
end
%% NOTE: alternatively one can define g as a 2D filter and call imfilter just once
end    