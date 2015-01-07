%module tinysg
%include "std_string.i"
%include Property.i
%include ObjectModel.i
%include SceneNode.i
%include SceneGraph.i

%apply const std::string& {std::string* foo};
