function Ppd = Ppendown(p)
    sum = 0;
    counter = 0;
    for i = 1: length(p)
        if p(i)~= 0
            sum = sum + p(i);
            counter = counter + 1;
        end
    end
    Ppd = sum / counter;
end