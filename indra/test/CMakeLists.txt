# -*- cmake -*-

project(lltest)

include(00-Common)
include(LLCommon)
include(LLCoreHttp)
include(LLInventory)
include(LLMath)
include(LLMessage)
include(LLVFS)
include(LLXML)
include(Linking)
include(Tut)
include(LLAddBuildTest)

include_directories(
    ${LLCOMMON_INCLUDE_DIRS}
    ${LLCOREHTTP_INCLUDE_DIRS}
    ${LLDATABASE_INCLUDE_DIRS}
    ${LLMATH_INCLUDE_DIRS}
    ${LLMESSAGE_INCLUDE_DIRS}
    ${LLINVENTORY_INCLUDE_DIRS}
    ${LLVFS_INCLUDE_DIRS}
    ${LLXML_INCLUDE_DIRS}
    ${TUT_INCLUDE_DIR}
    )
include_directories(SYSTEM
    ${LLCOMMON_SYSTEM_INCLUDE_DIRS}
    ${LLXML_SYSTEM_INCLUDE_DIRS}
    )

set(test_SOURCE_FILES
    io.cpp
    llapp_tut.cpp
    llbuffer_tut.cpp
    lldoubledispatch_tut.cpp
    llevents_tut.cpp
    llhttpdate_tut.cpp
    llhttpnode_tut.cpp
    lliohttpserver_tut.cpp
    llmessageconfig_tut.cpp
    llpermissions_tut.cpp
    llpipeutil.cpp
    llsaleinfo_tut.cpp
    llsdmessagebuilder_tut.cpp
    llsdmessagereader_tut.cpp
    llsd_new_tut.cpp
    llsdutil_tut.cpp
    llservicebuilder_tut.cpp
    llstreamtools_tut.cpp
    lltemplatemessagebuilder_tut.cpp
    lltut.cpp
    message_tut.cpp
    test.cpp
    )

set(test_HEADER_FILES
    CMakeLists.txt

    debug.h
    llpipeutil.h
    llsdtraits.h
    lltut.h
    )

if(NOT WINDOWS)
    list(APPEND test_SOURCE_FILES
        llmessagetemplateparser_tut.cpp
    )
endif()

set_source_files_properties(${test_HEADER_FILES}
                            PROPERTIES HEADER_FILE_ONLY TRUE)

list(APPEND test_SOURCE_FILES ${test_HEADER_FILES})

add_executable(lltest ${test_SOURCE_FILES})

target_link_libraries(lltest
    ${LLDATABASE_LIBRARIES}
    ${LLINVENTORY_LIBRARIES}
    ${LLMESSAGE_LIBRARIES}
    ${LLMATH_LIBRARIES}
    ${LLVFS_LIBRARIES}
    ${LLXML_LIBRARIES}
    ${LLCOMMON_LIBRARIES}
    ${LLCOREHTTP_LIBRARIES}
    ${EXPAT_LIBRARIES}
    ${PTHREAD_LIBRARY}
    ${WINDOWS_LIBRARIES}
    ${BOOST_PROGRAM_OPTIONS_LIBRARY}
    ${BOOST_REGEX_LIBRARY}
    ${BOOST_COROUTINE_LIBRARY}
    ${BOOST_CONTEXT_LIBRARY}
    ${BOOST_SYSTEM_LIBRARY}
    ${DL_LIBRARY}
    ${GOOGLE_PERFTOOLS_LIBRARIES}
    )

set(TEST_EXE $<TARGET_FILE:lltest>)

set_test_path(DYLD_LIBRARY_PATH)

ll_test_command(command
  "${DYLD_LIBRARY_PATH}"
  "${TEST_EXE}"
  "--output=${CMAKE_CURRENT_BINARY_DIR}/cpp_test_results.txt"
  "--touch=${CMAKE_CURRENT_BINARY_DIR}/cpp_tests_ok.txt")

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/cpp_tests_ok.txt
  COMMAND ${command}
  DEPENDS lltest
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  COMMENT "C++ unit tests"
  )

set(test_results ${CMAKE_CURRENT_BINARY_DIR}/cpp_tests_ok.txt)

# This should cause the test executable to be built, but not
# run if LL_TESTS is disabled. This will hopefully keep the
# tests up to date with any code changes changes even if
# developers choose to disable LL_TESTS.
if(LL_TESTS)
    add_custom_target(tests_ok ALL DEPENDS ${test_results})
endif()
