function Score=Matcher(test,Model)
%YOUR CODE

if size(Model,1) == 1
    Score = pdist2(Model,test,'euclidean');
else
    D = 0;
    for i = 1:size(Model,1)
        D = D + pdist2(Model(i,:),test,'euclidean');
    end
    Score = D / size(Model,1);
end 



end