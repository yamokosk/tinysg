function g = sceneSetParent(body,newParent)
% sceneSetParent - Sets the parent body of a body
%
%   sceneSetParent(body, newParent) sets the parent body to newParent. 
%
%   Example:
%       % Sets the 'world' as the new parent for 'ball'
%       sceneSetParent('ball', 'world');

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

if ( ~ischar(body) )
    error(['First argument must be the body name']);
end

if ( ~ischar(newParent) )
    error(['Second argument must be the new parent body name']);
end

b_old = sceneGetBody(body);
sceneml('SetProxBody', body, newParent);
b_new = sceneGetBody(body);
disp(['Old parent: ' b_old.proxBody]);
disp(['New parent: ' b_new.proxBody]);

fh = get(0,'Children');     % Get handles to all figure windows
% Now loop through and check to see if a SceneML figure window is open. If
% so, its a good idea to close it since problems can occur if we load a
% different scene description file.
oldFigNumber = -1;
for n = 1:length(fh)
    fig = fh(n);
    figUserData = get(fig, 'UserData');
    if ( ischar(figUserData) )
        if ( strcmp('xode', figUserData) )
            warning('Found a SceneML render window. Closing the old one but will re-render once the new scene is loaded.');
            oldFigNumber = fig;
            close(fig);
        end
    end
end

% If the user had a render window open before, lets be nice and re-render
% this one.
if (oldFigNumber > 0)
    figure(oldFigNumber);
    sceneRender;
end