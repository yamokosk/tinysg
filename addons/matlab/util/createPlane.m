function fv = createPlane(p, params)
% TODO: Write help for this function
% T - Transformation matrix
% p - [xmin, xmax, ymin, ymax]
% params - a,b,c,d of the plane equation
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

xmin = p(1); xmax = p(2); ymin = p(3); ymax = p(4);
a = params(1);  b = params(2); c = params(3); d = params(4);
z = zeros(1,4);
z(1) = (d - a*xmax - b*ymax)/c;
z(2) = (d - a*xmin - b*ymax)/c;
z(3) = (d - a*xmin - b*ymin)/c;
z(4) = (d - a*xmax - b*ymin)/c;

% Non-homogeneous vertices
fv.vertices = [xmax, xmin, xmin, xmax, xmax, xmin, xmin, xmax; ...
               ymax, ymax, ymin, ymin, ymax, ymax, ymin, ymin; ...
                z(1), z(2), z(3), z(4), z(1)-10, z(2)-10, z(3)-10, z(4)-10]';

% Create a new patch object
fv.faces = [1, 2, 3, 4; 5, 6, 7, 8; 4, 3, 7, 8; 1, 4, 8, 5; 1, 2, 6, 5; 2, 3, 7, 6];
