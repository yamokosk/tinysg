set( build_test FALSE )

# Required source files for this test
set( test_srcs ${PROJECT_SOURCE_DIR}/src/Exception.cpp
			   ${PROJECT_SOURCE_DIR}/src/Object.cpp
			   ${PROJECT_SOURCE_DIR}/src/SceneNode.cpp
			   ${PROJECT_SOURCE_DIR}/src/PropertyCollection.cpp
			   ${PROJECT_SOURCE_DIR}/src/MovableObject.cpp
			   ${PROJECT_SOURCE_DIR}/src/Vector3.cpp
			   ${PROJECT_SOURCE_DIR}/src/Math.cpp
			   ${PROJECT_SOURCE_DIR}/src/Matrix3.cpp
			   ${PROJECT_SOURCE_DIR}/src/Matrix4.cpp
			   ${PROJECT_SOURCE_DIR}/src/Quaternion.cpp
			   ${PROJECT_SOURCE_DIR}/src/MathExpression.cpp )