set(DEPENDENT_MP_BIN2HEX40_uart_default_wGu083SS "c:/Program Files/Microchip/xc16/v2.10/bin/xc16-bin2hex.exe")
set(DEPENDENT_DEPENDENT_TARGET_ELF40_uart_default_wGu083SS ${CMAKE_CURRENT_LIST_DIR}/../../../../out/40_uart/production/default-production.elf)
set(DEPENDENT_TARGET_DIR40_uart_default_wGu083SS ${CMAKE_CURRENT_LIST_DIR}/../../../../out/40_uart/production)
set(DEPENDENT_BYPRODUCTS40_uart_default_wGu083SS ${DEPENDENT_TARGET_DIR40_uart_default_wGu083SS}/${sourceFileName40_uart_default_wGu083SS}.s)
add_custom_command(
    OUTPUT ${DEPENDENT_TARGET_DIR40_uart_default_wGu083SS}/${sourceFileName40_uart_default_wGu083SS}.s
    COMMAND ${DEPENDENT_MP_BIN2HEX40_uart_default_wGu083SS} ${DEPENDENT_DEPENDENT_TARGET_ELF40_uart_default_wGu083SS} --image ${sourceFileName40_uart_default_wGu083SS} ${address40_uart_default_wGu083SS} ${mode40_uart_default_wGu083SS} -mdfp=C:/Users/simon/.mchp_packs/Microchip/dsPIC33E-GM-GP-MC-GU-MU_DFP/1.6.297/xc16 
    WORKING_DIRECTORY ${DEPENDENT_TARGET_DIR40_uart_default_wGu083SS}
    DEPENDS ${DEPENDENT_DEPENDENT_TARGET_ELF40_uart_default_wGu083SS})
add_custom_target(
    dependent_produced_source_artifact40_uart_default_wGu083SS 
    DEPENDS ${DEPENDENT_TARGET_DIR40_uart_default_wGu083SS}/${sourceFileName40_uart_default_wGu083SS}.s
    )
