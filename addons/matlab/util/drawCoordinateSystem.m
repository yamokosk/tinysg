function drawCoordinateSystem(h, T, str, scale)
% TODO: Write help for this function

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

if nargin < 4
    scale = 100;
end

origin = T(1:3,4);
v = T * [scale,0,0,1]'; xpt = v(1:3,1); 
xaxis = [origin'; xpt'];

v = T * [0,scale,0,1]'; ypt = v(1:3,1);
yaxis = [origin'; ypt'];

v = T * [0,0,scale,1]'; zpt = v(1:3,1);
zaxis = [origin'; zpt'];

figure(h);
line(xaxis(:,1), xaxis(:,2), xaxis(:,3), 'Color', 'r');
line(yaxis(:,1), yaxis(:,2), yaxis(:,3), 'Color', 'g');
line(zaxis(:,1), zaxis(:,2), zaxis(:,3), 'Color', 'b');
text(origin(1), origin(2), origin(3), str);
