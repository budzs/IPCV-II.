clear
close all;

ima = imread("hopper_vanishing.bmp");
ref_ima = imread('hopper_reference.bmp');
xy_origin = get_user_points_vmmc(ima);
xy_target = get_user_points_vmmc(ref_ima);
%%
H = homography_solve_vmmc(xy_origin, xy_target);


% ref image 

A = [xy_target(1,1), xy_target(2,1)];
B = [xy_target(1,2), xy_target(2,2)];
C = [xy_target(1,3), xy_target(2,3)];
D = [xy_target(1,4), xy_target(2,4)];

x = linspace(0, size(ref_ima, 2));

m1 = (B(2)-A(2))/(B(1)-A(1));
c1 = B(2) - m1*B(1);
y1 = m1*x + c1;

m2 = (C(2)-B(2))/(C(1)-B(1));
c2 = C(2) - m2*C(1);
y2 = m2*x + c2;

m3 = (C(2)-D(2))/(C(1)-D(1));
c3 = C(2) - m3*C(1);
y3 = m3*x + c3;

m4 = (D(2)-A(2))/(D(1)-A(1));
c4 = D(2) - m4*D(1);
y4 = m4*x + c4;

figure(1)
subplot(1,3,1)
imshow(ref_ima);
title("original");
hold on;
line(x,y1,'Color','g');
line(x,y2,'Color','g');
line(x,y3,'Color','g');
line(x,y4,'Color','g');

% ima 
% y=mx+c

A = [xy_origin(1,1), xy_origin(2,1)];
B = [xy_origin(1,2), xy_origin(2,2)];
C = [xy_origin(1,3), xy_origin(2,3)];
D = [xy_origin(1,4), xy_origin(2,4)];

x = linspace(0, size(ima, 2));

m1 = (B(2)-A(2))/(B(1)-A(1));
c1 = B(2) - m1*B(1);
y1 = m1*x + c1;

m2 = (C(2)-B(2))/(C(1)-B(1));
c2 = C(2) - m2*C(1);
y2 = m2*x + c2;

m3 = (C(2)-D(2))/(C(1)-D(1));
c3 = C(2) - m3*C(1);
y3 = m3*x + c3;

m4 = (D(2)-A(2))/(D(1)-A(1));
c4 = D(2) - m4*D(1);
y4 = m4*x + c4;

subplot(1,3,2);
imshow(ima);
title("graphical")
hold on;
line(x,y1,'Color','g');
line(x,y2,'Color','g');
line(x,y3,'Color','g');
line(x,y4,'Color','g');

line1= [[x(2), y1(2)]; [x(10), y1(10)]; [x(2), y3(2)];[x(10), y3(10)]];
line2= [[x(2), y2(2)]; [x(10), y2(10)]; [x(2), y4(2)];[x(10), y4(10)]];

P1 = lineintersect(line1);
P2 = lineintersect(line2);


m = (P2(2)-P1(2))/(P2(1)-P1(1));
b = P2(2) - m*P2(1);

y5 = m*x + b;
line(x,y5,'Color','r');


subplot(1,3,3);
imshow(ima);
hold on
x=linspace(0,size (ima,2));
y = (H(3,1)*x + H(3,3)) / (- H(3,2));
line(x, y, "Color", "g");
title('computed from homography');





