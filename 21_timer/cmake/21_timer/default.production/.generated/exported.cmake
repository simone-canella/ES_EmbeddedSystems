set(DEPENDENT_MP_BIN2HEX21_timer_default_X89Xdx7h "c:/Program Files/Microchip/xc16/v2.10/bin/xc16-bin2hex.exe")
set(DEPENDENT_DEPENDENT_TARGET_ELF21_timer_default_X89Xdx7h ${CMAKE_CURRENT_LIST_DIR}/../../../../out/21_timer/production/default-production.elf)
set(DEPENDENT_TARGET_DIR21_timer_default_X89Xdx7h ${CMAKE_CURRENT_LIST_DIR}/../../../../out/21_timer/production)
set(DEPENDENT_BYPRODUCTS21_timer_default_X89Xdx7h ${DEPENDENT_TARGET_DIR21_timer_default_X89Xdx7h}/${sourceFileName21_timer_default_X89Xdx7h}.s)
add_custom_command(
    OUTPUT ${DEPENDENT_TARGET_DIR21_timer_default_X89Xdx7h}/${sourceFileName21_timer_default_X89Xdx7h}.s
    COMMAND ${DEPENDENT_MP_BIN2HEX21_timer_default_X89Xdx7h} ${DEPENDENT_DEPENDENT_TARGET_ELF21_timer_default_X89Xdx7h} --image ${sourceFileName21_timer_default_X89Xdx7h} ${address21_timer_default_X89Xdx7h} ${mode21_timer_default_X89Xdx7h} -mdfp=C:/Users/simon/.mchp_packs/Microchip/dsPIC33E-GM-GP-MC-GU-MU_DFP/1.6.297/xc16 
    WORKING_DIRECTORY ${DEPENDENT_TARGET_DIR21_timer_default_X89Xdx7h}
    DEPENDS ${DEPENDENT_DEPENDENT_TARGET_ELF21_timer_default_X89Xdx7h})
add_custom_target(
    dependent_produced_source_artifact21_timer_default_X89Xdx7h 
    DEPENDS ${DEPENDENT_TARGET_DIR21_timer_default_X89Xdx7h}/${sourceFileName21_timer_default_X89Xdx7h}.s
    )
