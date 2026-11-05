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

**Code evidence:**

# 4. Accumulating Emotion Scores

**Question:**
Why is std::map or std::unordered_map suitable for accumulating emotion scores? State one difference
between them.
Container used: [std::map / std::unordered_map]
Reason for my choice: [Explain the key, value and why this container suits model-loaded labels.]
Code evidence: File: ___ Function: ___ Relevant line or fragment: ___

# 5. Separation of Responsibilities
   Question: Why should EmotionSmoother be separate from EmotionModel and main.cpp?
   Responsibility of main.cpp: [Your response]
   Responsibility of EmotionModel: [Your response]
   Responsibility of EmotionSmoother: [Your response]
   Code evidence: Relevant files/classes/functions: ___
# 6. Refactoring Summary
   Change 1: What changed? Where? Why was it useful?
   Change 2: What changed? Where? Why was it useful?
   Change 3: What changed? Where? Why was it useful?