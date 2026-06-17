set(DEPENDENT_MP_BIN2HEX41_uart_default_D85CN2hM "c:/Program Files/Microchip/xc16/v2.10/bin/xc16-bin2hex.exe")
set(DEPENDENT_DEPENDENT_TARGET_ELF41_uart_default_D85CN2hM ${CMAKE_CURRENT_LIST_DIR}/../../../../out/41_uart/production/default-production.elf)
set(DEPENDENT_TARGET_DIR41_uart_default_D85CN2hM ${CMAKE_CURRENT_LIST_DIR}/../../../../out/41_uart/production)
set(DEPENDENT_BYPRODUCTS41_uart_default_D85CN2hM ${DEPENDENT_TARGET_DIR41_uart_default_D85CN2hM}/${sourceFileName41_uart_default_D85CN2hM}.s)
add_custom_command(
    OUTPUT ${DEPENDENT_TARGET_DIR41_uart_default_D85CN2hM}/${sourceFileName41_uart_default_D85CN2hM}.s
    COMMAND ${DEPENDENT_MP_BIN2HEX41_uart_default_D85CN2hM} ${DEPENDENT_DEPENDENT_TARGET_ELF41_uart_default_D85CN2hM} --image ${sourceFileName41_uart_default_D85CN2hM} ${address41_uart_default_D85CN2hM} ${mode41_uart_default_D85CN2hM} -mdfp=C:/Users/simon/.mchp_packs/Microchip/dsPIC33E-GM-GP-MC-GU-MU_DFP/1.6.297/xc16 
    WORKING_DIRECTORY ${DEPENDENT_TARGET_DIR41_uart_default_D85CN2hM}
    DEPENDS ${DEPENDENT_DEPENDENT_TARGET_ELF41_uart_default_D85CN2hM})
add_custom_target(
    dependent_produced_source_artifact41_uart_default_D85CN2hM 
    DEPENDS ${DEPENDENT_TARGET_DIR41_uart_default_D85CN2hM}/${sourceFileName41_uart_default_D85CN2hM}.s
    )
