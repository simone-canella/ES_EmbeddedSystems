set(DEPENDENT_MP_BIN2HEXassignment_1_default_5NNy5Qb4 "/opt/microchip/xc16/v2.10/bin/xc16-bin2hex")
set(DEPENDENT_DEPENDENT_TARGET_ELFassignment_1_default_5NNy5Qb4 "${CMAKE_CURRENT_LIST_DIR}/../../../../out/assignment_1/production/default-production.elf")
set(DEPENDENT_TARGET_DIRassignment_1_default_5NNy5Qb4 "${CMAKE_CURRENT_LIST_DIR}/../../../../out/assignment_1/production")
set(DEPENDENT_BYPRODUCTSassignment_1_default_5NNy5Qb4 ${DEPENDENT_TARGET_DIRassignment_1_default_5NNy5Qb4}/${sourceFileNameassignment_1_default_5NNy5Qb4}.s)
add_custom_command(
    OUTPUT ${DEPENDENT_TARGET_DIRassignment_1_default_5NNy5Qb4}/${sourceFileNameassignment_1_default_5NNy5Qb4}.s
    COMMAND ${DEPENDENT_MP_BIN2HEXassignment_1_default_5NNy5Qb4} ${DEPENDENT_DEPENDENT_TARGET_ELFassignment_1_default_5NNy5Qb4} --image ${sourceFileNameassignment_1_default_5NNy5Qb4} ${addressassignment_1_default_5NNy5Qb4} ${modeassignment_1_default_5NNy5Qb4} -mdfp=/home/ric/.mchp_packs/Microchip/dsPIC33E-GM-GP-MC-GU-MU_DFP/1.6.297/xc16 
    WORKING_DIRECTORY ${DEPENDENT_TARGET_DIRassignment_1_default_5NNy5Qb4}
    DEPENDS ${DEPENDENT_DEPENDENT_TARGET_ELFassignment_1_default_5NNy5Qb4})
add_custom_target(
    dependent_produced_source_artifactassignment_1_default_5NNy5Qb4 
    DEPENDS ${DEPENDENT_TARGET_DIRassignment_1_default_5NNy5Qb4}/${sourceFileNameassignment_1_default_5NNy5Qb4}.s
    )
