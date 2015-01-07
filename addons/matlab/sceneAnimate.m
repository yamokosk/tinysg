function M = sceneAnimate(varNames, varargin)
% TODO: Completely re-write this function!

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

[numVars,junk] = size(varNames);

if nargin > 1
    A = varargin{1};
    [numFrames, junk] = size(A);
    
    if (numVars ~= nargin - 1)
        error(['Number of variables (' num2str(numVars) ') does not match the number of data inputs (' num2str(nargin-1) ').']);
    end
    
    for j = 1:numFrames
        vars = {};
        for n = 1:numVars
            A = varargin{n};
            [r,c] = size(A);
            
            vars(n,1) = mat2cell(A(j,:), 1, c);
        end
        sceneSetVars([varNames, vars]);
        sceneRender();
        M(j) = getframe;
    end
else
    M = [];
end
