close all
clear all
clc

%addpath('DetPlots');
%load de signature parameters
load('BiosecurIDparameters.mat');

%obtain size of users and signatures/user
usuarios=size(BiosecurIDparameters,1);
firmas=size(BiosecurIDparameters,2);

%% GENUINE SyCORES
i=1; n=1;

GenuineScores=cell(1,3);
for N=[1 4 12]
    for us=1:usuarios
            %Extract the user model
            modelo=cell(1,N);
            for k=1:N
                modelo{k}=BiosecurIDparameters{us,k};
            end
            for n_test=N+1:firmas 
                %Test signatures: remaining signatures of the user
                test=BiosecurIDparameters{us,n_test};
                Score = matchLocalFeatures(test, modelo);
                GenuineScores{n}(us,i)= Score;
                i=i+1;
            end
        i=1;
    end
    n=n+1;
end

GenuineScores_1=GenuineScores{1};
GenuineScores_4=GenuineScores{2};
GenuineScores_12=GenuineScores{3};


%% IMPOSTOR SCORES

i=1; n=1;

ImpostorScores=cell(1,3);
for N=[1 4 12]
    
    for us=1:usuarios
        here=[N,us]
            %Extract the user model
            modelo=cell(1,N);
            for k=1:N
                modelo{k}=BiosecurIDparameters{us,k};
            end
            for n_test=1:usuarios 
                
                if (n_test~=us) %the score is only computed using the other users
                    %First signature of the other user
                    test=BiosecurIDparameters{n_test,1}; %Always the first signature
                    Score = matchLocalFeatures(test, modelo);
                    ImpostorScores{n}(us,i)= Score; %50x49 for any N
                    i=i+1;
                end
            end
        i=1;
    end
    n=n+1;
end

ImpostorScores_1=ImpostorScores{1};
ImpostorScores_4=ImpostorScores{2};
ImpostorScores_12=ImpostorScores{3};




%% YOUR CODE
%here these are not needed, as the scores are already "good :D"
% GenuineScores_1 = 1./(GenuineScores_1+0.00000001);
% GenuineScores_4 = 1./(GenuineScores_4+0.00000001);
% GenuineScores_12 = 1./(GenuineScores_12+0.00000001);
% 
% ImpostorScores_1 = 1./(ImpostorScores_1+0.00000001);
% ImpostorScores_4 = 1./(ImpostorScores_4+0.00000001);
% ImpostorScores_12 = 1./(ImpostorScores_12+0.00000001);

figure;
[EER1,DCF_opt1,ThresEER1]=Eval_Det(GenuineScores_1(:)',ImpostorScores_1(:)','b');
figure;
[EER4,DCF_opt4,ThresEER4]=Eval_Det(GenuineScores_4(:)',ImpostorScores_4(:)','b');
figure;
[EER12,DCF_opt12,ThresEER12]=Eval_Det(GenuineScores_12(:)',ImpostorScores_12(:)','b');


