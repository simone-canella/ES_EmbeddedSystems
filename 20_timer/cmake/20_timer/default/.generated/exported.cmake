set(DEPENDENT_MP_BIN2HEX20_timer_default_HCMXp12j "c:/Program Files/Microchip/xc16/v2.10/bin/xc16-bin2hex.exe")
set(DEPENDENT_DEPENDENT_TARGET_ELF20_timer_default_HCMXp12j ${CMAKE_CURRENT_LIST_DIR}/../../../../out/20_timer/default.elf)
set(DEPENDENT_TARGET_DIR20_timer_default_HCMXp12j ${CMAKE_CURRENT_LIST_DIR}/../../../../out/20_timer)
set(DEPENDENT_BYPRODUCTS20_timer_default_HCMXp12j ${DEPENDENT_TARGET_DIR20_timer_default_HCMXp12j}/${sourceFileName20_timer_default_HCMXp12j}.s)
add_custom_command(
    OUTPUT ${DEPENDENT_TARGET_DIR20_timer_default_HCMXp12j}/${sourceFileName20_timer_default_HCMXp12j}.s
    COMMAND ${DEPENDENT_MP_BIN2HEX20_timer_default_HCMXp12j} ${DEPENDENT_DEPENDENT_TARGET_ELF20_timer_default_HCMXp12j} --image ${sourceFileName20_timer_default_HCMXp12j} ${address20_timer_default_HCMXp12j} ${mode20_timer_default_HCMXp12j} -mdfp=C:/Users/simon/.mchp_packs/Microchip/dsPIC33E-GM-GP-MC-GU-MU_DFP/1.6.297/xc16 
    WORKING_DIRECTORY ${DEPENDENT_TARGET_DIR20_timer_default_HCMXp12j}
    DEPENDS ${DEPENDENT_DEPENDENT_TARGET_ELF20_timer_default_HCMXp12j})
add_custom_target(
    dependent_produced_source_artifact20_timer_default_HCMXp12j 
    DEPENDS ${DEPENDENT_TARGET_DIR20_timer_default_HCMXp12j}/${sourceFileName20_timer_default_HCMXp12j}.s
    )
