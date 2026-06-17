set(DEPENDENT_MP_BIN2HEX60_adc_default__KrBTrtX "c:/Program Files/Microchip/xc16/v2.10/bin/xc16-bin2hex.exe")
set(DEPENDENT_DEPENDENT_TARGET_ELF60_adc_default__KrBTrtX ${CMAKE_CURRENT_LIST_DIR}/../../../../out/60_adc/production/default-production.elf)
set(DEPENDENT_TARGET_DIR60_adc_default__KrBTrtX ${CMAKE_CURRENT_LIST_DIR}/../../../../out/60_adc/production)
set(DEPENDENT_BYPRODUCTS60_adc_default__KrBTrtX ${DEPENDENT_TARGET_DIR60_adc_default__KrBTrtX}/${sourceFileName60_adc_default__KrBTrtX}.s)
add_custom_command(
    OUTPUT ${DEPENDENT_TARGET_DIR60_adc_default__KrBTrtX}/${sourceFileName60_adc_default__KrBTrtX}.s
    COMMAND ${DEPENDENT_MP_BIN2HEX60_adc_default__KrBTrtX} ${DEPENDENT_DEPENDENT_TARGET_ELF60_adc_default__KrBTrtX} --image ${sourceFileName60_adc_default__KrBTrtX} ${address60_adc_default__KrBTrtX} ${mode60_adc_default__KrBTrtX} -mdfp=C:/Users/simon/.mchp_packs/Microchip/dsPIC33E-GM-GP-MC-GU-MU_DFP/1.6.297/xc16 
    WORKING_DIRECTORY ${DEPENDENT_TARGET_DIR60_adc_default__KrBTrtX}
    DEPENDS ${DEPENDENT_DEPENDENT_TARGET_ELF60_adc_default__KrBTrtX})
add_custom_target(
    dependent_produced_source_artifact60_adc_default__KrBTrtX 
    DEPENDS ${DEPENDENT_TARGET_DIR60_adc_default__KrBTrtX}/${sourceFileName60_adc_default__KrBTrtX}.s
    )
