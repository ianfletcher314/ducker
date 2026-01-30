#pragma once

#include <cmath>

namespace DSPUtils
{
    // Level conversion
    inline float linearToDecibels(float linear)
    {
        return linear > 0.0f ? 20.0f * std::log10(linear) : -100.0f;
    }

    inline float decibelsToLinear(float dB)
    {
        return std::pow(10.0f, dB / 20.0f);
    }

    // Range mapping
    inline float mapRange(float value, float inMin, float inMax, float outMin, float outMax)
    {
        return outMin + (outMax - outMin) * (value - inMin) / (inMax - inMin);
    }

    // Clipping functions
    inline float softClip(float sample)
    {
        return std::tanh(sample);
    }

    inline float hardClip(float sample, float threshold = 1.0f)
    {
        return std::clamp(sample, -threshold, threshold);
    }

    // One-pole filter coefficient calculation
    inline float calculateCoefficient(double sampleRate, float timeMs)
    {
        if (timeMs <= 0.0f) return 1.0f;
        return 1.0f - std::exp(-1.0f / (static_cast<float>(sampleRate) * timeMs * 0.001f));
    }

    // Biquad coefficient structure
    struct BiquadCoeffs
    {
        float b0 = 1.0f, b1 = 0.0f, b2 = 0.0f;
        float a1 = 0.0f, a2 = 0.0f;
    };

    // High-pass filter coefficients
    inline BiquadCoeffs calcHighPass(double sampleRate, float freq, float q = 0.707f)
    {
        BiquadCoeffs c;
        float w0 = 2.0f * 3.14159265358979323846f * freq / static_cast<float>(sampleRate);
        float cosw0 = std::cos(w0);
        float sinw0 = std::sin(w0);
        float alpha = sinw0 / (2.0f * q);

        float a0 = 1.0f + alpha;
        c.b0 = ((1.0f + cosw0) / 2.0f) / a0;
        c.b1 = (-(1.0f + cosw0)) / a0;
        c.b2 = ((1.0f + cosw0) / 2.0f) / a0;
        c.a1 = (-2.0f * cosw0) / a0;
        c.a2 = (1.0f - alpha) / a0;

        return c;
    }

    // Low-pass filter coefficients
    inline BiquadCoeffs calcLowPass(double sampleRate, float freq, float q = 0.707f)
    {
        BiquadCoeffs c;
        float w0 = 2.0f * 3.14159265358979323846f * freq / static_cast<float>(sampleRate);
        float cosw0 = std::cos(w0);
        float sinw0 = std::sin(w0);
        float alpha = sinw0 / (2.0f * q);

        float a0 = 1.0f + alpha;
        c.b0 = ((1.0f - cosw0) / 2.0f) / a0;
        c.b1 = (1.0f - cosw0) / a0;
        c.b2 = ((1.0f - cosw0) / 2.0f) / a0;
        c.a1 = (-2.0f * cosw0) / a0;
        c.a2 = (1.0f - alpha) / a0;

        return c;
    }

    // Curve shape functions
    enum class CurveShape
    {
        Linear,
        Exponential,
        Logarithmic,
        SCurve
    };

    inline float applyCurveShape(float value, CurveShape shape)
    {
        switch (shape)
        {
            case CurveShape::Linear:
                return value;
            case CurveShape::Exponential:
                return value * value;
            case CurveShape::Logarithmic:
                return std::sqrt(value);
            case CurveShape::SCurve:
                // Smooth S-curve using sine
                return 0.5f * (1.0f - std::cos(value * 3.14159265358979323846f));
            default:
                return value;
        }
    }
}
