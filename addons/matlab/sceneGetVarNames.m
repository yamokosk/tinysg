function names = sceneGetVarNames()
% sceneGetVarNames
%
% SYNTAX
% 
%   names = sceneGetVarNames()
% 
% DESCRIPTION
% 
%   Returns the named variables defined in the SceneML file.

names = sceneml('GetVarNames');