cmake_minimum_required(VERSION 3.16)
project(SFMLProject LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)

FetchContent_Declare(cpu6502
    GIT_REPOSITORY https://github.com/Artemia76/cpu6502.git
)
FetchContent_MakeAvailable(SFML cpu6502)

set  (PROJECT_SOURCES
    "src/main.cpp"
	"src/crt.cpp"
    "src/loop.cpp"
    "src/mainapp.cpp")

    source_group("src" FILES ${PROJECT_SOURCES})

add_executable( C64Emu ${PROJECT_SOURCES} )

target_link_libraries(C64Emu PRIVATE sfml-graphics M6502Lib )
target_compile_features(C64Emu PRIVATE cxx_std_17)

if(WIN32)
    add_custom_command(
        TARGET C64Emu
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:C64Emu>
        VERBATIM)
endif()

install(TARGETS C64Emu)
