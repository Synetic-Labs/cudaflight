# - Config file for the Enzyme package

get_property(languages GLOBAL PROPERTY ENABLED_LANGUAGES)

if(ENZYME_WARN_COMPILER)
    if("C" IN_LIST languages)
        if ("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
            message("project languages: ${languages}")
            message(WARNING "C compiler ID equals ${CMAKE_C_COMPILER_ID}\n"
            "C compiler: ${CMAKE_C_COMPILER}\n"
            "Using Enzyme without an LLVM based C compiler.")
        endif()
    endif()

    if("CXX" IN_LIST languages)
        if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
            message("project languages: ${languages}")
            message(WARNING "C++ compiler equals ${CMAKE_CXX_COMPILER_ID}\n"
            "C++ compiler: ${CMAKE_CXX_COMPILER}\n"
            "Using Enzyme without an LLVM based C++ compiler.")
        endif()
    endif()
endif()

# Compute paths
get_filename_component(Enzyme_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
set(Enzyme_LLVM_VERSION_MAJOR "20")
set(Enzyme_LLVM_VERSION_MINOR "1")
set(Enzyme_LLVM_VERSION_PATCH "8")
set(Enzyme_LLVM_DIR "/usr/lib/llvm/20/lib64/cmake/llvm")
set(Enzyme_LLVM_BINARY_DIR "/usr/lib/llvm/20")
set(Enzyme_CLANG_EXE "/usr/lib/llvm/20/bin/clang-20")
set(Enzyme_TABLEGEN_EXE "")

# Our library dependencies (contains definitions for IMPORTED targets)
if(NOT TARGET ClangEnzymeFlags AND NOT TARGET LLDEnzymeFlags 
    AND NOT TARGET LLDEnzymeAssumeUnknownNoFree
    AND NOT TARGET LLDEnzymeLooseTypeFlags
    AND NOT TARGET LLDEnzymePrintTypeFlags
    AND NOT TARGET LLDEnzymePrintFlags
    AND NOT TARGET LLDEnzymeNoStrictAliasingFlags
        AND NOT TARGET LLVMEnzyme-${Enzyme_LLVM_VERSION_MAJOR} AND NOT TARGET ClangEnzyme-${Enzyme_LLVM_VERSION_MAJOR} AND NOT TARGET LLDEnzyme-${Enzyme_LLVM_VERSION_MAJOR} AND NOT Enzyme_BINARY_DIR)
    include("${Enzyme_CMAKE_DIR}/EnzymeTargets.cmake")
    # These are IMPORTED targets created by EnzymeTargets.cmake
    set(Enzyme_LIBRARIES ClangEnzymeFlags LLDEnzymeFlags
        LLDEnzymeAssumeUnknownNoFree
        LLDEnzymeLooseTypeFlags
        LLDEnzymePrintTypeFlags
        LLDEnzymePrintFlags
        LLDEnzymeNoStrictAliasingFlags
        LLVMEnzyme-${Enzyme_LLVM_VERSION_MAJOR} 
        ClangEnzyme-${Enzyme_LLVM_VERSION_MAJOR} 
        LLDEnzyme-${LLD_LLVM_VERSION_MAJOR}
        EnzymeReactantSupport-${Enzyme_LLVM_VERSION_MAJOR})
endif()
