%% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% VIDEO FOR MULTIPLE AND MOVING CAMERAS (VMMC)
% IPCV @ UAM
% Marcos Escudero-Viñolo (marcos.escudero@uam.es)
%% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function SS=Linear13(I,sigmas)

%% Initialize scale-space structure
nscales     = numel(sigmas);
t0          = sigmas(1);
[nr,nc]     = size(I);
if min(nr,nc) == 1
    onedim =true;
else
    onedim = false;
end
SS          = cell(1, nscales);
SS{1}       = double(I);
%% define Gaussian filter
sigmas = [0, sigmas];

%% obtain scale-space representation guided by the Gaussian filter
for j = 2:1:(nscales)
    g     = createGaussianKernel(nr,nc,sigmas(j));
    if ~onedim
    SS{j} =  imfilter(imfilter(SS{j-1},g,'symmetric','same'),g','symmetric','same');
    else
    
    SS{j} =  imfilter(SS{j-1},g,'symmetric','same');
    end
end

end
