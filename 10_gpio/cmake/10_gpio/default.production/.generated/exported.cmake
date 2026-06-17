set(DEPENDENT_MP_BIN2HEX10_gpio_default_jcUUD4LY "c:/Program Files/Microchip/xc16/v2.10/bin/xc16-bin2hex.exe")
set(DEPENDENT_DEPENDENT_TARGET_ELF10_gpio_default_jcUUD4LY ${CMAKE_CURRENT_LIST_DIR}/../../../../out/10_gpio/production/default-production.elf)
set(DEPENDENT_TARGET_DIR10_gpio_default_jcUUD4LY ${CMAKE_CURRENT_LIST_DIR}/../../../../out/10_gpio/production)
set(DEPENDENT_BYPRODUCTS10_gpio_default_jcUUD4LY ${DEPENDENT_TARGET_DIR10_gpio_default_jcUUD4LY}/${sourceFileName10_gpio_default_jcUUD4LY}.s)
add_custom_command(
    OUTPUT ${DEPENDENT_TARGET_DIR10_gpio_default_jcUUD4LY}/${sourceFileName10_gpio_default_jcUUD4LY}.s
    COMMAND ${DEPENDENT_MP_BIN2HEX10_gpio_default_jcUUD4LY} ${DEPENDENT_DEPENDENT_TARGET_ELF10_gpio_default_jcUUD4LY} --image ${sourceFileName10_gpio_default_jcUUD4LY} ${address10_gpio_default_jcUUD4LY} ${mode10_gpio_default_jcUUD4LY} -mdfp=C:/Users/simon/.mchp_packs/Microchip/dsPIC33E-GM-GP-MC-GU-MU_DFP/1.6.297/xc16 
    WORKING_DIRECTORY ${DEPENDENT_TARGET_DIR10_gpio_default_jcUUD4LY}
    DEPENDS ${DEPENDENT_DEPENDENT_TARGET_ELF10_gpio_default_jcUUD4LY})
add_custom_target(
    dependent_produced_source_artifact10_gpio_default_jcUUD4LY 
    DEPENDS ${DEPENDENT_TARGET_DIR10_gpio_default_jcUUD4LY}/${sourceFileName10_gpio_default_jcUUD4LY}.s
    )
