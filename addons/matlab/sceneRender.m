function fig = sceneRender(varargin)
% sceneRender - Renders a scene loaded by SceneML
%
%   fig = sceneRender() renders the scene currently stored in the SceneML
%   library and then returns the figure handle associated with the Matlab
%   window.
%
%   fig = sceneRender('paramName1', paramValue1, ...) passes additional
%   parameters to the Matlab figure window when its created for the first
%   time. Consult Matlab's documentation for more information on the
%   various figure properties.
%
%   Subsequent calls to sceneRender will reuse the same figure window.
%
%   Example:
%       sceneInit('demo/twopa10.scene');
%       h = sceneRender();
%
%   See also sceneInit

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

% Get geom data from the scene and compute face/vertex data if necessary
% TODO: Replace createGeomFV in Matlab and do this inside the mex
% file!
allGeomData = sceneGetAllGeoms();
%allGeomData = createGeomFV(allGeomData);
ngeoms = length(allGeomData);
    
% Found a better to determine if a sceneml window is already open
figHandle = findobj('UserData', 'tinysg');

if ( isempty(figHandle) )
    % No existing render window. Create a new one.
    fig = createWindow(varargin);
    %drawCoordinateSystem(fig, eye(4), 'world', 10);
    
    % And then create all the new 3D objects 
    %   NOTE: Pathces are created in reverse order.. later when we call
    %       patches = findobj(gca, 'Type', 'patch');
    %   the patches will be ordered correctly.
    for n=ngeoms:-1:1
        nmeshes = length(allGeomData(n).meshes);
        
        for m = nmeshes:-1:1
            patch('Faces', allGeomData(n).meshes(m).faces, ...
                  'Vertices', allGeomData(n).meshes(m).vertices, ...
                  'FaceColor', allGeomData(n).parameters.filenames(m).color'/255, ...
                  'FaceLighting', 'flat', ...
                  'EdgeColor', 'none', ...
                  'FaceAlpha', allGeomData(n).parameters.filenames(m).alpha, ...
                  'UserData', [allGeomData(n).name num2str(m)]);
        end
    end
else
    % Use the existing render window.
    figure(figHandle);
    
    % Get handles to all the 3D objects and then update their data
    patches = findobj(gca, 'Type', 'patch');
    
    % Get ready to set a bunch of properties all at once
    %propNames = {'Vertices'; 'FaceAlpha'};
%     propNames = {'Vertices'};
%     fv = [allGeomData.fv];
%     %propValues = {fv.vertices; allGeomData.alpha}';
%     propValues = {fv.vertices};
%     set(patches, propNames, propValues);
% 
    propValues = {};
    for n=1:ngeoms
        vertData = {allGeomData(n).meshes.vertices}';
        propValues = [propValues; vertData];
    end
    propNames = {'Vertices'};
    set(patches, propNames, propValues);
end

% NO LONGER RESPONSIBLE FOR COLORING OBJECTS BASED ON COLLISION STATUS...
% THIS IS UP TO THE USER
% % Now check if any geoms are in collision.. if so color them differently
% if ( sceneCollisionState() )
% 	c = sceneGetContactData();
% 	
% 	for n = 1:length(c)
%         h = sceneGetGeomHandle( c(n).geom1 );
%         set(h, 'FaceColor', [1, 0, 0]); % Set geom1 face color to red
%         h = sceneGetGeomHandle( c(n).geom2 );
%         set(h, 'FaceColor', [1, 0, 0]); % Set geom2 face color to red
%     end
% end

% drawnow;

end % End drawScene()


function geoms = createGeomFV(geoms)
% dSphereClass          0   Sphere
% dBoxClass             1   Box
% dCapsuleClass         2 	Capsule (i.e. cylinder with half-sphere caps at its ends)
% dCylinderClass        3	Regular flag ended Cylinder
% dPlaneClass           4   Infinite plane (non-placeable)
% dRayClass             5   Ray
% dConvexClass          6	
% dGeomTransformClass   7	Geometry transform
% dTrimeshClass         8	Triangle mesh

ngeoms = length(geoms);

for n = 1:ngeoms
    switch ( geoms(n).type )
        case 0  % sphere
            geoms(n).fv = createSphere(geoms(n).T_world_geom, geoms(n).params.radius);
        case 1  % box
            geoms(n).fv = createBox(geoms(n).T_world_geom, [geoms(n).params.length, geoms(n).params.width, geoms(n).params.height]);
        case 2  % capsule
            geoms(n).fv = createCapsule(geoms(n).T_world_geom, geoms(n).params.length, geoms(n).params.radius);
        case 3  % cylinder
            geoms(n).fv = createCylinder(geoms(n).T_world_geom, geoms(n).params.length, geoms(n).params.radius);
        case 4  % plane
            geoms(n).fv = createPlane([-2000, 2000, -4000, 4000], [geoms(n).params.normal_x,geoms(n).params.normal_y,geoms(n).params.normal_z,geoms(n).params.d]);
    end
end

end % End of drawGeom()


function fig = createWindow(varargin)

fig = figure('Position', [50, 50, 1024, 768], ...
             'Renderer', 'zbuffer', ...
             'UserData', 'tinysg');
nopts = size(varargin{1},2);
for n = 1:2:nopts
    set(fig, varargin{1}(n), varargin{1}(n+1));
end

% --- Problem! ---
% sceneGetAABB seems to be broken... can't use it to set up view volume.
% --- Problem! ---

% % Scene axes
% if (isunix())
    set(gca, 'DataAspectRatio', [1,1,1], ...
             'Visible', 'off');
%     set(gca, 'Visible', 'off', ...
%              'Projection', 'perspective');
	view(130,30);
    light('Position',[0 0 400],'Style','infinite');
% else
%     aabb = sceneGetAABB();
% 
%     cpos = [aabb(2), aabb(4), aabb(6)];
%     centroid = [aabb(1), aabb(3), aabb(5)];
%     fov = pi/3;
% 
%     set(gca, 'DataAspectRatio',   [1, 1, 1], ...
%              'CameraPosition',    cpos, ...
%              'CameraTarget',      centroid, ...
%              'CameraViewAngle',   fov, ...
%              'CameraUpVector',    [0,0,1], ...
%              'Visible',           'off');
%          
%     light('Position',[0 0 aabb(6)*20],'Style','infinite');
% end



end % End createWindow()