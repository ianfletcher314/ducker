# Ducker - Sidechain Ducker Plugin

A professional sidechain ducking audio plugin built with JUCE.

## Features

### Main Controls
- **Threshold** (-60 to 0 dB) - Sidechain trigger level
- **Duck Amount** (0 to -40 dB) - How much to reduce the signal when triggered
- **Attack** (0.1ms to 100ms) - How quickly ducking engages
- **Hold** (0ms to 500ms) - Time to maintain full ducking after trigger
- **Release** (10ms to 2000ms) - How quickly signal returns to normal
- **Range/Floor** - Minimum level when fully ducked
- **Look-ahead** (0-20ms) - For transparent, click-free ducking
- **Mix** - Parallel ducking (wet/dry blend)

### Curve Shapes
- Linear
- Exponential
- Logarithmic
- S-Curve

### Sidechain Filters
- High-pass filter (20Hz - 2kHz) - Remove low frequencies from detection
- Low-pass filter (1kHz - 20kHz) - Remove high frequencies from detection
- Sidechain listen mode - Monitor the filtered sidechain signal

### Advanced Features
- Zero-crossing detection for click-free ducking
- Tempo sync option for hold and release times
- Visual envelope display showing duck curve
- Input/Output level meters
- Gain reduction meter

## Use Cases

- **EDM Sidechain Pumping** - Classic 4-on-the-floor pumping effect
- **Podcast Ducking** - Automatically lower music under voice
- **De-essing Style Ducking** - Frequency-specific ducking
- **Creative Rhythmic Effects** - Tempo-synced ducking patterns

## Building

### Using Projucer
1. Open `Ducker.jucer` in Projucer
2. Export to your preferred IDE (Xcode, Visual Studio, etc.)
3. Build the project

### Using CMake
```bash
mkdir build && cd build
cmake .. -DJUCE_DIR=/path/to/JUCE
cmake --build . --config Release
```

## Requirements

- JUCE 7.0 or later
- C++17 compatible compiler
- macOS 10.13+ / Windows 10+ / Linux

## License

Copyright (c) Ian Fletcher Audio. All rights reserved.
