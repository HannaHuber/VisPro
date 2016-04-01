cmake_minimum_required(VERSION 2.8.5 FATAL_ERROR)
if(POLICY CMP0025)
  cmake_policy(SET CMP0025 NEW) # CMake 3.0
endif()
if(POLICY CMP0053)
  cmake_policy(SET CMP0053 NEW) # CMake 3.1
endif()

PROJECT (Visualisierung_2)

find_package(VTK COMPONENTS
  vtkFiltersSources
  vtkInteractionWidgets
  vtkRendering${VTK_RENDERING_BACKEND}
)
include(${VTK_USE_FILE})

add_executable(Visualisierung_2 MACOSX_BUNDLE Visualisierung_2.cxx)
target_link_libraries(Visualisierung_2 ${VTK_LIBRARIES})
