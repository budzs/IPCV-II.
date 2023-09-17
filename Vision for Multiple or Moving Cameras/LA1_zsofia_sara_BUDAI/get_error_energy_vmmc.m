function err_i = get_error_energy_vmmc(I1, I2)
% GET ERROR ENERGY returns the error of the luminance difference image I1-I2.
%
% Jesus Bescos, Universidad Autonoma de Madrid, November 2012

if(length(size(I1))>2) I1=rgb2gray(I1); end;
if(length(size(I2))>2) I2=rgb2gray(I2); end;
I1=double(I1);
I2=double(I2);
err_i=sum(sum((I1-I2).*(I1-I2)));

%Añadido para devolver la potencia en vez de la energia, lo cual resulta
%mas util si siempre se compara con imagenes de igual tamaño

num_values=size(I1,1)*size(I1,2);
err_i=err_i/num_values;
