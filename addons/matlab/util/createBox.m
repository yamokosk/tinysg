function fv = createBox(T, dim)
% TODO: Write help for this function
% T - Transformation matrix
% dim - dimensions of the box
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

lx = dim(1)*0.5;
ly = dim(2)*0.5;
lz = dim(3)*0.5;

% Homogeneous vertices
hv = [-lx,  lx,  lx, -lx, -lx,  lx, lx, -lx; ...
      -ly, -ly,  ly,  ly, -ly, -ly, ly,  ly; ...
      -lz, -lz, -lz, -lz,  lz,  lz, lz,  lz; ...
        1,   1,   1,   1,   1,   1,  1,   1];
v = T*hv; % Transformed

% Non-homogeneous vertices
fv.vertices = v(1:3,:)';

% Create a new patch object
fv.faces = [1, 2, 6, 5; ...
            2, 3, 7, 6; ...
            3, 4, 8, 7; ...
            4, 1, 5, 8; ...
            1, 2, 3, 4; ...
            5, 6, 7, 8];
