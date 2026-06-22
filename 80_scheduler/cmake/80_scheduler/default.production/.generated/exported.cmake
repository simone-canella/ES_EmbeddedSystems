set(DEPENDENT_MP_BIN2HEX80_scheduler_default_zqiCfCKE "c:/Program Files/Microchip/xc16/v2.10/bin/xc16-bin2hex.exe")
set(DEPENDENT_DEPENDENT_TARGET_ELF80_scheduler_default_zqiCfCKE ${CMAKE_CURRENT_LIST_DIR}/../../../../out/80_scheduler/production/default-production.elf)
set(DEPENDENT_TARGET_DIR80_scheduler_default_zqiCfCKE ${CMAKE_CURRENT_LIST_DIR}/../../../../out/80_scheduler/production)
set(DEPENDENT_BYPRODUCTS80_scheduler_default_zqiCfCKE ${DEPENDENT_TARGET_DIR80_scheduler_default_zqiCfCKE}/${sourceFileName80_scheduler_default_zqiCfCKE}.s)
add_custom_command(
    OUTPUT ${DEPENDENT_TARGET_DIR80_scheduler_default_zqiCfCKE}/${sourceFileName80_scheduler_default_zqiCfCKE}.s
    COMMAND ${DEPENDENT_MP_BIN2HEX80_scheduler_default_zqiCfCKE} ${DEPENDENT_DEPENDENT_TARGET_ELF80_scheduler_default_zqiCfCKE} --image ${sourceFileName80_scheduler_default_zqiCfCKE} ${address80_scheduler_default_zqiCfCKE} ${mode80_scheduler_default_zqiCfCKE} -mdfp=C:/Users/simon/.mchp_packs/Microchip/dsPIC33E-GM-GP-MC-GU-MU_DFP/1.6.297/xc16 
    WORKING_DIRECTORY ${DEPENDENT_TARGET_DIR80_scheduler_default_zqiCfCKE}
    DEPENDS ${DEPENDENT_DEPENDENT_TARGET_ELF80_scheduler_default_zqiCfCKE})
add_custom_target(
    dependent_produced_source_artifact80_scheduler_default_zqiCfCKE 
    DEPENDS ${DEPENDENT_TARGET_DIR80_scheduler_default_zqiCfCKE}/${sourceFileName80_scheduler_default_zqiCfCKE}.s
    )
