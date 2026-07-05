cmake_minimum_required(VERSION 3.16)

project(bighw VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 同时兼容 Qt6/Qt5，并都带上 Multimedia
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Multimedia)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Multimedia)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        other.h
        other.cpp
        bullet.h
        bullet.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(bighw
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        enemy.h
        enemy.cpp
        paotai.h
        paotai.cpp
        button.h
        button.cpp
        coin.h
        coin.cpp
        coinmaker.h
        coinmaker.cpp
        protecter.h
        protecter.cpp
        shooter.h
        shooter.cpp
        bomb.h
        bomb.cpp
        map.h
        map.cpp
        shop.h
        shop.cpp
        card.h
        card.cpp
        normalenemy.h
        normalenemy.cpp
        plusenemy.h
        plusenemy.cpp
    )
else()
    if(ANDROID)
        add_library(bighw SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(bighw
            ${PROJECT_SOURCES}
        )
    endif()
endif()

# 链接时统一用 Qt::Multimedia（Qt5/Qt6 都能用）
target_link_libraries(bighw PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Multimedia
)

if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.bighw)
endif()

set_target_properties(bighw PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS bighw
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(bighw)
endif()