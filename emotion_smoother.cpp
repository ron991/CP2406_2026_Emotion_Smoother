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
    if (audio.empty()) throw std::invalid_argument("audio should not be empty");
    if (sampleRate == 0) throw std::invalid_argument("sampleRate should not be 0");
    if (windowSeconds == 0) throw std::invalid_argument("windowSeconds should not be 0");
    if (overlapSeconds == 0) throw std::invalid_argument("overlapSeconds should not be 0");

}