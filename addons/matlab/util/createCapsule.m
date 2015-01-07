function fv = createCapsule(T, len, r)
% TODO: Write help for this function
% T - Transformation matrix
% len - length of the capsule
% r - radius of the capsule
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

cyl = createCylinder(T,len,r);

% Place the spheres at the ends of the cylinder
F1 = eye(4); F2 = eye(4);
F1(3,4) = len/2; F2(3,4) = -len/2;
sp1 = createSphere(T*F1,r);
sp2 = createSphere(T*F2,r);

% Concatenate vertices
NVCYL = size(cyl.vertices,1);
NVSP1 = size(sp1.vertices,1);
fv.faces = [cyl.faces; sp1.faces + NVCYL; sp2.faces + NVCYL + NVSP1];
fv.vertices = [cyl.vertices; sp1.vertices; sp2.vertices];