set(DEPENDENT_MP_BIN2HEX51_spi_default_rd8yzVYh "c:/Program Files/Microchip/xc16/v2.10/bin/xc16-bin2hex.exe")
set(DEPENDENT_DEPENDENT_TARGET_ELF51_spi_default_rd8yzVYh ${CMAKE_CURRENT_LIST_DIR}/../../../../out/51_spi/production/default-production.elf)
set(DEPENDENT_TARGET_DIR51_spi_default_rd8yzVYh ${CMAKE_CURRENT_LIST_DIR}/../../../../out/51_spi/production)
set(DEPENDENT_BYPRODUCTS51_spi_default_rd8yzVYh ${DEPENDENT_TARGET_DIR51_spi_default_rd8yzVYh}/${sourceFileName51_spi_default_rd8yzVYh}.s)
add_custom_command(
    OUTPUT ${DEPENDENT_TARGET_DIR51_spi_default_rd8yzVYh}/${sourceFileName51_spi_default_rd8yzVYh}.s
    COMMAND ${DEPENDENT_MP_BIN2HEX51_spi_default_rd8yzVYh} ${DEPENDENT_DEPENDENT_TARGET_ELF51_spi_default_rd8yzVYh} --image ${sourceFileName51_spi_default_rd8yzVYh} ${address51_spi_default_rd8yzVYh} ${mode51_spi_default_rd8yzVYh} -mdfp=C:/Users/simon/.mchp_packs/Microchip/dsPIC33E-GM-GP-MC-GU-MU_DFP/1.6.297/xc16 
    WORKING_DIRECTORY ${DEPENDENT_TARGET_DIR51_spi_default_rd8yzVYh}
    DEPENDS ${DEPENDENT_DEPENDENT_TARGET_ELF51_spi_default_rd8yzVYh})
add_custom_target(
    dependent_produced_source_artifact51_spi_default_rd8yzVYh 
    DEPENDS ${DEPENDENT_TARGET_DIR51_spi_default_rd8yzVYh}/${sourceFileName51_spi_default_rd8yzVYh}.s
    )
