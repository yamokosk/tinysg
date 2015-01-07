# Locate Xerces home

# This module defines
# XERCES_HOME where to find include, lib, bin, etc.
# XERCES_FOUND, If set to 0, don't try to use Xerces.

# start with 'not found'
SET( XERCES_FOUND 0 CACHE BOOL "Do we have Xerces?" )

FIND_PATH( XERCES_HOME_INCLUDE xercesc/dom/DOM.hpp
  $ENV{XERCES_HOME}/include
  # Standard linux installation points
  # Standard windows installation points
  C:/xerces-c_2_8_0/include
)

# NOTE: if ORCA2_HOME_INCLUDE_ORCA is set to *-NOTFOUND it will evaluate to FALSE
IF ( XERCES_HOME_INCLUDE )
	SET( XERCES_FOUND 1 CACHE BOOL "Do we have Xerces?" FORCE )

	# strip to get rid of 'include'
	GET_FILENAME_COMPONENT( XERCES_HOME ${XERCES_HOME_INCLUDE} PATH )
ENDIF ( XERCES_HOME_INCLUDE )

#MESSAGE(STATUS	"Setting Xerces installed directory to ${XERCES_HOME}" )
