# -----------------------------------------------------------------------------
#	Define some varaibles used later
# -----------------------------------------------------------------------------
set(MEX_TARGET_NAME mex_tinysg )

set( INPUT_FILE ${PROJECT_SOURCE_DIR}/addons/matlab/mex/compile_mex_interface.m.in )
set( OUTPUT_FILE ${PROJECT_SOURCE_DIR}/addons/matlab/mex/compile_mex_interface.m )

configure_file( ${INPUT_FILE} ${OUTPUT_FILE} )
 
message( STATUS "Wrote compile_mex_interface.m" )
message( STATUS "** NOTE **")
message( STATUS "  To finish installation of the Matlab addon, you will need to run the" )
message( STATUS "  compile_mex_interface.m script located in the mex subdirectory." )
message( STATUS "** NOTE **")