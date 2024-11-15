file(
 GLOB_RECURSE PLANET_INC CONFIGURE_DEPENDS
 RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
 *.h *.hpp
)

file(
 GLOB_RECURSE PLANET_SRC CONFIGURE_DEPENDS
 RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
 *.c *.cpp
)
#Copies this planet's asset folder to bin when it is built
add_custom_target(copyAssetsA5 ALL COMMAND ${CMAKE_COMMAND} -E copy_directory
${CMAKE_CURRENT_SOURCE_DIR}/assets/
${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/)

install(FILES ${PLANET_INC} DESTINATION include/planet)
add_executable(planet ${PLANET_SRC} ${PLANET_INC})
target_link_libraries(planet PUBLIC core IMGUI glm)
target_include_directories(planet PUBLIC ${CORE_INC_DIR} ${stb_INCLUDE_DIR})

#Trigger asset copy when planet is built
add_dependencies(planet copyAssetsA5)