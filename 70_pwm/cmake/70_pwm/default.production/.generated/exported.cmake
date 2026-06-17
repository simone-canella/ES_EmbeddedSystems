set(DEPENDENT_MP_BIN2HEX70_pwm_default_9f0L0zm3 "c:/Program Files/Microchip/xc16/v2.10/bin/xc16-bin2hex.exe")
set(DEPENDENT_DEPENDENT_TARGET_ELF70_pwm_default_9f0L0zm3 ${CMAKE_CURRENT_LIST_DIR}/../../../../out/70_pwm/production/default-production.elf)
set(DEPENDENT_TARGET_DIR70_pwm_default_9f0L0zm3 ${CMAKE_CURRENT_LIST_DIR}/../../../../out/70_pwm/production)
set(DEPENDENT_BYPRODUCTS70_pwm_default_9f0L0zm3 ${DEPENDENT_TARGET_DIR70_pwm_default_9f0L0zm3}/${sourceFileName70_pwm_default_9f0L0zm3}.s)
add_custom_command(
    OUTPUT ${DEPENDENT_TARGET_DIR70_pwm_default_9f0L0zm3}/${sourceFileName70_pwm_default_9f0L0zm3}.s
    COMMAND ${DEPENDENT_MP_BIN2HEX70_pwm_default_9f0L0zm3} ${DEPENDENT_DEPENDENT_TARGET_ELF70_pwm_default_9f0L0zm3} --image ${sourceFileName70_pwm_default_9f0L0zm3} ${address70_pwm_default_9f0L0zm3} ${mode70_pwm_default_9f0L0zm3} -mdfp=C:/Users/simon/.mchp_packs/Microchip/dsPIC33E-GM-GP-MC-GU-MU_DFP/1.6.297/xc16 
    WORKING_DIRECTORY ${DEPENDENT_TARGET_DIR70_pwm_default_9f0L0zm3}
    DEPENDS ${DEPENDENT_DEPENDENT_TARGET_ELF70_pwm_default_9f0L0zm3})
add_custom_target(
    dependent_produced_source_artifact70_pwm_default_9f0L0zm3 
    DEPENDS ${DEPENDENT_TARGET_DIR70_pwm_default_9f0L0zm3}/${sourceFileName70_pwm_default_9f0L0zm3}.s
    )
