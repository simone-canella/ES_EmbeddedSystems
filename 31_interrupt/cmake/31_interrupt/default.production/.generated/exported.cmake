set(DEPENDENT_MP_BIN2HEX31_interrupt_default_sJPZMnGx "c:/Program Files/Microchip/xc16/v2.10/bin/xc16-bin2hex.exe")
set(DEPENDENT_DEPENDENT_TARGET_ELF31_interrupt_default_sJPZMnGx ${CMAKE_CURRENT_LIST_DIR}/../../../../out/31_interrupt/production/default-production.elf)
set(DEPENDENT_TARGET_DIR31_interrupt_default_sJPZMnGx ${CMAKE_CURRENT_LIST_DIR}/../../../../out/31_interrupt/production)
set(DEPENDENT_BYPRODUCTS31_interrupt_default_sJPZMnGx ${DEPENDENT_TARGET_DIR31_interrupt_default_sJPZMnGx}/${sourceFileName31_interrupt_default_sJPZMnGx}.s)
add_custom_command(
    OUTPUT ${DEPENDENT_TARGET_DIR31_interrupt_default_sJPZMnGx}/${sourceFileName31_interrupt_default_sJPZMnGx}.s
    COMMAND ${DEPENDENT_MP_BIN2HEX31_interrupt_default_sJPZMnGx} ${DEPENDENT_DEPENDENT_TARGET_ELF31_interrupt_default_sJPZMnGx} --image ${sourceFileName31_interrupt_default_sJPZMnGx} ${address31_interrupt_default_sJPZMnGx} ${mode31_interrupt_default_sJPZMnGx} -mdfp=C:/Users/simon/.mchp_packs/Microchip/dsPIC33E-GM-GP-MC-GU-MU_DFP/1.6.297/xc16 
    WORKING_DIRECTORY ${DEPENDENT_TARGET_DIR31_interrupt_default_sJPZMnGx}
    DEPENDS ${DEPENDENT_DEPENDENT_TARGET_ELF31_interrupt_default_sJPZMnGx})
add_custom_target(
    dependent_produced_source_artifact31_interrupt_default_sJPZMnGx 
    DEPENDS ${DEPENDENT_TARGET_DIR31_interrupt_default_sJPZMnGx}/${sourceFileName31_interrupt_default_sJPZMnGx}.s
    )
