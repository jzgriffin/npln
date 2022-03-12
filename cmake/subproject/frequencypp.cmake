include(SubProject)

FetchContent_Declare(frequencypp
    GIT_REPOSITORY https://github.com/nokurn/frequencypp
    GIT_TAG 0.1.1
    GIT_SHALLOW ON
)
FetchContent_Populate(frequencypp)
SubProject_Add(frequencypp
    QUIET
    CACHE_ARGS
        -DBUILD_SHARED_LIBS:BOOL=OFF
)
find_package(frequencypp REQUIRED)
