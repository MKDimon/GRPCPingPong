cmake_minimum_required(VERSION 3.16)

project(Server LANGUAGES CXX)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        grpcserver.h grpcserver.cpp
)


set(PROTO_GEN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/generated_proto)
set(PROTO_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/proto/pingservice.proto
)
set(PROTOBUF_GEN ${PROTO_GEN_DIR}/pingservice.pb.cc
    ${PROTO_GEN_DIR}/pingservice.pb.h
    ${PROTO_GEN_DIR}/pingservice.grpc.pb.cc
    ${PROTO_GEN_DIR}/pingservice.grpc.pb.h)

find_package(gRPC REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Widgets Protobuf Grpc Network)

qt_standard_project_setup()

find_program(PROTOC_EXECUTABLE protoc)
find_program(GRPC_CPP_PLUGIN grpc_cpp_plugin)

if(NOT PROTOC_EXECUTABLE)
message(FATAL_ERROR "protoc executable not found")
endif()

if(NOT GRPC_CPP_PLUGIN)
message(FATAL_ERROR "grpc_cpp_plugin executable not found")
endif()

add_custom_command(
    OUTPUT ${PROTOBUF_GEN}
    COMMAND ${PROTOC_EXECUTABLE} --cpp_out=${PROTO_GEN_DIR} --grpc_out=${PROTO_GEN_DIR} --plugin=protoc-gen-grpc=${GRPC_CPP_PLUGIN} --proto_path=${CMAKE_CURRENT_SOURCE_DIR}/proto pingservice.proto
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/proto/pingservice.proto
    COMMENT "Generating protobuf and gRPC files"
    COMMAND_EXPAND_LISTS
    VERBATIM
)

qt_add_executable(Server
    ${PROJECT_SOURCES}
    ${PROTOBUF_GEN}
)

target_link_libraries(Server
    PRIVATE
        Qt6::Widgets
        Qt6::Network
)
target_link_libraries(Server
    PRIVATE
        gRPC::grpc++
        gRPC::grpc
)

install(TARGETS Server
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
