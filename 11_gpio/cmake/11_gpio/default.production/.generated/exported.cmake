set(DEPENDENT_MP_BIN2HEX11_gpio_default_TX2kwOsV "c:/Program Files/Microchip/xc16/v2.10/bin/xc16-bin2hex.exe")
set(DEPENDENT_DEPENDENT_TARGET_ELF11_gpio_default_TX2kwOsV ${CMAKE_CURRENT_LIST_DIR}/../../../../out/11_gpio/production/default-production.elf)
set(DEPENDENT_TARGET_DIR11_gpio_default_TX2kwOsV ${CMAKE_CURRENT_LIST_DIR}/../../../../out/11_gpio/production)
set(DEPENDENT_BYPRODUCTS11_gpio_default_TX2kwOsV ${DEPENDENT_TARGET_DIR11_gpio_default_TX2kwOsV}/${sourceFileName11_gpio_default_TX2kwOsV}.s)
add_custom_command(
    OUTPUT ${DEPENDENT_TARGET_DIR11_gpio_default_TX2kwOsV}/${sourceFileName11_gpio_default_TX2kwOsV}.s
    COMMAND ${DEPENDENT_MP_BIN2HEX11_gpio_default_TX2kwOsV} ${DEPENDENT_DEPENDENT_TARGET_ELF11_gpio_default_TX2kwOsV} --image ${sourceFileName11_gpio_default_TX2kwOsV} ${address11_gpio_default_TX2kwOsV} ${mode11_gpio_default_TX2kwOsV} -mdfp=C:/Users/simon/.mchp_packs/Microchip/dsPIC33E-GM-GP-MC-GU-MU_DFP/1.6.297/xc16 
    WORKING_DIRECTORY ${DEPENDENT_TARGET_DIR11_gpio_default_TX2kwOsV}
    DEPENDS ${DEPENDENT_DEPENDENT_TARGET_ELF11_gpio_default_TX2kwOsV})
add_custom_target(
    dependent_produced_source_artifact11_gpio_default_TX2kwOsV 
    DEPENDS ${DEPENDENT_TARGET_DIR11_gpio_default_TX2kwOsV}/${sourceFileName11_gpio_default_TX2kwOsV}.s
    )
