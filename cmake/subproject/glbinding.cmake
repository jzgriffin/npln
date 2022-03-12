include(SubProject)

FetchContent_Declare(glbinding
    GIT_REPOSITORY https://github.com/cginternals/glbinding
    GIT_TAG 70e76f9
    GIT_SHALLOW ON
)
FetchContent_Populate(glbinding)

# Patch the FindKHR.cmake module to not attempt add_library if the KHR::KHR target already exists.
# This is necessary to avoid a conflict when glbinding is imported via globjects.
set(module_file "${glbinding_SOURCE_DIR}/cmake/FindKHR.cmake")
file(READ "${module_file}" module_src)
string(REPLACE
    "if(KHR_INCLUDE_DIR)"
    "if(KHR_INCLUDE_DIR AND NOT TARGET KHR::KHR)"
    module_src
    "${module_src}"
)
file(WRITE "${module_file}" "${module_src}")

SubProject_Add(glbinding
    QUIET
    CACHE_ARGS
        -DBUILD_SHARED_LIBS:BOOL=OFF
        -DOPTION_BUILD_TESTS:BOOL=OFF
        -DOPTION_BUILD_TOOLS:BOOL=OFF
        -DOPTION_BUILD_EXAMPLES:BOOL=OFF
)
find_package(glbinding REQUIRED)
