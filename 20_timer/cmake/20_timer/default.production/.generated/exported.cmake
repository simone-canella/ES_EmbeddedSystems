set(DEPENDENT_MP_BIN2HEX20_timer_default_yreke6rT "c:/Program Files/Microchip/xc16/v2.10/bin/xc16-bin2hex.exe")
set(DEPENDENT_DEPENDENT_TARGET_ELF20_timer_default_yreke6rT ${CMAKE_CURRENT_LIST_DIR}/../../../../out/20_timer/production/default-production.elf)
set(DEPENDENT_TARGET_DIR20_timer_default_yreke6rT ${CMAKE_CURRENT_LIST_DIR}/../../../../out/20_timer/production)
set(DEPENDENT_BYPRODUCTS20_timer_default_yreke6rT ${DEPENDENT_TARGET_DIR20_timer_default_yreke6rT}/${sourceFileName20_timer_default_yreke6rT}.s)
add_custom_command(
    OUTPUT ${DEPENDENT_TARGET_DIR20_timer_default_yreke6rT}/${sourceFileName20_timer_default_yreke6rT}.s
    COMMAND ${DEPENDENT_MP_BIN2HEX20_timer_default_yreke6rT} ${DEPENDENT_DEPENDENT_TARGET_ELF20_timer_default_yreke6rT} --image ${sourceFileName20_timer_default_yreke6rT} ${address20_timer_default_yreke6rT} ${mode20_timer_default_yreke6rT} -mdfp=C:/Users/simon/.mchp_packs/Microchip/dsPIC33E-GM-GP-MC-GU-MU_DFP/1.6.297/xc16 
    WORKING_DIRECTORY ${DEPENDENT_TARGET_DIR20_timer_default_yreke6rT}
    DEPENDS ${DEPENDENT_DEPENDENT_TARGET_ELF20_timer_default_yreke6rT})
add_custom_target(
    dependent_produced_source_artifact20_timer_default_yreke6rT 
    DEPENDS ${DEPENDENT_TARGET_DIR20_timer_default_yreke6rT}/${sourceFileName20_timer_default_yreke6rT}.s
    )
