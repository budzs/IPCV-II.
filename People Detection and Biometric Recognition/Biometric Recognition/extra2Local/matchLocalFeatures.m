function [score] = matchLocalFeatures(features_test, cell_of_features_model)
  
    score=0;
    
    N=size(cell_of_features_model,2);

    for n_model = 1:N
        n_model_features=cell_of_features_model{n_model};
        dist=0;
        weights=[3,3,6,1,1,1,1,1,1]; %maybe the derivative or the secondary could be more important... needs empirical testing
        %take into account that it only allows to compare time functions of different lengths
        if size(features_test{1},2) == size(n_model_features{1},2)
            for i=1:9
                dist=dist+ sum(features_test{i}-n_model_features{i}) * weights(i);
            end
            k=size(features_test{1},2);
        else
            for i=1:9
                [temp_dist, ix, ~] = dtw(features_test{i},n_model_features{i});
                dist=dist+ temp_dist * weights(i);
            end
            k=size(ix,1);
        end
        D=dist/sum(weights);
        score=score+exp(-1*D/k);
    end
    score=score/N;
end