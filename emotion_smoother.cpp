#include "../include/emotion_smoother.h"
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
    if (sampleRate <= 0) throw std::invalid_argument("Sample rate must be positive.");
    if (windowSeconds <= 0.0F) throw std::invalid_argument("Window length must be positive.");
    if (overlapSeconds < 0.0F || overlapSeconds >= windowSeconds) {
        throw std::invalid_argument("Overlap must be at least zero and smaller than the window length.");
    }

    const auto windowSamples = static_cast<std::size_t>(std::lround(windowSeconds * sampleRate));
    const auto overlapSamples = static_cast<std::size_t>(std::lround(overlapSeconds * sampleRate));
    const auto stepSamples = windowSamples - overlapSamples;

    if (windowSamples < 1600) {
        throw std::invalid_argument("Each window must contain at least 1,600 samples.");
    }

    std::vector<WindowPrediction> windows;

    for (std::size_t start = 0; start < audio.size(); start += stepSamples) {
        const std::size_t end = std::min(start + windowSamples, audio.size());
        
        if (end - start < 1600) break; 

        std::vector<float> segment(
            audio.begin() + static_cast<std::ptrdiff_t>(start),
            audio.begin() + static_cast<std::ptrdiff_t>(end));

        auto predictions = model.predict(std::move(segment));
        
        if (predictions.size() < 2) {
            throw std::runtime_error("The model must return at least two emotion classes.");
        }

        windows.push_back({
            static_cast<float>(start) / static_cast<float>(sampleRate),
            static_cast<float>(end) / static_cast<float>(sampleRate),
            predictions[0],
            predictions[1]
        });

        if (end == audio.size()) break;
    }
    return windows;
}

SmoothedEmotion EmotionSmoother::smoothPredictions(const std::vector<WindowPrediction>& windows) {
    if (windows.empty()) return {"", 0.0F, false};

    std::map<std::string, float> emotionScores;
    std::map<std::string, float> topOnlyScores;
    float totalScore = 0.0F;

    for (const auto& window : windows) {
        const float topWeight = 2.0F;
        const float secondWeight = 1.0F;

        float topScore = window.topEmotion.probability * topWeight;
        float secondScore = window.secondEmotion.probability * secondWeight;

        emotionScores[window.topEmotion.label] += topScore;
        topOnlyScores[window.topEmotion.label] += topScore;
        totalScore += topScore;

        emotionScores[window.secondEmotion.label] += secondScore;
        totalScore += secondScore;
    }

    if (totalScore <= 0.0F) return {"", 0.0F, false};

    std::vector<std::pair<std::string, float>> sortedScores(emotionScores.begin(), emotionScores.end());
    
    std::sort(sortedScores.begin(), sortedScores.end(), [&topOnlyScores](const auto& a, const auto& b) {
        const float tolerance = 1e-6F;
        if (std::abs(a.second - b.second) > tolerance) return a.second > b.second; 
        
        float topA = topOnlyScores[a.first];
        float topB = topOnlyScores[b.first];
        if (std::abs(topA - topB) > tolerance) return topA > topB; 
        
        return a.first < b.first; 
    });

    float confidence = (sortedScores.front().second / totalScore) * 100.0F;
    return {sortedScores.front().first, confidence, true};
}