% FDLT_Norm  The normalized DLT 8-point algorithm for F (fundamental matrix) 
% Algorithm  10.1 p.265 from  Multiple View (Hartley)
%
% Input: point correspondences (P,Q)
%  - P(3,npoints) : hom. coords of the points in the first image
%  - Q(3,npoints) : hom. coords of the points in the second image
%
% Output:
%  - F(3,3) : fundamental matix computed using SVD or eigen-values/vectors of (A'*A)
%  - cost: value of the smallest singular value of F


function [F,cost]=FDLT_Norm(P,Q)

    % 1. Affine normalization of the points in the first image
    [Pn,Tp] = NormalizAfin(P);

    % 2. Affine normalization of the points in the second image
    [Qn,Tq] = NormalizAfin(Q);

    % ------------------------------------------------------------------------
    % Create matrix A that represents the homogeneous system Af=0. Vector f contains the coefficients of the fundamental matrix
    % A is a matrix with as many rows as point correspondences and 9 columns
    % ------------------------------------------------------------------------
    npoints=size(P,2);
    A=zeros(npoints,9);
    for i=1:npoints
        x = Pn(1,i);
        y = Pn(2,i);
        u = Qn(1,i);
        v = Qn(2,i);
        A(i,:) = [u*x, u*y, u, v*x, v*y, v, x, y, 1];
    end

    
   
    % ----------------------------------------------------------------
    % SVD solution for F. Use svd() To reshape a column vector Vect into a 3x3
    % matrix use reshape(Vect,3,3).';
    % ----------------------------------------------------------------
    [~,D,V] =  svd(A);
    F=V(:,end);
    F=reshape(F, 3, 3).';
    
    if (size(P,2)==8) 
        cost=0; 
    else
        S = diag(D);
        cost = S(9);
    end
    disp(['Minimum singular value = ',num2str(cost)]);
       
    % ----------------------------------------------------------------
    % Enforce rank 2 for F
    % ----------------------------------------------------------------
    
    [U,D,V]=svd(F);
    D(:,3)=0;
    F=U*D*V';
    %F=D1;
    % 4. De-normalization
    F = normalize_matrix((Tq.')*F*Tp);
    
end
