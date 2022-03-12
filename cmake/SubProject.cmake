# Copyright 2020-2022 Jeremiah Griffin
#
# Permission to use, copy, modify, and/or distribute this software for any
# purpose with or without fee is hereby granted, provided that the above
# copyright notice and this permission notice appear in all copies.
#
# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
# WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
# ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
# WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
# ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
# OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

include_guard(GLOBAL)

#[=======================================================================[.rst:
SubProject
---------------

.. only:: html

   .. contents::

Commands
^^^^^^^^

Sub-Project Definition
"""""""""""""""""""""""""""
.. command:: SubProject_Add
  The ``SubProject_Add()`` function configures, builds, and installs a CMake
  project inside of the build tree at configuration time. This is in contrast
  to the ExternalProject module, which performs these steps at build time.
  .. code-block:: cmake
    SubProject_Add(<name> [<option>...])

  **Options:**
    Most of the time, FetchContent is used to acquire the source artifacts of
    the sub-project. In that case, the default options for this function will
    automatically detect where the sub-project should go. The directory layout
    can be specified using ``SOURCE_DIR``, ``BINARY_DIR``, and ``INSTALL_DIR``
    if a custom layout is needed (e.g. FetchContent is not used) or desired.

    ``QUIET``
      Run the CMake commands with standard output disabled.

    ``NO_INHERIT_PREFIX_PATH``
      Do not automatically forward ``CMAKE_PREFIX_PATH`` to the sub-project.
      Without this option, ``CMAKE_PREFIX_PATH`` is passed to the sub-project
      via the initial cache.

    ``NO_INHERIT_MODULE_PATH``
      Do not automatically forward ``CMAKE_MODULE_PATH`` to the sub-project.
      Without this option, ``CMAKE_MODULE_PATH`` is passed to the sub-project
      via the initial cache.

    ``CONFIGURATION <config>``
      Configuration or build type to use for the sub-project. For generators
      that support multiple configurations, this will be the only
      configuration built. Defaults to ``Release``.

    ``SOURCE_DIR <dir>``
      Search for the sub-project source code in the given directory.
      Defaults to ``<name>_SOURCE_DIR``, which is the variable set by
      ``FetchContent_Populate``.

    ``BINARY_DIR <dir>``
      Place the sub-project build artifacts in the given directory. Defaults
      to ``<name>_BINARY_DIR``, which is the variable set by
      ``FetchContent_Populate``, or ``<SUB_PROJECT_BASE>/<name>-build``.

    ``INSTALL_DIR <dir>``
      Install the sub-project to the given directory. Defaults to
      ``<SUB_PROJECT_BASE>/<name>-install``.

    ``CACHE_ARGS <arg>...``
      Pass the given CMake variables to the sub-project configuration via the
      initial cache. The arguments must be of the form ``-Dvar:type=value``.
      This is the preferred way to pass variables, as opposed to
      ``CONFIGURE_ARGS``, because it is not limited in the length of the
      value.

    ``CONFIGURE_ARGS <arg>...``
      Pass the given arguments to the CMake command for configuring the
      sub-project. Variable definitions should be passed in ``CACHE_ARGS``
      instead.

    ``BUILD_ARGS <arg>...``
      Pass the given arguments to the CMake command for building the
      sub-project.

    ``INSTALL_ARGS <arg>...``
      Pass the given arguments to the CMake command for installing the
      sub-project.

#]=======================================================================]

include(FetchContent)
include(CMakeParseArguments)

define_property(DIRECTORY PROPERTY "SUB_PROJECT_BASE" INHERITED
    BRIEF_DOCS "Base directory for Sub Project storage."
    FULL_DOCS
    "See documentation of the SubProject_Add() function in the SubProject "
    "module."
)

function(SubProject_Add name)
    cmake_parse_arguments(args
        "QUIET;NO_INHERIT_PREFIX_PATH;NO_INHERIT_MODULE_PATH"
        "CONFIGURATION"
        "SOURCE_DIR;BINARY_DIR;INSTALL_DIR;CACHE_ARGS;CONFIGURE_ARGS;BUILD_ARGS;INSTALL_ARGS"
        ${ARGN}
    )

    # Default to release build configuration.
    if(args_CONFIGURATION)
        set(configuration "${args_CONFIGURATION}")
    else()
        set(configuration Release)
    endif()

    # Choose the directories to use, with defaults relative to the base.
    get_property(base_dir DIRECTORY PROPERTY SUB_PROJECT_BASE)
    if(NOT base_dir)
        set(base_dir "${CMAKE_BINARY_DIR}/_subprojects")
    endif()

    if(args_SOURCE_DIR)
        set(source_dir "${args_SOURCE_DIR}")
    elseif(${name}_SOURCE_DIR)
        set(source_dir "${${name}_SOURCE_DIR}")
    else()
        message(FATAL_ERROR "Sub-project ${name} is missing SOURCE_DIR")
    endif()
    if(args_BINARY_DIR)
        set(binary_dir "${args_BINARY_DIR}")
    elseif(${name}_BINARY_DIR)
        set(binary_dir "${${name}_BINARY_DIR}")
    else()
        set(binary_dir "${base_dir}/${name}-binary")
    endif()
    if(args_INSTALL_DIR)
        set(install_dir "${args_INSTALL_DIR}")
    elseif(${name}_INSTALL_DIR)
        set(install_dir "${${name}_INSTALL_DIR}")
    else()
        set(install_dir "${base_dir}/${name}-install")
    endif()

    # Compile the configure, build, and install commands.
    set(configure_cmd "${CMAKE_COMMAND}")
    set(build_cmd "${CMAKE_COMMAND}")
    set(install_cmd "${CMAKE_COMMAND}")

    # Inherit the prefix and module paths through CACHE_ARGS if applicable.
    if(NOT args_NO_INHERIT_PREFIX_PATH)
        if(NOT args_CACHE_ARGS)
            set(args_CACHE_ARGS "")
        endif()
        list(APPEND args_CACHE_ARGS
            "-DCMAKE_PREFIX_PATH:STRING=${CMAKE_PREFIX_PATH}")
    endif()
    if(NOT args_NO_INHERIT_MODULE_PATH)
        if(NOT args_CACHE_ARGS)
            set(args_CACHE_ARGS "")
        endif()
        list(APPEND args_CACHE_ARGS
            "-DCMAKE_MODULE_PATH:STRING=${CMAKE_MODULE_PATH}")
    endif()

    # Write the initial cache args.
    if(args_CACHE_ARGS)
        set(initial_cache "")
        # The logic for building each set directive is further complicated by
        # CMake's lack of support for nested lists. Variable definitions that
        # contain lists are treated as a single `-Dx:y=z` element followed by
        # an arbitrary number of `w` elements representing the tail of the
        # nested list. To account for this, we iteratively build the set
        # directive and only append it to the initial cache script when we
        # encounter another variable beginning or the argument list ends.
        foreach(element ${args_CACHE_ARGS})
            # Elements beginning with -D start a variable definition.
            # Otherwise, the line is considered a continuation of a nested
            # list.
            if("${element}" MATCHES "^-D(.*)")
                # Strip the -D prefix.
                set(element "${CMAKE_MATCH_1}")
                if(NOT "${set_line}" STREQUAL "")
                    string(APPEND set_line
                        "${accumulator}\" CACHE ${var_type} \"Initial cache\")")
                    string(APPEND initial_cache "${set_line}\n")
                    set(accumulator "")
                    set(set_line "")
                endif()
                if("${element}" MATCHES "^([^:]+):([^=]+)=(.*)$")
                    set(var_name "${CMAKE_MATCH_1}")
                    set(var_type "${CMAKE_MATCH_2}")
                    set(var_value "${CMAKE_MATCH_3}")
                    set(set_line "set(${var_name} \"${var_value}")
                else()
                    message(WARNING
                        "Invalid syntax in cache argument ${element}")
                endif()
            else()
                string(APPEND accumulator ";${element}")
            endif()
        endforeach()
        if(NOT "${set_line}" STREQUAL "")
            string(APPEND set_line
                "${accumulator}\" CACHE ${var_type} \"Initial cache\")")
            string(APPEND initial_cache "${set_line}\n")
        endif()

        set(cache_file "${base_dir}/${name}-initial-cache.cmake")
        file(WRITE "${cache_file}" "${initial_cache}")
        list(APPEND configure_cmd "-C${cache_file}")
    endif()

    # Propagate the generator and toolchain settings from the current
    # environment.
    if(CMAKE_EXTRA_GENERATOR)
        list(APPEND configure_cmd
            "-G${CMAKE_EXTRA_GENERATOR} - ${CMAKE_GENERATOR}")
    else()
        list(APPEND configure_cmd "-G${CMAKE_GENERATOR}")
    endif()
    if(CMAKE_GENERATOR_PLATFORM)
        list(APPEND configure_cmd "-A${CMAKE_GENERATOR_PLATFORM}")
    endif()
    if(CMAKE_GENERATOR_TOOLSET)
        list(APPEND configure_cmd "-T${CMAKE_GENERATOR_TOOLSET}")
    endif()
    if(CMAKE_GENERATOR_INSTANCE)
        list(APPEND configure_cmd
            "-DCMAKE_GENERATOR_INSTANCE:INTERNAL=${CMAKE_GENERATOR_INSTANCE}")
    endif()
    if(CMAKE_TOOLCHAIN_FILE)
        list(APPEND configure_cmd --toolchain "${CMAKE_TOOLCHAIN_FILE}")
    endif()

    # Select the configuration. This module currently only supports single
    # configurations, so on multi-configuration generators the configuration
    # types are restricted.
    get_property(is_multi_config GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
    if(is_multi_config)
        list(APPEND configure_cmd
            "-DCMAKE_CONFIGURATION_TYPES:STRING=${configuration}")
        list(APPEND build_cmd --config "${configuration}")
        list(APPEND install_cmd --config "${configuration}")
    else()
        list(APPEND configure_cmd
            "-DCMAKE_BUILD_TYPE:STRING=${configuration}")
    endif()

    list(APPEND configure_cmd "-S${source_dir}")
    list(APPEND configure_cmd "-B${binary_dir}")
    list(APPEND configure_cmd -Wno-dev)
    list(APPEND configure_cmd --install-prefix "${install_dir}")
    if(args_CONFIGURE_ARGS)
        list(APPEND configure_cmd ${args_CONFIGURE_ARGS})
    endif()

    list(APPEND build_cmd --build "${binary_dir}")
    if(args_BUILD_ARGS)
        list(APPEND build_cmd ${args_BUILD_ARGS})
    endif()

    list(APPEND install_cmd --install "${binary_dir}")
    if(args_INSTALL_ARGS)
        list(APPEND install_cmd ${args_INSTALL_ARGS})
    endif()

    # Execute the configure, build, and install steps.
    if(args_QUIET)
        set(quiet OUTPUT_QUIET)
    else()
        set(quiet "")
    endif()
    message(STATUS "Configuring sub-project ${name}")
    execute_process(COMMAND ${configure_cmd} ${quiet})
    message(STATUS "Building sub-project ${name}")
    execute_process(COMMAND ${build_cmd} ${quiet})
    message(STATUS "Installing sub-project ${name}")
    execute_process(COMMAND ${install_cmd} ${quiet})

    # Add the installation directory to the prefix path to enable
    # find_package.
    list(APPEND CMAKE_PREFIX_PATH "${install_dir}")
    set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} PARENT_SCOPE)

    # Export the source, binary, and install directories.
    set(${name}_SOURCE_DIR "${source_dir}" PARENT_SCOPE)
    set(${name}_BINARY_DIR "${binary_dir}" PARENT_SCOPE)
    set(${name}_INSTALL_DIR "${install_dir}" PARENT_SCOPE)
endfunction()
