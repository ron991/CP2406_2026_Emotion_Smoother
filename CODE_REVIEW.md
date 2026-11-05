# 1. Student and Project Details

**Student name**
Ronald Kelly

**Student ID:**
13817176

**Project option:**
Emotion Smoother

# 3. Sorting the Predictions

**Question:**
Why does EmotionModel::predict receive the waveform by value, and what happens when
normalise (waveform) modifies it?

**Your response:**
`EmotionModel::predict` takes the `std::vector<float> waveform` by value, which means it creates a localised copy in
memory. When `normalise(waveform)` modifies the data, it only alters that local copy. The original audio vector residing
in `main()` remains untouched and safe to be reused for subsequent predictions.

