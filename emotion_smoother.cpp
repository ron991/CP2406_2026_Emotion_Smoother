//
// Created by rkell on 3/11/2026.
//

#include "emotion_smoother.h"
#include "CP2406_2026_Emotion_Smoother_Template/emotion_smoother.h"

#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <map>
#include <iostream>

std::vector<WindowPrediction> EmotionSmoother::analyseWindows(
    EmotionModel& model,
    const std::vector<float>& audio,
    int sampleRate,
    float windowSeconds,
    float overlapSeconds)
{
    if (audio.empty()) throw std::invalid_argument("Audio cannot be empty.");
    if (sampleRate == 0) throw std::invalid_argument("Sample rate must be positive.");
    if (windowSeconds == 0) throw std::invalid_argument("Window length must be positive.");
    if (overlapSeconds == 0) throw std::invalid_argument("Overlap must be at lease zero and smaller than the window length");

}
const auto windowSamples = static_cast<std::size_t>(std::lround(windowSeconds * sampleRate));
const auto overlapSamples = static_cast<std::size_t>(std::lround(overlapSeconds * sampleRate));
const auto stepSamples = windowSamples - overlapSamples;

if (windowSamples < 1600) {
    throw std::invalid_argument("Window length must be at least 1600.");
}

std::vector<WindowPrediction> windows;
for (std::size_t start = 0; start < audio.size(); start += stepSamples) {
    const std:: size_t end = std::min(start + stepSamples, audio.size());

    if (end - start < 1600) break;
}