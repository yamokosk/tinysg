# tinysg
Tiny Scene Graph (TinySG) is a lightweight and extensible scene graph manager. It is the successor of my [SceneML](https://code.google.com/p/sceneml/) project.

# Features
* Lightweight scene graph.
* Scenes can be saved/loaded from fairly simple XML definitions.
* Fairly extensible scene and object queries supported through a plugin-type framework.
* Current plugins support collision and distance queries based on [ODE](http://www.ode.org) and the [Lin-Canny](http://www.cs.berkeley.edu/~jfc/mirtich/collDet.html) algorithm respectively.

# To-do Features
* Finish porting SceneML's Matlab interface to TinySG
* Finish multithreading scene node update algorithm to take advantage of today's multi-core computers

# Example of a XML scene description

TinySG supports virtually every primitive geometry supported by ODE (e.g. boxes, spheres, capsules, etc) as well as arbitrary user-defined meshes specified in either OBJ or STL file formats.

    <?xml version="1.0" ?>
    <TinySG version="0">
        <node name="_WORLD_">
            <position value="0 0 0" />
            <orientation format="AngleAxis" angle="0" axis="1 0 0" />
            <object name="lspace" type="ODESimpleSpace" />
            <object name="rspace" type="ODESimpleSpace" />
        </node>
        <node name="n1" parent="_WORLD_">
            <position value="1 1 0" />
            <orientation format="AngleAxis" angle="0" axis="1 0 0" />
        </node>
        <node name="n3" parent="n1">
            <position value="2 0 0" />
            <orientation format="AngleAxis" angle="0" axis="1 0 0" />
            <object name="asphere" type="ODESphere">
                <property name="radius" class="real" value="2" />
                <property name="space" class="scene_object_ptr" value="rspace" />
            </object>
        </node>
        <node name="n4" parent="n1">
            <position value="0 0 0" />
            <orientation format="AngleAxis" angle="0" axis="1 0 0" />
        </node>
        <node name="n2" parent="_WORLD_">
            <position value="2 1 0" />
            <orientation format="AngleAxis" angle="0" axis="1 0 0" />
            <object name="abox" type="ODEBox">
                <property name="length" class="real" value="1" />
                <property name="width" class="real" value="2" />
                <property name="height" class="real" value="3" />
                <property name="space" class="scene_object_ptr" value="lspace" />
            </object>
        </node>
    </TinySG>