find_package(common REQUIRED)
find_package(math REQUIRED)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/../floah-layout")
include(floahVersionString)

set(NAME floah-put)
set(TYPE module)
set(INCLUDE_DIR "include/floah-put")
set(SRC_DIR "src")

set(HEADERS
    ${INCLUDE_DIR}/input_context.h
    ${INCLUDE_DIR}/input_element.h
)

set(SOURCES
    ${SRC_DIR}/input_context.cpp
    ${SRC_DIR}/input_element.cpp
)

set(DEPS_PUBLIC
    common::common
    floah-layout
    math::math
)

set(DEPS_PRIVATE

)

make_target(
    NAME ${NAME}
    TYPE ${TYPE}
    VERSION ${FLOAH_VERSION}
    WARNINGS WERROR
    HEADERS "${HEADERS}"
    SOURCES "${SOURCES}"
    DEPS_PUBLIC "${DEPS_PUBLIC}"
    DEPS_PRIVATE "${DEPS_PRIVATE}"
)

install_target(
    NAME ${NAME}
    TYPE ${TYPE}
    VERSION ${FLOAH_VERSION}
    HEADERS "${HEADERS}"
)

target_compile_definitions(
    ${NAME}
    PRIVATE
        FLOAH_VERSION_MAJOR=${FLOAH_VERSION_MAJOR}
        FLOAH_VERSION_MINOR=${FLOAH_VERSION_MINOR}
        FLOAH_VERSION_PATCH=${FLOAH_VERSION_PATCH}
)
