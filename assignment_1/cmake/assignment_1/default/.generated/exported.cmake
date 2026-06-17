set(DEPENDENT_MP_BIN2HEXassignment_1_default_a_s9q9zO "/opt/microchip/xc16/v2.10/bin/xc16-bin2hex")
set(DEPENDENT_DEPENDENT_TARGET_ELFassignment_1_default_a_s9q9zO "${CMAKE_CURRENT_LIST_DIR}/../../../../out/assignment_1/default.elf")
set(DEPENDENT_TARGET_DIRassignment_1_default_a_s9q9zO "${CMAKE_CURRENT_LIST_DIR}/../../../../out/assignment_1")
set(DEPENDENT_BYPRODUCTSassignment_1_default_a_s9q9zO ${DEPENDENT_TARGET_DIRassignment_1_default_a_s9q9zO}/${sourceFileNameassignment_1_default_a_s9q9zO}.s)
add_custom_command(
    OUTPUT ${DEPENDENT_TARGET_DIRassignment_1_default_a_s9q9zO}/${sourceFileNameassignment_1_default_a_s9q9zO}.s
    COMMAND ${DEPENDENT_MP_BIN2HEXassignment_1_default_a_s9q9zO} ${DEPENDENT_DEPENDENT_TARGET_ELFassignment_1_default_a_s9q9zO} --image ${sourceFileNameassignment_1_default_a_s9q9zO} ${addressassignment_1_default_a_s9q9zO} ${modeassignment_1_default_a_s9q9zO} -mdfp=/home/ric/.mchp_packs/Microchip/dsPIC33E-GM-GP-MC-GU-MU_DFP/1.6.297/xc16 
    WORKING_DIRECTORY ${DEPENDENT_TARGET_DIRassignment_1_default_a_s9q9zO}
    DEPENDS ${DEPENDENT_DEPENDENT_TARGET_ELFassignment_1_default_a_s9q9zO})
add_custom_target(
    dependent_produced_source_artifactassignment_1_default_a_s9q9zO 
    DEPENDS ${DEPENDENT_TARGET_DIRassignment_1_default_a_s9q9zO}/${sourceFileNameassignment_1_default_a_s9q9zO}.s
    )
