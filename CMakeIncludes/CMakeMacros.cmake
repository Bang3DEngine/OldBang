
#=================================================================
# Include guard ==================================================
#=================================================================
if (DEFINED BANG_CMAKE_MACROS_BUILT)
    return()
endif()
set(BANG_CMAKE_MACROS_BUILT ON)
#=================================================================
#=================================================================
#=================================================================

#=================================================================
# Macros =========================================================
#=================================================================
macro(list_replace LIST INDEX NEWVALUE)
    list(INSERT ${LIST} ${INDEX} ${NEWVALUE})
    MATH(EXPR __INDEX "${INDEX} + 1")
    list (REMOVE_AT ${LIST} ${__INDEX})
endmacro()

macro(get_soname __SONAME __SOPATH)
    execute_process(
      COMMAND /bin/sh -c "objdump -p ${__SOPATH} | grep SONAME | sed 's/.*SONAME.*\\s//g'"
      OUTPUT_VARIABLE ${__SONAME}
      OUTPUT_STRIP_TRAILING_WHITESPACE
    )
endmacro()

macro(get_list_sonames __SOPATH)
    set(__IDX 0)
    list(LENGTH ${__LIST} __COUNT)
    while(${__IDX} LESS ${__COUNT})
      list(GET ${__LIST} ${__IDX} __PATH)
      get_soname(${__PATH})
      list_replace(${__LIST} ${__IDX} ${__PATH})
      math(EXPR __IDX "${__IDX} + 1")
    endwhile()
endmacro()

macro(get_realpath __PATH)
    get_filename_component(__PATH ${__PATH} REALPATH)
endmacro()

macro(get_list_realpaths __LIST)
    set(__IDX 0)
    list(LENGTH ${__LIST} __COUNT)
    while(${__IDX} LESS ${__COUNT})
      list(GET ${__LIST} ${__IDX} __PATH)
      get_realpath(${__PATH})
      list_replace(${__LIST} ${__IDX} ${__PATH})
      math(EXPR __IDX "${__IDX} + 1")
    endwhile()
endmacro()

macro(add_misc_files)
     foreach(_it ${ARGN})
         if(NOT IS_DIRECTORY ${_it})
             get_filename_component(name ${_it} NAME)
             if(NOT ${_it} MATCHES "^/\\\\..*$;~$")
                set_source_files_properties(${_it} PROPERTIES HEADER_FILE_ONLY TRUE)
             endif()
         endif()
     endforeach()
endmacro()

#=================================================================
#=================================================================
#=================================================================
