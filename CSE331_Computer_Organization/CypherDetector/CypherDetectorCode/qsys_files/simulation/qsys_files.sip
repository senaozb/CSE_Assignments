set_global_assignment -entity "qsys_files" -library "lib_qsys_files" -name IP_TOOL_NAME "Qsys"
set_global_assignment -entity "qsys_files" -library "lib_qsys_files" -name IP_TOOL_VERSION "13.1"
set_global_assignment -entity "qsys_files" -library "lib_qsys_files" -name IP_TOOL_ENV "Qsys"
set_global_assignment -library "lib_qsys_files" -name SPD_FILE [file join $::quartus(sip_path) "../../qsys_files.spd"]
set_global_assignment -library "lib_qsys_files" -name MISC_FILE [file join $::quartus(sip_path) "../../qsys_files.qsys"]

set_global_assignment -library "lib_qsys_files" -name MISC_FILE [file join $::quartus(sip_path) "qsys_files.v"]
