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

**Container used:**
[std::map / std::unordered_map]
**Reason for my choice:**
[Explain the key, value and why this container suits model-loaded labels.]
**Code evidence:**
File: ___ Function: ___ Relevant line or fragment: ___

# 5. Separation of Responsibilities
**Question:** Why should EmotionSmoother be separate from EmotionModel and main.cpp?
   
**Responsibility of main.cpp:** [Your response]

**Responsibility of EmotionModel:** [Your response]

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
Document at least four meaningful tests, including an empty window list, a tie, repeated emotions and a short final audio window.

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

**Student name:** Ronald Kelly
**Date:** November 5, 2026