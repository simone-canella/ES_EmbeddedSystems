set(DEPENDENT_MP_BIN2HEX42_uart_default_UkogqIc3 "c:/Program Files/Microchip/xc16/v2.10/bin/xc16-bin2hex.exe")
set(DEPENDENT_DEPENDENT_TARGET_ELF42_uart_default_UkogqIc3 ${CMAKE_CURRENT_LIST_DIR}/../../../../out/42_uart/production/default-production.elf)
set(DEPENDENT_TARGET_DIR42_uart_default_UkogqIc3 ${CMAKE_CURRENT_LIST_DIR}/../../../../out/42_uart/production)
set(DEPENDENT_BYPRODUCTS42_uart_default_UkogqIc3 ${DEPENDENT_TARGET_DIR42_uart_default_UkogqIc3}/${sourceFileName42_uart_default_UkogqIc3}.s)
add_custom_command(
    OUTPUT ${DEPENDENT_TARGET_DIR42_uart_default_UkogqIc3}/${sourceFileName42_uart_default_UkogqIc3}.s
    COMMAND ${DEPENDENT_MP_BIN2HEX42_uart_default_UkogqIc3} ${DEPENDENT_DEPENDENT_TARGET_ELF42_uart_default_UkogqIc3} --image ${sourceFileName42_uart_default_UkogqIc3} ${address42_uart_default_UkogqIc3} ${mode42_uart_default_UkogqIc3} -mdfp=C:/Users/simon/.mchp_packs/Microchip/dsPIC33E-GM-GP-MC-GU-MU_DFP/1.6.297/xc16 
    WORKING_DIRECTORY ${DEPENDENT_TARGET_DIR42_uart_default_UkogqIc3}
    DEPENDS ${DEPENDENT_DEPENDENT_TARGET_ELF42_uart_default_UkogqIc3})
add_custom_target(
    dependent_produced_source_artifact42_uart_default_UkogqIc3 
    DEPENDS ${DEPENDENT_TARGET_DIR42_uart_default_UkogqIc3}/${sourceFileName42_uart_default_UkogqIc3}.s
    )
