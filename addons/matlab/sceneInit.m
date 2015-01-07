function sceneInit(filename)
% sceneLoad - Instructs the TinySG library to load a scene from file
%
%   sceneLoad(filename) simply instructs the SceneML library to load the
%   scene XML definition stored in filename. 
%
%   This function only has to be called anytime the XML description
%   changes. Otherwise this function only needs to be called once.
%
%   Example:
%       sceneLoad('demo/twopa10.scene');

% TinySG, Copyright (C) 2007, 2008  J.D. Yamokoski
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

% Found a better to determine if a sceneml window is already open
figHandle = findobj('UserData', 'tinysg');

if ( ~isempty(figHandle) )
    warning('Found a TinySG render window. Closing the old one but will re-render once the new scene is loaded.');
    close(figHandle);
end

% Now load the new scene description
[pathstr,name,ext] = fileparts(filename);
% If the scene file is locate in another directory, cd to it, load the
% file, and the cd back to the original directory.
%if ( ~isempty(pathstr) ) 
    %currPath = pwd; % Record the current path
    %cd(pathstr);
    libmex_tinysg('LoadScene', [name, ext]);
    %cd(currPath);
%else
%    sceneml('LoadScene', [name, ext]);
%end

% If the user had a render window open before, lets be nice and re-render
% this one.
if ( ~isempty(figHandle) )
    figure(figHandle);
    sceneRender;
end