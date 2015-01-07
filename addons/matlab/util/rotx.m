%ROTX Rotation about X axis
%
%	TR = ROTX(theta)
%
% Returns a homogeneous transformation representing a rotation of theta 
% about the X axis.
%
% See also: ROTY, ROTZ, ROTVEC.

% $Log: rotx.m,v $
% Revision 1.2  2002/04/01 11:47:16  pic
% General cleanup of code: help comments, see also, copyright, remnant dh/dyn
% references, clarification of functions.
%
% $Revision: 1.2 $
% Copyright (C) 1993-2002, by Peter I. Corke

function r = rotx(t)
%ct = cos(t);
%st = sin(t);
%r = eye(4);
% 	r =    [1	0	0	0
% 		0	ct	-st	0
% 		0	st	ct	0
% 		0	0	0	1];
r(2,2) = cos(t);
r(2,3) = -sin(t);
r(3,2) = sin(t);
r(3,3) = cos(t);
r(1,1) = 1;