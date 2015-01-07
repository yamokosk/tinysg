function sceneSetVars(vars, values)
% sceneSetVars - Method to change values declared as mutable in SceneML
%
%   sceneSetVars(varstruct) updates the named mutable variables in the 
%   library with values stored in the structure varstruct. The field names
%   of varstruct must coincide with the mutable variable names.
%
%   sceneSetVars(names, values) also updates the named mutable variables.
%   names is a single column cell array of the variable names and values is
%   a single column cell array the associated values. If all variables are
%   the same size (e.g. all 3x1 vectors), then values can be a matrix (or
%   vector) of values.
%
%   Example: Suppose we had a scene with the following variables, 'apple'
%   (scalar), 'peach' (1x3 vector), and 'pear' (scalar).
%
%   Syntax using varstruct:
%       vs.apple = 20;
%       vs.peach = [1, 2, 3];
%       vs.pear = 2;
%       sceneSetVars(vs);
%
%       ns.apple = 30;      % Not all variables need to be defined
%       sceneSetVars(ns);
%
%   Syntax using cell arrays:
%       names = {'apple'; 'peach'; 'pear'}
%       vals = {20; [1, 2, 3]; 2};
%       sceneSetVars(names, vals);
%
%       % Again not all variables need to be defined
%       sceneSetVars('apple', 20);
%
%       % If all values are the same size, then a standard matrix (non-cell
%       % array suffices
%       sceneSetVars({'apple'; 'pear'}, rand(2,1));

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

% Internally, the Matlab interface to the SceneML library only accepts a
% Nx2 cell array of variable/value pairs. To support the different calling
% methods of sceneSetVars we need to turn the inputs into this required
% format.
libnames = {};
libvalues = {};

% If only one argument, it should be the structure method
if (nargin == 1)
    if ( ~isstruct(vars) )
        error('Expecting a structure for the first argument!');
    else
        % Convert vars struct into the required cell array
        libnames = fieldnames(vars);
        nvars = size(libnames,1);
        libvalues = cell(nvars,1);
        for n = 1:length(libnames)
            libvalues{n,1} = vars.(libnames{n});
        end
    end
elseif (nargin == 2) % Two arguments, its the cell array approach
    if ( ~iscell(vars) )
        error('Expecting a cell array for the first argument!');
    end
    libnames = vars;
    
    nvar = size(vars, 1); nval = 0;
    if ( iscell(values) )
        libvalues = values;
        nval = size(libvalues,1);
    else
        [nval, dim] = size(values);
        libvalues = mat2cell(values, ones(nval,1), dim);
    end

    if (nvar ~= nval)
        error(['You gave me ' num2str(nvar) ' variables and ' num2str(nval) ' values. What am I suppose to do with that??']);
    end
else
    error('Incorrect number of arguments.');
end

libinput = [libnames, libvalues];
% Update the library
sceneml('Update', libinput);