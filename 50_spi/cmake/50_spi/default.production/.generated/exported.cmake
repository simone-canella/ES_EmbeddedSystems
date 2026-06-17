set(DEPENDENT_MP_BIN2HEX50_spi_default_L3n6kYJ3 "c:/Program Files/Microchip/xc16/v2.10/bin/xc16-bin2hex.exe")
set(DEPENDENT_DEPENDENT_TARGET_ELF50_spi_default_L3n6kYJ3 ${CMAKE_CURRENT_LIST_DIR}/../../../../out/50_spi/production/default-production.elf)
set(DEPENDENT_TARGET_DIR50_spi_default_L3n6kYJ3 ${CMAKE_CURRENT_LIST_DIR}/../../../../out/50_spi/production)
set(DEPENDENT_BYPRODUCTS50_spi_default_L3n6kYJ3 ${DEPENDENT_TARGET_DIR50_spi_default_L3n6kYJ3}/${sourceFileName50_spi_default_L3n6kYJ3}.s)
add_custom_command(
    OUTPUT ${DEPENDENT_TARGET_DIR50_spi_default_L3n6kYJ3}/${sourceFileName50_spi_default_L3n6kYJ3}.s
    COMMAND ${DEPENDENT_MP_BIN2HEX50_spi_default_L3n6kYJ3} ${DEPENDENT_DEPENDENT_TARGET_ELF50_spi_default_L3n6kYJ3} --image ${sourceFileName50_spi_default_L3n6kYJ3} ${address50_spi_default_L3n6kYJ3} ${mode50_spi_default_L3n6kYJ3} -mdfp=C:/Users/simon/.mchp_packs/Microchip/dsPIC33E-GM-GP-MC-GU-MU_DFP/1.6.297/xc16 
    WORKING_DIRECTORY ${DEPENDENT_TARGET_DIR50_spi_default_L3n6kYJ3}
    DEPENDS ${DEPENDENT_DEPENDENT_TARGET_ELF50_spi_default_L3n6kYJ3})
add_custom_target(
    dependent_produced_source_artifact50_spi_default_L3n6kYJ3 
    DEPENDS ${DEPENDENT_TARGET_DIR50_spi_default_L3n6kYJ3}/${sourceFileName50_spi_default_L3n6kYJ3}.s
    )
