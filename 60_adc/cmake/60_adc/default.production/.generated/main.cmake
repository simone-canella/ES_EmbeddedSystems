include("${CMAKE_CURRENT_LIST_DIR}/rule.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/file.cmake")

set(60_adc_default_library_list )

# Handle files with suffix s, for group default-XC16
if(60_adc_default_default_XC16_FILE_TYPE_assemble)
add_library(A_60_adc_default_default_XC16_assemble OBJECT ${60_adc_default_default_XC16_FILE_TYPE_assemble})
    A_60_adc_default_default_XC16_assemble_rule(A_60_adc_default_default_XC16_assemble)
    list(APPEND 60_adc_default_library_list "$<TARGET_OBJECTS:A_60_adc_default_default_XC16_assemble>")

endif()

# Handle files with suffix S, for group default-XC16
if(60_adc_default_default_XC16_FILE_TYPE_assemblePreproc)
add_library(A_60_adc_default_default_XC16_assemblePreproc OBJECT ${60_adc_default_default_XC16_FILE_TYPE_assemblePreproc})
    A_60_adc_default_default_XC16_assemblePreproc_rule(A_60_adc_default_default_XC16_assemblePreproc)
    list(APPEND 60_adc_default_library_list "$<TARGET_OBJECTS:A_60_adc_default_default_XC16_assemblePreproc>")

endif()

# Handle files with suffix c, for group default-XC16
if(60_adc_default_default_XC16_FILE_TYPE_compile)
add_library(A_60_adc_default_default_XC16_compile OBJECT ${60_adc_default_default_XC16_FILE_TYPE_compile})
    A_60_adc_default_default_XC16_compile_rule(A_60_adc_default_default_XC16_compile)
    list(APPEND 60_adc_default_library_list "$<TARGET_OBJECTS:A_60_adc_default_default_XC16_compile>")

endif()

# Handle files with suffix s, for group default-XC16
if(60_adc_default_default_XC16_FILE_TYPE_dependentObject)
add_library(A_60_adc_default_default_XC16_dependentObject OBJECT ${60_adc_default_default_XC16_FILE_TYPE_dependentObject})
    A_60_adc_default_default_XC16_dependentObject_rule(A_60_adc_default_default_XC16_dependentObject)
    list(APPEND 60_adc_default_library_list "$<TARGET_OBJECTS:A_60_adc_default_default_XC16_dependentObject>")

endif()

# Handle files with suffix elf, for group default-XC16
if(60_adc_default_default_XC16_FILE_TYPE_bin2hex)
add_library(A_60_adc_default_default_XC16_bin2hex OBJECT ${60_adc_default_default_XC16_FILE_TYPE_bin2hex})
    A_60_adc_default_default_XC16_bin2hex_rule(A_60_adc_default_default_XC16_bin2hex)
    list(APPEND 60_adc_default_library_list "$<TARGET_OBJECTS:A_60_adc_default_default_XC16_bin2hex>")

endif()


# Main target for this project
add_executable(60_adc_default_image__KrBTrtX ${60_adc_default_library_list})

set_target_properties(60_adc_default_image__KrBTrtX PROPERTIES
    OUTPUT_NAME "default-production"
    SUFFIX ".elf"
    RUNTIME_OUTPUT_DIRECTORY "${60_adc_default_output_dir}")
target_link_libraries(60_adc_default_image__KrBTrtX PRIVATE ${60_adc_default_default_XC16_FILE_TYPE_link})

# Add the link options from the rule file.
A_60_adc_default_link_rule( 60_adc_default_image__KrBTrtX)

# Call bin2hex function from the rule file
A_60_adc_default_bin2hex_rule(60_adc_default_image__KrBTrtX)

