

close all
clear all
clc

%BiosecurIDparameters matrix with: 50 (users) x 16 (signatures/user)
BiosecurIDparameters=cell(50,16);

usuario = 1;
%YOUR CODE

for user = 1:50
    temp_sesion = [];
    for session = 1:4
        temp_FeatVect = [];
        for sign_genuine_id = 1:4
            indexes=[1,2,6,7];
            sign_genuine=indexes(sign_genuine_id);
            %You could use this inside your code
            
            %This is how to load the signatures:  
            
            if usuario<10
                BiosecurID=load(['../DB/u100', num2str(user),'s000', num2str(session), '_sg000', num2str(sign_genuine), '.mat']);
            else
                BiosecurID=load(['../DB/u10', num2str(user),'s000', num2str(session), '_sg000', num2str(sign_genuine), '.mat']);
            end
            signature=(session-1)*4+sign_genuine_id;
            BiosecurIDparameters{user,signature}=localFeatureExtractor(BiosecurID);
        end

    end
    usuario = usuario + 1;

end
            
%YOUR CODE         
            
            
save('BiosecurIDparameters','BiosecurIDparameters');


