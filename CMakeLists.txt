cmake_minimum_required(VERSION 3.22)

project(Ducker VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find JUCE - adjust path as needed or set JUCE_DIR environment variable
if(DEFINED ENV{JUCE_DIR})
    set(JUCE_DIR $ENV{JUCE_DIR})
else()
    set(JUCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../JUCE")
endif()

if(EXISTS "${JUCE_DIR}/CMakeLists.txt")
    add_subdirectory(${JUCE_DIR} ${CMAKE_BINARY_DIR}/JUCE)
else()
    find_package(JUCE CONFIG REQUIRED)
endif()

juce_add_plugin(Ducker
    VERSION 1.0.0
    COMPANY_NAME "Ian Fletcher Audio"
    COMPANY_WEBSITE "https://ianfletcher.audio"
    COMPANY_EMAIL "plugins@ianfletcher.audio"
    BUNDLE_ID "com.ianfletcheraudio.ducker"
    PLUGIN_MANUFACTURER_CODE IFlA
    PLUGIN_CODE Dukr
    FORMATS AU VST3 Standalone
    PRODUCT_NAME "Ducker"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD TRUE
    VST3_CATEGORIES "Fx" "Dynamics"
)

target_sources(Ducker
    PRIVATE
        Source/PluginProcessor.cpp
        Source/PluginEditor.cpp
        Source/DSP/Ducker.cpp
        Source/DSP/EnvelopeGenerator.cpp
        Source/DSP/SidechainProcessor.cpp
)

target_include_directories(Ducker
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Source
        ${CMAKE_CURRENT_SOURCE_DIR}/JuceLibraryCode
)

target_compile_definitions(Ducker
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
        JUCE_DISPLAY_SPLASH_SCREEN=0
)

target_link_libraries(Ducker
    PRIVATE
        juce::juce_audio_basics
        juce::juce_audio_devices
        juce::juce_audio_formats
        juce::juce_audio_plugin_client
        juce::juce_audio_processors
        juce::juce_audio_utils
        juce::juce_core
        juce::juce_data_structures
        juce::juce_dsp
        juce::juce_events
        juce::juce_graphics
        juce::juce_gui_basics
        juce::juce_gui_extra
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)
