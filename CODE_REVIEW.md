# 1. Student and Project Details

**Student name**
Ronald Kelly

**Student ID:**
13817176

**Project option:**
Emotion Smoother

# 2. Waveform Parameter and Normalisation

**Question:**
Why does EmotionModel::predict receive the waveform by value, and what happens when
normalise (waveform) modifies it?

**Your response:**
`EmotionModel::predict` takes the `std::vector<float> waveform` by value, which means it creates a localised copy in
memory. When `normalise(waveform)` modifies the data, it only alters that local copy. The original audio vector residing
in `main()` remains untouched and safe to be reused for subsequent predictions.

**Code evidence:**
File: `emotion_model.cpp`
Function: `EmotionModel::predict`
Relevant line or fragment: `void EmotionModel::predict(std::vector<float> waveform)`

# 3. Sorting the Predictions

**Question:** Why are predictions sorted before being returned, and how does sorting support selection of the top two
emotions?

**Your response:**
The raw output logits from the ONNX machine learning model reflect label order rather than confidence magnitude. Sorting
the resulting `Prediction` structs in descending order of probability places the highest-confidence emotion strictly at
index `0` (`topEmotion`) and the second-highest at index `1` (`second
Emotion`).

# 4. Accumulating Emotion Scores

**Question:**
Why is std::map or std::unordered_map suitable for accumulating emotion scores? State one difference
between them.

**Container used:**

`std::unordered_map`

**Reason for my choice:**

`std::unordered_map` provides O (1) average time complexity for lookups and insertion via a hash table, making it highly
efficient for tallying string labels. `std::map` uses a binary tree which requires O (log n) time and constant string
comparisons, making it slower.

**Code evidence:**

File: `EmotionSmoother.h` Function: `struct SmoothedEmotion` Relevant line or fragment:
`std::unordered_map<std::string, float> accumulatedScores;`

# 5. Separation of Responsibilities

**Question:** Why should EmotionSmoother be separate from EmotionModel and main.cpp?

**Responsibility of main.cpp:**

Acts as the application coordinator. It manages the CLI user menu, collects input, coordinates audio acquisition
(microphone recording via `AudioRecorder` or WAV file loading), handles console streat error recovery via
`clearInput()`, and invokes window analysis loops.

**Responsibility of EmotionModel:** 

Encapsulates ONNX Runtime interactions and neural network execution. It loads asset

**Responsibility of EmotionSmoother:** [Your response]

**Code evidence:** Relevant files/classes/functions: ___

# 6. Refactoring Summary

**Change 1:**
What changed? Where? Why was it useful?

**Change 2:** What changed?
Where? Why was it useful?

**Change 3:**
What changed? Where? Why was it useful?

# 7. Testing Evidence

Document at least four meaningful tests, including an empty window list, a tie, repeated emotions and a short final
audio window.

| Test                         | Purpose                                           | Input/setup                                                                  | Expected result                                                             | Actual result           | Pass/fail and code location                 |
|:-----------------------------|:--------------------------------------------------|:-----------------------------------------------------------------------------|:----------------------------------------------------------------------------|:------------------------|:--------------------------------------------|
| **Empty window list**        | Ensure the algorithm safely rejects empty inputs. | `std::vector<WindowPrediction> emptyWindows;` passed to `smoothPredictions`. | Invalid/False flag returned.                                                | Invalid/False returned. | Pass - `EmotionSmoother::smoothPredictions` |
| **Tie between emotions**     | Ensure tie-breaking is deterministic.             | Two windows: one Happy (0.8), one Sad (0.8).                                 | Deterministic winner chosen based on secondary logic or alphabetical order. | Happy                   | Pass - `EmotionSmoother::smoothPredictions` |
| **Repeated emotions**        | Verify scoring weights accumulate correctly.      | Three windows where Happy wins consecutively (0.6, 0.7, 0.8).                | Happy                                                                       | Happy                   | Pass - `EmotionSmoother::smoothPredictions` |
| **Short final audio window** | Verify the 1,600 sample boundary is respected.    | 1,000 sample silence array passed to `analyseWindows`.                       | 0 windows processed.                                                        | 0 windows processed.    | Pass - `EmotionSmoother::analyseWindows`    |

# 8. Challenge and Resolution

**Challenge encountered:** [Your response]

**How I investigated it:** [Your response]

**Solution implemented:** [Your response]

**Evidence that the solution works:** [Your response]

# 9. Generative AI Use Statement

**Option A:** Generative AI was used

**Tool or tools used:** [Your response]

**Purpose of use:** [Your response]

**Parts of the project affected:**

**How I checked the output:**

**Changes I made:**

**How I demonstrated my understanding:**

**Option B:**

Generative AI was not used

**Option B statement:**

I did not use generative AI to produce or revise the submitted code, documentation or analysis
for this assessment.

# 10. Final Student Declaration

    [x] The explanations in this file are written in my own words.
    [x] The code references relate to my submitted project.
    [x] The reported tests were performed on my implementation.
    [x] I can explain and modify the submitted code.
    [x] Any use of generative AI has been declared accurately.

**Student name:** Ronald Kelly **Date:** November 5, 2026