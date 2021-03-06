# Definitions for all external bundled libraries

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/CMakeModules)
include(DownloadExternals)
include(ExternalProject)

# Catch
add_library(catch-single-include INTERFACE)
target_include_directories(catch-single-include INTERFACE catch/single_include)

# Crypto++
add_subdirectory(cryptopp)

# Dynarmic
if (ARCHITECTURE_x86_64)
    # Dynarmic will skip defining xbyak if it's already defined, we then define it below
    add_library(xbyak INTERFACE)
    option(DYNARMIC_TESTS OFF)
    set(DYNARMIC_NO_BUNDLED_FMT ON)
    add_subdirectory(dynarmic)
endif()

# libfmt
add_subdirectory(fmt)
add_library(fmt::fmt ALIAS fmt)

# Glad
add_subdirectory(glad)

# inih
add_subdirectory(inih)

# Nihstro
add_library(nihstro-headers INTERFACE)
target_include_directories(nihstro-headers INTERFACE ./nihstro/include)

# Open Source Archives
add_subdirectory(open_source_archives)

# SoundTouch
add_subdirectory(soundtouch)
# The SoundTouch target doesn't export the necessary include paths as properties by default
target_include_directories(SoundTouch INTERFACE ./soundtouch/include)

# Teakra
add_subdirectory(teakra)

# Xbyak
if (ARCHITECTURE_x86_64)
    # Defined before "dynarmic" above
    # add_library(xbyak INTERFACE)
    target_include_directories(xbyak INTERFACE ./xbyak/xbyak)
    target_compile_definitions(xbyak INTERFACE XBYAK_NO_OP_NAMES)
endif()

# Zstandard
add_subdirectory(zstd/build/cmake EXCLUDE_FROM_ALL)
target_include_directories(libzstd_static INTERFACE ./zstd/lib)

# Cubeb
if (ENABLE_CUBEB)
    set(BUILD_TESTS OFF CACHE BOOL "")
    add_subdirectory(cubeb EXCLUDE_FROM_ALL)
endif()

# Discord RPC
if (USE_DISCORD_PRESENCE)
    add_subdirectory(discord-rpc EXCLUDE_FROM_ALL)
    target_include_directories(discord-rpc INTERFACE ./discord-rpc/include)
endif()

# LibreSSL
set(LIBRESSL_SKIP_INSTALL ON CACHE BOOL "")
add_subdirectory(libressl EXCLUDE_FROM_ALL)
target_include_directories(ssl INTERFACE ./libressl/include)
target_compile_definitions(ssl PRIVATE -DHAVE_INET_NTOP)

# lurlparser
add_subdirectory(lurlparser EXCLUDE_FROM_ALL)

# httplib
add_library(httplib INTERFACE)
target_include_directories(httplib INTERFACE ./httplib)

# JSON
add_library(json INTERFACE)
target_include_directories(json INTERFACE ./json)

# portable-file-dialogs
add_library(portable-file-dialogs INTERFACE)
target_include_directories(portable-file-dialogs INTERFACE ./portable-file-dialogs/include)

add_subdirectory(lodepng)
add_subdirectory(semver)
add_subdirectory(clipp)
add_subdirectory(easywsclient)
