function fv = createSphere(T, r)
% TODO: Write help for this function
% T - Transformation matrix
% r - radius of the sphere
% fv - vertex/face structure
% NOTE - Body coordinate system placed and center of volume

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

[x y z] = sphere; 
fv = surf2patch(x,y,z,z);
NV = size(fv.vertices,1);

% Homogenous vertices - hv
hv = ones(4,NV);
hv(1:3,:) = fv.vertices';

% Scaling to go from unit sphere to r
S = eye(4);
S(1,1) = r; S(2,2) = r; S(3,3) = r;

% Transformed vertices
hv = T*S*hv;
fv.vertices = hv(1:3,:)';