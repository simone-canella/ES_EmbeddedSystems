set(DEPENDENT_MP_BIN2HEXassignment_2_default_GNqKI2pC "/opt/microchip/xc16/v2.10/bin/xc16-bin2hex")
set(DEPENDENT_DEPENDENT_TARGET_ELFassignment_2_default_GNqKI2pC "${CMAKE_CURRENT_LIST_DIR}/../../../../out/assignment_2/production/default-production.elf")
set(DEPENDENT_TARGET_DIRassignment_2_default_GNqKI2pC "${CMAKE_CURRENT_LIST_DIR}/../../../../out/assignment_2/production")
set(DEPENDENT_BYPRODUCTSassignment_2_default_GNqKI2pC ${DEPENDENT_TARGET_DIRassignment_2_default_GNqKI2pC}/${sourceFileNameassignment_2_default_GNqKI2pC}.s)
add_custom_command(
    OUTPUT ${DEPENDENT_TARGET_DIRassignment_2_default_GNqKI2pC}/${sourceFileNameassignment_2_default_GNqKI2pC}.s
    COMMAND ${DEPENDENT_MP_BIN2HEXassignment_2_default_GNqKI2pC} ${DEPENDENT_DEPENDENT_TARGET_ELFassignment_2_default_GNqKI2pC} --image ${sourceFileNameassignment_2_default_GNqKI2pC} ${addressassignment_2_default_GNqKI2pC} ${modeassignment_2_default_GNqKI2pC} -mdfp=/home/samuisa/.mchp_packs/Microchip/dsPIC33E-GM-GP-MC-GU-MU_DFP/1.6.297/xc16 
    WORKING_DIRECTORY ${DEPENDENT_TARGET_DIRassignment_2_default_GNqKI2pC}
    DEPENDS ${DEPENDENT_DEPENDENT_TARGET_ELFassignment_2_default_GNqKI2pC})
add_custom_target(
    dependent_produced_source_artifactassignment_2_default_GNqKI2pC 
    DEPENDS ${DEPENDENT_TARGET_DIRassignment_2_default_GNqKI2pC}/${sourceFileNameassignment_2_default_GNqKI2pC}.s
    )
