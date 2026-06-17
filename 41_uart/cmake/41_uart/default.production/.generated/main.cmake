include("${CMAKE_CURRENT_LIST_DIR}/rule.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/file.cmake")

set(41_uart_default_library_list )

# Handle files with suffix s, for group default-XC16
if(41_uart_default_default_XC16_FILE_TYPE_assemble)
add_library(A_41_uart_default_default_XC16_assemble OBJECT ${41_uart_default_default_XC16_FILE_TYPE_assemble})
    A_41_uart_default_default_XC16_assemble_rule(A_41_uart_default_default_XC16_assemble)
    list(APPEND 41_uart_default_library_list "$<TARGET_OBJECTS:A_41_uart_default_default_XC16_assemble>")

endif()

# Handle files with suffix S, for group default-XC16
if(41_uart_default_default_XC16_FILE_TYPE_assemblePreproc)
add_library(A_41_uart_default_default_XC16_assemblePreproc OBJECT ${41_uart_default_default_XC16_FILE_TYPE_assemblePreproc})
    A_41_uart_default_default_XC16_assemblePreproc_rule(A_41_uart_default_default_XC16_assemblePreproc)
    list(APPEND 41_uart_default_library_list "$<TARGET_OBJECTS:A_41_uart_default_default_XC16_assemblePreproc>")

endif()

# Handle files with suffix c, for group default-XC16
if(41_uart_default_default_XC16_FILE_TYPE_compile)
add_library(A_41_uart_default_default_XC16_compile OBJECT ${41_uart_default_default_XC16_FILE_TYPE_compile})
    A_41_uart_default_default_XC16_compile_rule(A_41_uart_default_default_XC16_compile)
    list(APPEND 41_uart_default_library_list "$<TARGET_OBJECTS:A_41_uart_default_default_XC16_compile>")

endif()

# Handle files with suffix s, for group default-XC16
if(41_uart_default_default_XC16_FILE_TYPE_dependentObject)
add_library(A_41_uart_default_default_XC16_dependentObject OBJECT ${41_uart_default_default_XC16_FILE_TYPE_dependentObject})
    A_41_uart_default_default_XC16_dependentObject_rule(A_41_uart_default_default_XC16_dependentObject)
    list(APPEND 41_uart_default_library_list "$<TARGET_OBJECTS:A_41_uart_default_default_XC16_dependentObject>")

endif()

# Handle files with suffix elf, for group default-XC16
if(41_uart_default_default_XC16_FILE_TYPE_bin2hex)
add_library(A_41_uart_default_default_XC16_bin2hex OBJECT ${41_uart_default_default_XC16_FILE_TYPE_bin2hex})
    A_41_uart_default_default_XC16_bin2hex_rule(A_41_uart_default_default_XC16_bin2hex)
    list(APPEND 41_uart_default_library_list "$<TARGET_OBJECTS:A_41_uart_default_default_XC16_bin2hex>")

endif()


# Main target for this project
add_executable(41_uart_default_image_D85CN2hM ${41_uart_default_library_list})

set_target_properties(41_uart_default_image_D85CN2hM PROPERTIES
    OUTPUT_NAME "default-production"
    SUFFIX ".elf"
    RUNTIME_OUTPUT_DIRECTORY "${41_uart_default_output_dir}")
target_link_libraries(41_uart_default_image_D85CN2hM PRIVATE ${41_uart_default_default_XC16_FILE_TYPE_link})

# Add the link options from the rule file.
A_41_uart_default_link_rule( 41_uart_default_image_D85CN2hM)

# Call bin2hex function from the rule file
A_41_uart_default_bin2hex_rule(41_uart_default_image_D85CN2hM)

