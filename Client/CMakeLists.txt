cmake_minimum_required(VERSION 3.16)

project(Client LANGUAGES CXX)

set(PROJECT_SOURCES
        main.cpp
        pingclient.h pingclient.cpp
        connectingtablemodel.h connectingtablemodel.cpp
        broadcastlistener.h broadcastlistener.cpp
        actionbuttondelegate.h actionbuttondelegate.cpp
)

set(PROTO_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/proto/pingservice.proto
)

find_package(gRPC REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Widgets Protobuf Grpc Network)

qt_standard_project_setup()

qt_add_executable(Client
    ${PROJECT_SOURCES}
)

add_test(Client foo)

qt_add_protobuf(Client
    PROTO_FILES ${PROTO_FILES}
)

qt_add_grpc(Client
    CLIENT
    PROTO_FILES ${PROTO_FILES}
)

target_link_libraries(Client
    PRIVATE
        Qt6::Widgets
        Qt6::Network
        Qt6::Grpc
        gRPC::grpc++
        gRPC::grpc
)

install(TARGETS Client
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

