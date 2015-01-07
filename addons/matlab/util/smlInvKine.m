function Q = smlInvKine(base,tool,T_w_tool)
% smlInvKine    Computes the inverse kinematics for a PA-10 in SceneML
%
%   USAGE:
%       Q = smlInvKine(base, tool, Td)
%   
%   INPUTS:
%       base - Name of the robot's base body (e.g. src0)
%
%       tool - Name of the body attached to the robot's end effector (e.g.
%       xray_tube)
%
%       T_w_tool - The desired 4x4 tranformation matrix of the end 
%       effector body expressed in the world coordinate system. Several 
%       utility functions are provided to help compute this matrix.
%
%   OUTPUTS:
%       Q - row matrix of all possible, and feasible, joint angle solutions
%
%   EXAMPLE:
%       % Load a scene with a PA-10 in it
%       sceneInit('demo/twopa10.scene');
%       % Now let's put the X-ray tube body somewhere interesting
%       T_tube = transl(600, 200, 1000) * rotz(pi/6) * roty(pi/2);
%       % Get solutions, if any exist
%       Q = smlInvKine('src0', 'xray_tube', T_tube);
%       % If we got any solutions, lets take a look at them
%       varnames = {'q_src_1'; 'q_src_2'; 'q_src_3'; 'q_src_4'; 'q_src_5'; 'q_src_6'};
%       for n = 1:size(Q,1)
%           sceneSetVars(varnames, Q(n,:)');
%           sceneRender();
%           pause;
%       end
%
%   SEE ALSO: rotx, roty, rotz, transl, sceneInit, sceneRender,
%   sceneSetVars
%
%       Author: J.D. Yamokoski
%       Modified: 2/11/2008

% SceneML, Copyright (C) 2007, 2008  J.D. Yamokoski
% All rights reserved.
% Email: yamokosk at gmail dot com
%
% This library is free software; you can redistribute it and/or
% modify it under the terms of the GNU Lesser General Public License as
% published by the Free Software Foundation; either version 2.1 of the License, 
% or (at your option) any later version. The text of the GNU Lesser General 
% Public License is included with this library in the file LICENSE.TXT.
%
% This library is distributed in the hope that it will be useful, but WITHOUT 
% ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
% or FITNESS FOR A PARTICULAR PURPOSE. See the file LICENSE.TXT for 
% more details.

% Get the location of the base and tool frame from the scene
b_base = sceneGetBody(base);
b_tool = sceneGetBody(tool);

T_w_1 = b_base.T_world_body;
T_6_tool = b_tool.T_prox_body;

T_1_6 = inv_tmatrix(T_w_1) * T_w_tool * inv_tmatrix(T_6_tool);
Q = ikine_pa10(T_w_1, T_1_6);

% IKINE_PA10    Computes the inverse kinematic solution for the PA-10
%               robotic manipulator.
%
%   USAGE:
%       Q = IKINE_PA10(T_w_1, T_1_6)
%   
%   INPUTS:
%       T_w_1 - Transformation matrix from the world coordinate system to
%       the robot's base frame.
%
%       T_1_6 - Transformation matrix from the robot's 1st frame to the 
%       last frame
%
%   OUTPUTS:
%       Q - row matrix of all possible, and feasible, solutions
%
%   REFERENCE:
%       See Crane & Duffy's book for more details on the analytic solution
%       method.
%
%       Author: J.D. Yamokoski
%       Modified: 2/11/2008
function Q = ikine_pa10(T_w_1, T_1_6) 
%--------------------------------------------------------------------------
% Step 1. DH parameters - hard coded for the PA-10
%--------------------------------------------------------------------------
% Link 12
alp12 = -pi/2;

% Link 23
a23 = 450;    
alp23 = 0;     

% Link 34
alp34 = pi/2;

% Link 45
alp45 = -pi/2;
S4 = 480;

% Link 56
alp56 = pi/2;

% Link 67
alp67 = pi/2; % This should come from the t-matrix from 6 to tool - but in reality, only pi/2 seems to work??
S6 = 70;

% Pre-compute the constant mechanism parameters to find cj, sj, cij, and sij.
c12 = cos(alp12); s12 = sin(alp12);
c23 = cos(alp23); s23 = sin(alp23);
c34 = cos(alp34); s34 = sin(alp34);
c45 = cos(alp45); s45 = sin(alp45);
c56 = cos(alp56); s56 = sin(alp56);
c67 = cos(alp67); s67 = sin(alp67);

%--------------------------------------------------------------------------
% Step 2. Create a virtual closed-loop mechanism so that we can write a
% vector loop equation.
%
%   Vector-loop equation for PA-10:
%   S1*S1_vec + a23*a23_vec + S4*S4_vec + S6*S6_vec + S7*S7_vec + a71*a71_vec = 0_vec
%--------------------------------------------------------------------------
S1_f = T_w_1(1:3,3);
[a71, S7, S1, alp71, th7, gam1] = close_loop(T_1_6, S1_f, 317);

c7 = cos(th7); s7 = sin(th7);
c71 = cos(alp71); s71 = sin(alp71);

X7 = s67*s7;
Y7 = -(s71*c67 + c71*s67*c7); % alph67!!!
Z7 = c71*c67 - s71*s67*c7; % alph67!!!

%--------------------------------------------------------------------------
% Step 3. Solve for theta_1 and phi_1
%
%   There should be two valid solutions for theta_1 (phi_1). Therefore this
%   creates the first branch in our tree of solutions.
%--------------------------------------------------------------------------
% Expand z-component of vector loop eqn and collect s1 and c1 terms to get
%   A*c1 + B*s1 + D = 0
A = -S6*Y7 + S7*s71;
B = -S6*X7 - a71;
D = S1*c12; % Should be equal to zero

[ang, numSolutions, success] = solve_trig(A, B, D);
Q = [];
if ~success
    % If the above did not work out, then there is no point in continuing.
    %if verbose, disp('Error! Encountered bad solutions at theta_1!'); end
    angles = [];
    good_soln = zeros(PossibleNumSolns,1);
    return;
end

% Allocate memory for the rest of the solutions depending on whether
% solving the above equation yielded 1 or 2 solutions. (It should always
% yield two solutions).
PossibleNumSolns = 4 * numSolutions;
angles = zeros(PossibleNumSolns,6);
ph1 = zeros(PossibleNumSolns,1); th2 = zeros(PossibleNumSolns,1);
th3 = zeros(PossibleNumSolns,1); th4 = zeros(PossibleNumSolns,1);
th5 = zeros(PossibleNumSolns,1); th6 = zeros(PossibleNumSolns,1);
%th7 = zeros(PossibleNumSolns,1); th8 = zeros(PossibleNumSolns,1);
th1 = zeros(PossibleNumSolns,1);
good_soln = zeros(PossibleNumSolns,1);

% Assign theta_1
good_soln(:,1) = success;
ph1(1:4,1) = (ang(1) - gam1); ph1(5:8,1) = (ang(2) - gam1);
th1(1:4,1) = ang(1); th1(5:8,1) = ang(2);

% -------------------------------------------------------------------------
% Solutions for the rest of the joint angles
%   The rest of this program consists of a various number of nested FOR and
%   IF statements. The purpose of this code is to traverse the entire
%   solution tree.
%
%   The if statments are used to primarily set and check the status of the
%   good_soln array. If there is a problem calculating any of the joint
%   angles at any part of the tree the problematic joint angle will set the
%   appropriate sections of good_soln = 0 which will essentially cancel
%   calculation of the tree from that angle forward.
% -------------------------------------------------------------------------

%--------------------------------------------------------------------------
% Step 4. Solve for theta_3
%
%   There are two possible solutions for theta_3. Thus creating the second
%   branch in the tree of solutions.
%--------------------------------------------------------------------------
for th1_index = [1, 5]
    c1 = cos(th1(th1_index,1)); s1 = sin(th1(th1_index,1));

    X1 = s71*s1;
    Y1 = -(s12*c71 + c12*s71*c1);
    %Z1 = c12*c71 - s12*s71*c1;

    X71 = X7*c1 - Y7*s1;
    Y71 = c12*(X7*s1 + Y7*c1) - s12*Z7;
    Z71 = s12*(X7*s1 + Y7*c1) + c12*Z7;

    A = -S6*X71 - S7*X1 - a71*c1;
    B = -S1*s12 + S6*Y71 + S7*Y1 + a71*s1*c12;

    s3 = (A^2 + B^2 - a23^2 - S4^2)/(2*S4*a23);
    
    if ( isabout(abs(s3), 1, 1e-6) ), s3=1.0; end;

    if abs(s3) > 1.0
        %if verbose, disp('Encountered bad solutions at th3.'); end
        good_soln(:,1) = 0;
        return;
    end

    th3(th1_index:th1_index + 1,1) = asin(s3);
    th3(th1_index+2:th1_index+3,1) = pi - asin(s3);


    for th3_index = [th1_index, th1_index+2]

        %----------------------------------------------------------------------
        % Step 5. Solve for theta_2
        %
        %   There is one unique solution for theta_2.
        %----------------------------------------------------------------------
        c3 = cos(th3(th3_index,1)); s3 = sin(th3(th3_index,1));

        M(1,1) = a23 + S4*s3;   M(1,2) = S4*c3;
        M(2,1) = -S4*c3;        M(2,2) = a23 + S4*s3;

        b(1,1) = -S6*X71 - S7*X1 - a71*c1;
        b(2,1) = -S1*s12 + S6*Y71 + S7*Y1 + a71*s1*c12;

        if ( rank([M b]) < 2)
            %if verbose, disp('Encountered some bad solutions at th2.'); end;
            good_soln(th3_index:th3_index+1,1) = 0;
            continue;
        end

        x = M \ b;
        c2 = x(1,1); s2 = x(2,1);

        th2(th3_index:th3_index+1,1) = atan2(s2,c2);

        %----------------------------------------------------------------------
        % Step 6. Solve for theta_5
        %
        %   There are two possible solutions for theta_5 for each value of
        %   theta_3. This creates the second branch in the tree of solutions.
        %----------------------------------------------------------------------
        X712 = X71*c2 - Y71*s2;
        Y712 = c23*(X71*s2 + Y71*c2) - s23*Z71;
        Z712 = s23*(X71*s2 + Y71*c2) + c23*Z71;

        X7123 = X712*c3 - Y712*s3;
        Y7123 = c34*(X712*s3 + Y712*c3) - s34*Z712;
        Z7123 = s34*(X712*s3 + Y712*c3) + c34*Z712;

        c5 = -(Z7123 - c45*c56)/(s45*s56);

        if ( isabout( abs(c5), 1, 1e-6 ) ), c5 = 1.0; end;

        if abs(c5) > 1.0
            %if verbose, disp('Encountered some bad solutions at th5.'); end;
            good_soln(th3_index:th3_index+1,1) = 0;
            continue;
        end

        th5(th3_index,1) = acos(c5);
        th5(th3_index+1,1) = -acos(c5);

        for th5_index = [th3_index, th3_index+1]

            c5 = cos(th5(th5_index,1));
            s5 = sin(th5(th5_index,1));

            %------------------------------------------------------------------
            % Step 7. Solve for theta_4
            %
            %   There is one unique solution for theta_4.
            %------------------------------------------------------------------
            X5_bar = s56*s5;
            Y5_bar = -(s45*c56 + c45*s56*c5);

            M(1,1) = X5_bar;    M(1,2) = -Y5_bar;
            M(2,1) = Y5_bar;    M(2,2) = X5_bar;

            b(1,1) = X7123;
            b(2,1) = -Y7123;

            if ( rank([M b]) < 2)
                %if verbose, disp('Encountered some bad solutions at th4.'); end;
                good_soln(th5_index,1) = 0;
                continue;
            end

            x = M \ b;
            c4 = x(1); s4 = x(2);

            th4(th5_index,1) = atan2(s4,c4);

            %------------------------------------------------------------------
            % Step 8. Solve for theta_6
            %
            %   There is one unique solution for theta_6.
            %------------------------------------------------------------------
            bX4 = s45*s4;
            bY4 = -(s34*c45 + c34*s45*c4);
            bZ4 = c34*c45 - s34*s45*c4;

            X43 = bX4*c3 - bY4*s3;
            Y43 = c23*(bX4*s3 + bY4*c3) - s23*bZ4;
            Z43 = s23*(bX4*s3 + bY4*c3) + c23*bZ4;

            X432 = X43*c2 - Y43*s2;
            Y432 = c12*(X43*s2 + Y43*c2) - s12*Z43;
            Z432 = s12*(X43*s2 + Y43*c2) + c12*Z43;

            X4321 = X432*c1 - Y432*s1;
            Y4321 = c71*(X432*s1 + Y432*c1) - s71*Z432;
            Z4321 = s71*(X432*s1 + Y432*c1) + c71*Z432;

            X43217 = X4321*c7 - Y4321*s7;
            Y43217 = c67*(X4321*s7 + Y4321*c7) - s67*Z4321;

            c6 = Y43217/s56;
            s6 = X43217/s56;

            th6(th5_index,1) = atan2(s6,c6);
        end
    end
end

%qOffsets = Robot.offset;

angles(:,1) = ph1;
angles(:,2) = th2 - (-pi/2);
angles(:,3) = th3 - pi/2;
angles(:,4) = th4;
angles(:,5) = th5;
angles(:,6) = th6;

% Check each solution to see if it is outside its joint limits
%qlimits = Robot.qlim;
qmin = [-177, -64, -107, -255, -165, -255] * (pi/180);
qmax = [177, 124, 158, 255, 165, 255] * (pi/180);

for ii = 1:PossibleNumSolns
    for jj = 1:6
                
        % Check angle against upper range of joint
        if ( angles(ii,jj) > qmax(jj) )
            % Flip angle around unit circle. Check if this new angle
            % exceeds the lower range of the joint.
            angles(ii,jj) = angles(ii,jj) - 2*pi;
            
            if ( angles(ii,jj) < qmin(jj) )
                good_soln(ii) = 0;
                break;
            end            
        end
        
        % Check angle against lower range of joint
        if ( angles(ii,jj) < qmin(jj) )
            % Flip angle around unit circle. Check if this new angle
            % exceeds the upper range of the joint.
            angles(ii,jj) = 2*pi + angles(ii,jj);
            
            if ( angles(ii,jj) > qmax(jj) )
                good_soln(ii) = 0;
                break;
            end            
        end
    end
end

GoodIndex = find( good_soln == 1);
Q = angles(GoodIndex,:);


% CLOSE_LOOP    Determines the closed-loop parameters for an open-loop
%               chain.
%
%   USAGE:
%   [a71, S7, S1, alp71, th71, gam1] = CLOSE_LOOP(T_1_6, S1_f, S1_Offset)
%   
%   INPUTS:
%       T_1_6 - Transformation matrix from the robot's 1st frame to the 
%       tool frame
%
%       S1_f - S1 vector expressed in the world coordinate system
%
%       S1_offset - 1st 'D' DH parameter
%
%   OUTPUTS:
%       a71 and S7 are all scalar values for the distances to travel 
%       along the imaginary close-loop 7th frame x and z axes respectively.
%
%       S1 is the distance to travel from a71 to a12.
%
%       alp71, th7, gam1 are angles use to specify directions for the new
%       7th frame.
%
%   REFERENCE:
%       See Crane & Duffy's book for a more detailed explanation of these
%       parameters.
%
%       Author: J.D. Yamokoski
%       Modified: 2/11/2008
function [a71, S7, S1, alp71, th7, gam1] = close_loop(T_1_6, S1_f, S1_Offset)
S6_f = T_1_6(1:3,3);
a67_f = T_1_6(1:3,1);
P6_f = T_1_6(1:3,4);

% Now we can also define the z-axis unit vectors for the first and 7th
% frames.
S7_f = cross3(a67_f, S6_f);
S7_f = S7_f/norm(S7_f);

% At this point we can go no further without doing some checks. We will use
% cos(alpz1) [c71] to test if S1_f and S7_f are parrallel
c71 = dot3(S7_f,S1_f);

% Check the first special case:
if ( isabout(abs(c71),1,1e-6) ) % S1 and S7 parrallel?
    % If S1_f and S7_f are parrallel, then we will just set S7 to zero and
    % continue on with calculations.
    s71 = 0;
    alp71 = atan2(s71,c71);
    
    % Calculate the scalar distances for the parrallel case
    S7 = 0;
    S1 = -1*dot3(P6_f,S1_f) + S1_Offset; % Add any existing S1
    a71 = norm(-1*(P6_f + S1 * S1_f));
    
    % Here we have our second and final special case. If the scalar
    % distance a71 is zero, then that implies that not only are S1_f and 
    % S7_f parrallel (condition required to get into this part of the if
    % statement) but they are also collinear! So let's now handle that 
    % case:
    if isabout(abs(a71),0,1e-6)
        %disp('S1_f and S7_f are collinear!');
        a71 = 0;
        th7 = 0;
        a71_f = a67_f; % Since th7 = 0, these are now parrallel.
    else
        % Finish calculations for just parrallel case
        %disp('S1_f and S7_f are parrallel!'); % Can make this statement with confidence now.
        a71_f = (-1*(P6_f + S1 * S1_f)) * (1/a71);

        % With a71_f we can now get theta7 (th7)
        c7 = dot3(a67_f,a71_f);
        s7 = dot3(cross3(a67_f,a71_f),S7_f);
        th7 = atan2(s7,c7);
    end
else % Normal case
    a71_f = cross3(S7_f,S1_f) * inv(norm(cross3(S7_f,S1_f)));
    
    % With a71_f we can now get theta7 (th7)
    c7 = dot3(a67_f,a71_f);
    s7 = dot3(cross3(a67_f,a71_f),S7_f);
    th7 = atan2(s7,c7);
    
    % Now we have checked special cases, and by this time we should have the
    % correct a71_f vector so we can finish calculation of alp71.
    s71 = dot3(cross3(S7_f,S1_f),a71_f);
    alp71 = atan2(s71,c71);
    
    % Finally get the three scalar distances for the normal case.
    S7 = dot3(cross3(S1_f,P6_f),a71_f)/s71;
    a71 = dot3(cross3(P6_f,S1_f),S7_f)/s71;
    S1 = (dot3(cross3(P6_f,S7_f),a71_f)/s71) + S1_Offset; % Add any existing S1
end

% Calculation of gam1 the same for all cases.
cgam1 = dot3(a71_f,[1,0,0]');
sgam1 = dot3(cross3(a71_f,[1,0,0]'),S1_f);
gam1 = atan2(sgam1,cgam1);


function answer = isabout(var, value, tol)
if (abs(var - value) < tol)
    answer = true;
else
    answer = false;
end


%   SOLVE_TRIG     Determines coefficients of trigonometric equation
%       SOLVE_TRIG(A, B, D) returns the two solutions (ANG_A
%       and ANG_B) of the equation:
%
%           A*cos(x) + B*sin(x) + D = 0
%
%       SOLVE_TRIG will also return a Boolean 1 or 0 depending on the
%       success of the calculation. If it returns a 0, then no solution was
%       found and ANG_A and ANG_B = [0]
function [ang, numSolutions, success] = solve_trig(A, B, D)
if ( abs(D) < 1e-9 ) D=0; end;

ang = zeros(2,1);

if ( D == 0 ) % Special case
    ang(1,1) = atan2(-A,B);
    ang(2,1) = atan2(A,-B);
    numSolutions = 2;
    success = true;
else
    sgam = B/sqrt(A^2 + B^2);
    cgam = A/sqrt(A^2 + B^2);
    
    gam = atan2(sgam,cgam);
    
    RHS = -D/sqrt(A^2 + B^2);
    
    if (abs(RHS) <= 1)
        LHS1 = acos(RHS);
        LHS2 = -acos(RHS);
    
        ang(1,1) = LHS1 + gam;
        ang(2,1) = LHS2 + gam;
        numSolutions = 2;
        success = true;
    else
        ang = [];
        numSolutions = 0;
        success = false;
    end
end

% For vectors u==(u_x,u_y,u_z)  and v==(v_x,v_y,v_z) in R^3, the cross 
% product is defined by
%   u x v =	(u_y * v_z - u_z * v_y)*i +
%           (u_z * v_x - u_x * v_z)*j + 
%           (u_x * v_y - u_y * v_x)*k
function out = cross3(u,v)
out = zeros(3,1);
out(1) = u(2) * v(3) - u(3) * v(2);
out(2) = u(3) * v(1) - u(1) * v(3);
out(3) = u(1) * v(2) - u(2) * v(1);

function out = dot3(u,v)
out = u(1)*v(1) + u(2)*v(2) + u(3)*v(3);

function tinv = inv_tmatrix(t)
tinv = eye(4);
tinv(1:3,1:3) = t(1:3,1:3)';
tinv(1:3,4) = - tinv(1:3,1:3) * t(1:3,4);