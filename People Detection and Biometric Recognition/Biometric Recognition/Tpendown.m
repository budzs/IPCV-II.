function Tpd = Tpendown(p)
    counter = 0;
    for i = 1: length(p)
        if p(i)~= 0
            counter=counter+1;
        end
    end
    Tpd = counter / length(p);
end