function [out] = localFeatureExtractor(signature)
    %x,y,p
    out{1}=signature.x;
    out{2}=signature.y;
    out{3}=signature.p;

    % minmax normalization of location
    for i =1:2
        out{i}=(out{i}-min(out{i})) / (max(out{i})-min(out{i}));
    end

    %Do we use absolute or relative normalization for the pressure?
    out{3}=out{3}/max(out{3}); % relative (depends on given max)
    %out{3}=out{3}*10e-4; % absolute (divide with max detection possible)

    %dx,dy,dp
    out{4}=diff(out{1});
    out{5}=diff(out{2});
    out{6}=diff(out{3});
    %ddx,ddy,ddp
    out{7}=diff(out{4});
    out{8}=diff(out{5});
    out{9}=diff(out{6});
end
