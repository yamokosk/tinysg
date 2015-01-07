# Collision query plugin
if ( Ode_FOUND )
	add_subdirectory( ode )
endif ( Ode_FOUND )

# Distance query plugin
add_subdirectory( lincanny )
