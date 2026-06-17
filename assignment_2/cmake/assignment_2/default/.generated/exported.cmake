set(DEPENDENT_MP_BIN2HEXassignment_2_default_TXSwxkEr "/opt/microchip/xc16/v2.10/bin/xc16-bin2hex")
set(DEPENDENT_DEPENDENT_TARGET_ELFassignment_2_default_TXSwxkEr "${CMAKE_CURRENT_LIST_DIR}/../../../../out/assignment_2/default.elf")
set(DEPENDENT_TARGET_DIRassignment_2_default_TXSwxkEr "${CMAKE_CURRENT_LIST_DIR}/../../../../out/assignment_2")
set(DEPENDENT_BYPRODUCTSassignment_2_default_TXSwxkEr ${DEPENDENT_TARGET_DIRassignment_2_default_TXSwxkEr}/${sourceFileNameassignment_2_default_TXSwxkEr}.s)
add_custom_command(
    OUTPUT ${DEPENDENT_TARGET_DIRassignment_2_default_TXSwxkEr}/${sourceFileNameassignment_2_default_TXSwxkEr}.s
    COMMAND ${DEPENDENT_MP_BIN2HEXassignment_2_default_TXSwxkEr} ${DEPENDENT_DEPENDENT_TARGET_ELFassignment_2_default_TXSwxkEr} --image ${sourceFileNameassignment_2_default_TXSwxkEr} ${addressassignment_2_default_TXSwxkEr} ${modeassignment_2_default_TXSwxkEr} -mdfp=/home/ric/.mchp_packs/Microchip/dsPIC33E-GM-GP-MC-GU-MU_DFP/1.6.297/xc16 
    WORKING_DIRECTORY ${DEPENDENT_TARGET_DIRassignment_2_default_TXSwxkEr}
    DEPENDS ${DEPENDENT_DEPENDENT_TARGET_ELFassignment_2_default_TXSwxkEr})
add_custom_target(
    dependent_produced_source_artifactassignment_2_default_TXSwxkEr 
    DEPENDS ${DEPENDENT_TARGET_DIRassignment_2_default_TXSwxkEr}/${sourceFileNameassignment_2_default_TXSwxkEr}.s
    )
