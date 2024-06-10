file(GLOB_RECURSE SRC_FILES *.cpp CMAKE_CONFIGURE_DEPENDS)
file(GLOB_RECURSE HEADER_FILES *.h CMAKE_CONFIGURE_DEPENDS)
add_executable(gl1 ${HEADER_FILES} ${SRC_FILES})

target_include_directories(gl1 PUBLIC .)
target_include_directories(gl1 PUBLIC ${SDL2_INCLUDE_DIRS} ${Vulkan_INCLUDE_DIRS} )

include_directories(${CMAKE_SOURCE_DIR}/glad/include)
target_link_libraries(gl1 glfw glad)

# set(copy_source_dir "${CMAKE_SOURCE_DIR}/data/")
# set(copy_dest_dir "${CMAKE_BINARY_DIR}/Build/bin")

# add_custom_command(
#         TARGET ${PROJECT_NAME} POST_BUILD
#         COMMAND ${CMAKE_COMMAND} -E copy_directory
#                 ${CMAKE_CURRENT_SOURCE_DIR}/assets/shaders
#                 ${CMAKE_BINARY_DIR}/bin/shaders
#         COMMAND ${CMAKE_COMMAND} -E copy_directory
#                 ${CMAKE_CURRENT_SOURCE_DIR}/assets/textures
#                 ${CMAKE_BINARY_DIR}/bin/textures
#         COMMAND ${CMAKE_COMMAND} -E copy_directory
#                 ${CMAKE_CURRENT_SOURCE_DIR}/assets/gltf
#                 ${CMAKE_BINARY_DIR}/bin/gltf
#         COMMENT "Copying to ${CMAKE_BINARY_DIR}"
# )