set(DEPENDENT_MP_BIN2HEX30_interrupt_default_uwuY0AYI "c:/Program Files/Microchip/xc16/v2.10/bin/xc16-bin2hex.exe")
set(DEPENDENT_DEPENDENT_TARGET_ELF30_interrupt_default_uwuY0AYI ${CMAKE_CURRENT_LIST_DIR}/../../../../out/30_interrupt/production/default-production.elf)
set(DEPENDENT_TARGET_DIR30_interrupt_default_uwuY0AYI ${CMAKE_CURRENT_LIST_DIR}/../../../../out/30_interrupt/production)
set(DEPENDENT_BYPRODUCTS30_interrupt_default_uwuY0AYI ${DEPENDENT_TARGET_DIR30_interrupt_default_uwuY0AYI}/${sourceFileName30_interrupt_default_uwuY0AYI}.s)
add_custom_command(
    OUTPUT ${DEPENDENT_TARGET_DIR30_interrupt_default_uwuY0AYI}/${sourceFileName30_interrupt_default_uwuY0AYI}.s
    COMMAND ${DEPENDENT_MP_BIN2HEX30_interrupt_default_uwuY0AYI} ${DEPENDENT_DEPENDENT_TARGET_ELF30_interrupt_default_uwuY0AYI} --image ${sourceFileName30_interrupt_default_uwuY0AYI} ${address30_interrupt_default_uwuY0AYI} ${mode30_interrupt_default_uwuY0AYI} -mdfp=C:/Users/simon/.mchp_packs/Microchip/dsPIC33E-GM-GP-MC-GU-MU_DFP/1.6.297/xc16 
    WORKING_DIRECTORY ${DEPENDENT_TARGET_DIR30_interrupt_default_uwuY0AYI}
    DEPENDS ${DEPENDENT_DEPENDENT_TARGET_ELF30_interrupt_default_uwuY0AYI})
add_custom_target(
    dependent_produced_source_artifact30_interrupt_default_uwuY0AYI 
    DEPENDS ${DEPENDENT_TARGET_DIR30_interrupt_default_uwuY0AYI}/${sourceFileName30_interrupt_default_uwuY0AYI}.s
    )
