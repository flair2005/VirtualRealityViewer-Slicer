cmake_minimum_required(VERSION 2.8.9)

#-----------------------------------------------------------------------------
project(VirtualRealityViewer)

#-----------------------------------------------------------------------------
set(EXTENSION_HOMEPAGE "http://www.slicer.org/slicerWiki/index.php/Documentation/Nightly/Modules/DicomExplorer")
set(EXTENSION_CATEGORY "Visualization")
set(EXTENSION_CONTRIBUTORS "Franklin King (PerkLab, Queen's University), Saskia Camps (Brigham)")
set(EXTENSION_DESCRIPTION "Virtual Reality")
set(EXTENSION_ICONURL "http://www.slicer.org/slicerWiki/images/7/71/DicomExplorerLogo.png")
set(EXTENSION_SCREENSHOTURLS "http://www.slicer.org/slicerWiki/images/c/ca/DicomExplorerScreenshot.png")
set(EXTENSION_STATUS "Experimental")
set(EXTENSION_DEPENDS "NA") # Specified as a space separated list or 'NA' if any

#-----------------------------------------------------------------------------
find_package(Slicer REQUIRED)
include(${Slicer_USE_FILE})

#-----------------------------------------------------------------------------
add_subdirectory(VirtualRealityViewer)

#-----------------------------------------------------------------------------
INCLUDE(${Slicer_EXTENSION_CPACK})
