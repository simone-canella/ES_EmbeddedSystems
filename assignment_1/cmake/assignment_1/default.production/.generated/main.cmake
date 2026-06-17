include("${CMAKE_CURRENT_LIST_DIR}/rule.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/file.cmake")

set(assignment_1_default_library_list )

# Handle files with suffix s, for group default-XC16
if(assignment_1_default_default_XC16_FILE_TYPE_assemble)
add_library(assignment_1_default_default_XC16_assemble OBJECT ${assignment_1_default_default_XC16_FILE_TYPE_assemble})
    assignment_1_default_default_XC16_assemble_rule(assignment_1_default_default_XC16_assemble)
    list(APPEND assignment_1_default_library_list "$<TARGET_OBJECTS:assignment_1_default_default_XC16_assemble>")

endif()

# Handle files with suffix S, for group default-XC16
if(assignment_1_default_default_XC16_FILE_TYPE_assemblePreproc)
add_library(assignment_1_default_default_XC16_assemblePreproc OBJECT ${assignment_1_default_default_XC16_FILE_TYPE_assemblePreproc})
    assignment_1_default_default_XC16_assemblePreproc_rule(assignment_1_default_default_XC16_assemblePreproc)
    list(APPEND assignment_1_default_library_list "$<TARGET_OBJECTS:assignment_1_default_default_XC16_assemblePreproc>")

endif()

# Handle files with suffix c, for group default-XC16
if(assignment_1_default_default_XC16_FILE_TYPE_compile)
add_library(assignment_1_default_default_XC16_compile OBJECT ${assignment_1_default_default_XC16_FILE_TYPE_compile})
    assignment_1_default_default_XC16_compile_rule(assignment_1_default_default_XC16_compile)
    list(APPEND assignment_1_default_library_list "$<TARGET_OBJECTS:assignment_1_default_default_XC16_compile>")

endif()

# Handle files with suffix s, for group default-XC16
if(assignment_1_default_default_XC16_FILE_TYPE_dependentObject)
add_library(assignment_1_default_default_XC16_dependentObject OBJECT ${assignment_1_default_default_XC16_FILE_TYPE_dependentObject})
    assignment_1_default_default_XC16_dependentObject_rule(assignment_1_default_default_XC16_dependentObject)
    list(APPEND assignment_1_default_library_list "$<TARGET_OBJECTS:assignment_1_default_default_XC16_dependentObject>")

endif()

# Handle files with suffix elf, for group default-XC16
if(assignment_1_default_default_XC16_FILE_TYPE_bin2hex)
add_library(assignment_1_default_default_XC16_bin2hex OBJECT ${assignment_1_default_default_XC16_FILE_TYPE_bin2hex})
    assignment_1_default_default_XC16_bin2hex_rule(assignment_1_default_default_XC16_bin2hex)
    list(APPEND assignment_1_default_library_list "$<TARGET_OBJECTS:assignment_1_default_default_XC16_bin2hex>")

endif()


# Main target for this project
<<<<<<< HEAD:assignments/assignment_1/cmake/assignment_1/default.production/.generated/main.cmake
add_executable(assignment_1_default_image_izOYjmLg ${assignment_1_default_library_list})

set_target_properties(assignment_1_default_image_izOYjmLg PROPERTIES
    OUTPUT_NAME "default-production"
    SUFFIX ".elf"
    RUNTIME_OUTPUT_DIRECTORY "${assignment_1_default_output_dir}")
target_link_libraries(assignment_1_default_image_izOYjmLg PRIVATE ${assignment_1_default_default_XC16_FILE_TYPE_link})

# Add the link options from the rule file.
assignment_1_default_link_rule( assignment_1_default_image_izOYjmLg)

# Call bin2hex function from the rule file
assignment_1_default_bin2hex_rule(assignment_1_default_image_izOYjmLg)
=======
add_executable(assignment_1_default_image_5NNy5Qb4 ${assignment_1_default_library_list})

set_target_properties(assignment_1_default_image_5NNy5Qb4 PROPERTIES
    OUTPUT_NAME "default-production"
    SUFFIX ".elf"
    RUNTIME_OUTPUT_DIRECTORY "${assignment_1_default_output_dir}")
target_link_libraries(assignment_1_default_image_5NNy5Qb4 PRIVATE ${assignment_1_default_default_XC16_FILE_TYPE_link})

# Add the link options from the rule file.
assignment_1_default_link_rule( assignment_1_default_image_5NNy5Qb4)

# Call bin2hex function from the rule file
assignment_1_default_bin2hex_rule(assignment_1_default_image_5NNy5Qb4)
>>>>>>> c176c6882c7d4871c8768f8dacbe6e5a0ff4a087:assignments/exam/17_assignment1/cmake/assignment_1/default.production/.generated/main.cmake

