
%A_3 = internal_parameters_solve_vmmc({H_ref{1:3}});
% A_4 = internal_parameters_solve_vmmc({H_ref{1:4}});
A_5 = internal_parameters_solve_vmmc(H_ref);

%A_3_origin = internal_parameters_solve_vmmc({H{1:3}});
% A_4_orign = internal_parameters_solve_vmmc({H{1:4}});
% A_5_orign = internal_parameters_solve_vmmc(H);

M = ["u_axis" , A_5(1,1)]';
M = [M,["v_axis" , A_5(2,2)]'];
M = [M,["gamma" , A_5(1,2)]'];
M = [M,["u_0" , A_5(1,3)]'];
M = [M,["v_0" , A_5(2,3)]'];

[R, T]= external_parameters_solve_vmmc(A_5, H_ref);
RT_1 = [R{1},T{1}];
RT_2 = [R{2},T{2}];
RT_3 = [R{3},T{3}];
RT_4 = [R{4},T{4}];
RT_5 = [R{5},T{5}];

%%
InternalParameters_iPhoneXs = A_5
save("InternalParametersiPhoneXs.mat","InternalParameters_iPhoneXs")
