set(DEPENDENT_MP_BIN2HEX10_gpio_default_R7iygVKP "c:/Program Files/Microchip/xc16/v2.10/bin/xc16-bin2hex.exe")
set(DEPENDENT_DEPENDENT_TARGET_ELF10_gpio_default_R7iygVKP ${CMAKE_CURRENT_LIST_DIR}/../../../../out/10_gpio/default.elf)
set(DEPENDENT_TARGET_DIR10_gpio_default_R7iygVKP ${CMAKE_CURRENT_LIST_DIR}/../../../../out/10_gpio)
set(DEPENDENT_BYPRODUCTS10_gpio_default_R7iygVKP ${DEPENDENT_TARGET_DIR10_gpio_default_R7iygVKP}/${sourceFileName10_gpio_default_R7iygVKP}.s)
add_custom_command(
    OUTPUT ${DEPENDENT_TARGET_DIR10_gpio_default_R7iygVKP}/${sourceFileName10_gpio_default_R7iygVKP}.s
    COMMAND ${DEPENDENT_MP_BIN2HEX10_gpio_default_R7iygVKP} ${DEPENDENT_DEPENDENT_TARGET_ELF10_gpio_default_R7iygVKP} --image ${sourceFileName10_gpio_default_R7iygVKP} ${address10_gpio_default_R7iygVKP} ${mode10_gpio_default_R7iygVKP} -mdfp=C:/Users/simon/.mchp_packs/Microchip/dsPIC33E-GM-GP-MC-GU-MU_DFP/1.6.297/xc16 
    WORKING_DIRECTORY ${DEPENDENT_TARGET_DIR10_gpio_default_R7iygVKP}
    DEPENDS ${DEPENDENT_DEPENDENT_TARGET_ELF10_gpio_default_R7iygVKP})
add_custom_target(
    dependent_produced_source_artifact10_gpio_default_R7iygVKP 
    DEPENDS ${DEPENDENT_TARGET_DIR10_gpio_default_R7iygVKP}/${sourceFileName10_gpio_default_R7iygVKP}.s
    )
