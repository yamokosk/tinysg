set( build_test TRUE )

# Required source files for this test
set( test_srcs	${PROJECT_SOURCE_DIR}/addons/mocap/EvartSdk2Interface.cpp
				${PROJECT_SOURCE_DIR}/addons/mocap/EvartGeneratorManager.cpp
				${PROJECT_SOURCE_DIR}/addons/mocap/MocapPoseGenerator.cpp
				${PROJECT_SOURCE_DIR}/addons/mocap/Marker.cpp
)

link_directories( ${PROJECT_SOURCE_DIR}/math )
link_libraries( TinySgMath EVaRT2 )