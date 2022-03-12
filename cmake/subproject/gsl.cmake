include(SubProject)

FetchContent_Declare(gsl
    GIT_REPOSITORY https://github.com/microsoft/GSL
    GIT_TAG 4377f6e
    GIT_SHALLOW ON
)
FetchContent_Populate(gsl)
SubProject_Add(gsl
    QUIET
    CACHE_ARGS
        -DBUILD_SHARED_LIBS:BOOL=OFF
        -DGSL_TEST:BOOL=OFF
)
find_package(Microsoft.GSL REQUIRED)
