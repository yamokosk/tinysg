function sceneDemo1

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

sceneInit('twoGeomTest.scene');
names = {'sp1'; 'sp2'; 'sp3'; 'sp4';'sp5';'sp6';'sp7';'sp8';'sp9';'sp10';'sp11';'sp12';'sp13';'sp14';'sp15'};
rtrans = mat2cell(rand(15,3)*400 - 200, ones(15,1), 3);
sceneSetVars( [names, rtrans] );
sceneSetVars( {'userdata', [410, 0, 0]} );
sceneRender('KeyReleaseFcn',@keyCallback);
%sceneRender();

function keyCallback(src, evnt)
fig = gcbf;
% patches = findobj(fig, 'Type', 'patch');
% names = get(patches, 'UserData');
% ind = find( strcmp( 'box', names ) );
g = sceneGetGeom('user');
pos = g.T_world_geom(1:3,4);

if ~isempty(evnt.Modifier)
    ind = find( strcmp(evnt.Modifier, 'shift') );
    
    if ~isempty(ind)
        switch (evnt.Key)
            case 'w'
                camorbit(0,-5);
            case 's'
                camorbit(0,5);
            case 'a'
                camorbit(5,0);
            case 'd'
                camorbit(-5,0);
        end
    end
else
    switch (evnt.Key)
        case 'w'
            pos(2) = pos(2) - 10;
        case 's'
            pos(2) = pos(2) + 10;
        case 'a'
            pos(1) = pos(1) + 10;
        case 'd'
            pos(1) = pos(1) - 10;
        case 'q'
            pos(3) = pos(3) - 10;
        case 'e'
            pos(3) = pos(3) + 10;
    end    
end

sceneSetVars({'usertrans', pos'});
sceneRender();