

close all
clear all
clc

%BiosecurIDparameters matrix with: 50 (users) x 16 (signatures/user) x 4 (params)
BiosecurIDparameters=ones(50,16,4);

usuario = 1;
%YOUR CODE

for user = 1:50
    temp_sesion = [];
    for sesion = 1:4
        temp_FeatVect = [];
        for sign_genuine = [1,2,6,7]
            
            %You could use this inside your code
            
            %This is how to load the signatures:  
            
            if usuario<10
                BiosecurID=load(['./DB/u100', num2str(user),'s000', num2str(sesion), '_sg000', num2str(sign_genuine), '.mat']);
            else
                BiosecurID=load(['./DB/u10', num2str(user),'s000', num2str(sesion), '_sg000', num2str(sign_genuine), '.mat']);
            end
            
            x=BiosecurID.x;
            y=BiosecurID.y;
            p=BiosecurID.p;
            
            temp_FeatVect = [temp_FeatVect; featureExtractor(x,y,p)];
            
        end
        temp_sesion = [temp_sesion; temp_FeatVect];
    end
    usuario = usuario + 1;
    BiosecurIDparameters(user,:,:) = temp_sesion;
end
            
%YOUR CODE         
            
            
save('BiosecurIDparameters','BiosecurIDparameters');

figure(1)
histogram(BiosecurIDparameters(:,:,1),'Normalization','probability');
title('Total duration');

figure(2)
histogram(BiosecurIDparameters(:,:,2),'Normalization','probability');
title('N pen-ups');

figure(3)
histogram(BiosecurIDparameters(:,:,3),'Normalization','probability');
title('Tpendown/T');

figure(4)
histogram(BiosecurIDparameters(:,:,4),'Normalization','probability');
title('Average P in pen-down');

