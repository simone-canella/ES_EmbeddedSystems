set(DEPENDENT_MP_BIN2HEX22_timer_default_IISWX71S "c:/Program Files/Microchip/xc16/v2.10/bin/xc16-bin2hex.exe")
set(DEPENDENT_DEPENDENT_TARGET_ELF22_timer_default_IISWX71S ${CMAKE_CURRENT_LIST_DIR}/../../../../out/22_timer/production/default-production.elf)
set(DEPENDENT_TARGET_DIR22_timer_default_IISWX71S ${CMAKE_CURRENT_LIST_DIR}/../../../../out/22_timer/production)
set(DEPENDENT_BYPRODUCTS22_timer_default_IISWX71S ${DEPENDENT_TARGET_DIR22_timer_default_IISWX71S}/${sourceFileName22_timer_default_IISWX71S}.s)
add_custom_command(
    OUTPUT ${DEPENDENT_TARGET_DIR22_timer_default_IISWX71S}/${sourceFileName22_timer_default_IISWX71S}.s
    COMMAND ${DEPENDENT_MP_BIN2HEX22_timer_default_IISWX71S} ${DEPENDENT_DEPENDENT_TARGET_ELF22_timer_default_IISWX71S} --image ${sourceFileName22_timer_default_IISWX71S} ${address22_timer_default_IISWX71S} ${mode22_timer_default_IISWX71S} -mdfp=C:/Users/simon/.mchp_packs/Microchip/dsPIC33E-GM-GP-MC-GU-MU_DFP/1.6.297/xc16 
    WORKING_DIRECTORY ${DEPENDENT_TARGET_DIR22_timer_default_IISWX71S}
    DEPENDS ${DEPENDENT_DEPENDENT_TARGET_ELF22_timer_default_IISWX71S})
add_custom_target(
    dependent_produced_source_artifact22_timer_default_IISWX71S 
    DEPENDS ${DEPENDENT_TARGET_DIR22_timer_default_IISWX71S}/${sourceFileName22_timer_default_IISWX71S}.s
    )
