close all;clear;clc;addpath("../DB")

db_info=dir("../DB"); %~list dir

random_signature=db_info(3).name %chose filename

random_signature='u1001s0002_sg0001.mat'

load(random_signature) %loads x,y,p data for given signature

figure()
plot(x,y,"r")
title("y as a function of x")
ylabel("y (pixel)")
xlabel("x (pixel)")




t=linspace(0, length(x)/200,length(x));
figure()
plot(t,x,"r")
title("x as a function of t")
ylabel("x (pixel)")
xlabel("t (seconds)");xlim([0,length(x)/200])

figure()
plot(t,y,"r")
title("y as a function of t")
ylabel("y (pixel)")
xlabel("t (seconds)");xlim([0,length(x)/200])
figure()
plot(t,p,"r")
title("p as a function of t")
ylabel("p (pixel)")
xlabel("t (seconds)");xlim([0,length(x)/200])
