% startup - Sets up the Matlab environment for SceneML

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

echo off

BASE = pwd;

MEX = fullfile(cd,'mex');
disp(['Found the mex path as  ' MEX]);

DEMO=fullfile(cd,'demo');
disp(['Found the demo path as  ' DEMO]);

UTIL=fullfile(cd,'util');
disp(['Found the util path as  ' UTIL]);

% add require paths
addpath(BASE,MEX,DEMO,UTIL);

% Create mex files
% cd(MEX);
% disp(['Compiling mex file for this platform']);
% compile;
% cd(BASE);

% All done, clean up
if (savepath)
    disp(['There was some sort of error when I tried to save the new path information.']);
    disp(['Please investigate this otherwise this script will need to be run anytime Matlab is restarted before every use of SceneML.']);
end
disp(['Installation complete.']);
clear BASE MEX DEMO