cmake_minimum_required(VERSION 3.22.1)

project("randogen")

add_library(${CMAKE_PROJECT_NAME} SHARED
    CommonCpp/CommonLibJNI.cpp
    CommonCpp/SortedRollsMaker.cpp

    DndCpp/DndJNI.cpp
    DndCpp/DndChancesCounter.cpp
    DndCpp/DndRollButtonTextFormatter.cpp
    DndCpp/DndRollRulesLib.cpp

    WodCpp/WodJNI.cpp
    WodCpp/WodChanceCounter.cpp
    WodCpp/WodSuccessCounter.cpp

    RollAndKeepCpp/RollAndKeepJNI.cpp
    RollAndKeepCpp/RollAndKeepRulesLib.cpp
    RollAndKeepCpp/RollAndKeepChanceAsync.cpp
    RollAndKeepCpp/RollAndKeepChanceCounter.cpp
)

target_compile_features(${CMAKE_PROJECT_NAME} PRIVATE cxx_std_20)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/CommonCpp
    ${CMAKE_CURRENT_SOURCE_DIR}/DndCpp
    ${CMAKE_CURRENT_SOURCE_DIR}/WodCpp
    ${CMAKE_CURRENT_SOURCE_DIR}/RollAndKeepCpp
)

target_link_libraries(${CMAKE_PROJECT_NAME}
    android
    log
)