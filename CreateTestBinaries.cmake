include(CMakeParseArguments)

#Function could have been just simple with 2 parameters
#But we use the cmake_parse_arguments, keeping in mind to use it to parse much more options for future
function(add_test_project)
    set(options)
    set(oneValueArgs TARGET INPUT_FILE_NAME)
    set(multiArgsValue)
    cmake_parse_arguments(ARGS "${options}" "${oneValueArgs}" "${multiArgsValue}" ${ARGN})
    if("${ARGS_TARGET}" STREQUAL "" OR "${ARGS_INPUT_FILE_NAME}" STREQUAL "")
        message(FATAL_ERROR "Invalid arguments please provide target name and the source")
    endif()

    add_executable(${ARGS_TARGET} ${ARGS_INPUT_FILE_NAME})

    target_link_libraries(${ARGS_TARGET} GTest::gtest)

endfunction(add_test_project)