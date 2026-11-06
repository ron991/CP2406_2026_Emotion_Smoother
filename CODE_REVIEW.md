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

Encapsulates ONNX Runtime interactions and neural network execution. It loads asset files (`mode.onnx`, `labels.txt`),
normalises raw floating-point audio waveforms and returns label-probability pairs in example Happy and Sad.

**Responsibility of EmotionSmoother:**

Encapsulates time-frame segmentation and statistical smoothing algorithms. It divides raw audio vectors into valid
sample windows (1600 samples), generates `WindowPrediction`, applies scoring formula's and computes the final winning
emotion and confidence percentage.

**Code evidence:**

`main.cpp` (`main`, `runAnalysis`), `EmotionModel.h` / `EmotionModel.cpp` (`EmotionModel::predict`),
`EmotionSmoother.h` / `EmotionSmoother.cpp` (`EmotionSmoother::analyseWindows`, `EmotionSmoother::smoothPredictions`)

# 6. Refactoring Summary

**Change 1:**

What changed? Extracted the magic number `1600` into a named `constexpr std::size_t MIN_WINDOW_SAMPLES = 1600` constant
in `EmotionSmoother.h`.  
Why was it useful? Eliminates code duplication, prevents boundary logic drift, and enforces central configuration for
short-window safety checks.

**Change 2:**
What changed? Promoted scoring weight multipliers (`topWeight = 2.0F`, `secondWeight = 1.0F`) to configurable
class/struct constants inside `EmotionSmoother.h`.  
Why was it useful? Decouples algorithm tuning parameters from local loop logic, making future weight adjustments
straightforward without modifying implementation loops.

**Change 3:**
What changed? Refactored score accumulation storage from `std::map<std::string, float>` to
`std::unordered_map<std::string, float>` inside `EmotionSmoother::smoothPredictions`.  
Why was it useful? Improved runtime execution efficiency by replacing $O (\log n)$ tree traversal overhead with $O (1)$
hash table lookups during window tallying.

# 7. Testing Evidence

Document at least four meaningful tests, including an empty window list, a tie, repeated emotions and a short final
audio window.

| Test                         | Purpose                                                                     | Input/setup                                                                                | Expected result                                                                            | Actual result                                           | Pass/fail and code location               |
|:-----------------------------|:----------------------------------------------------------------------------|:-------------------------------------------------------------------------------------------|:-------------------------------------------------------------------------------------------|:--------------------------------------------------------|:------------------------------------------|
| **Empty window list**        | Verify graceful handling when no valid windows are available to process.    | `std::vector<WindowPrediction> emptyWindows;` passed to `smoothPredictions`.               | Returns `SmoothedEmotion` with `valid = false`.                                            | `result.valid == false`                                 | Pass (`EmotionSmoother.cpp`)              |
| **Tie between emotions**     | Ensure tie-breaking logic resolves deterministically.                       | Two windows: W1 Top="Happy" (0.8), 2nd="Sad" (0.2); W2 Top="Sad" (0.8), 2nd="Happy" (0.2). | Accumulated scores equal ($1.8$ vs $1.8$); deterministic tie-break selects winner cleanly. | Winner selected deterministically ("Happy")             | Pass (`EmotionSmoother.cpp`)              |
| **Repeated emotions**        | Verify scoring weight accumulation across multiple consecutive frames.      | Three windows where "Happy" consistently scores as top prediction (0.6, 0.7, 0.8).         | "Happy" accumulates highest overall score and returns high confidence.                     | Label: "Happy", Confidence $> 70\%$                     | Pass (`EmotionSmoother.cpp`)              |
| **Short final audio window** | Verify that audio segments smaller than 1,600 samples are safely discarded. | Audio vector with 1,000 samples ($< 1600$ minimum threshold) passed to `analyseWindows`.   | 0 window predictions generated; no model crash or buffer overrun.                          | `windows.size() == 0`                                   | Pass (`EmotionSmoother.cpp`)              |
| **Invalid Sample Rate**      | Verify exception safety when non-16kHz WAV files are supplied.              | Audio loaded at 44.1 kHz passed to `analyseWindows`.                                       | Throws `std::invalid_argument` with clear error message.                                   | Exception caught; user informed of sample rate mismatch | Pass (`main.cpp` / `EmotionSmoother.cpp`) |

# 8. Challenge and Resolution

**Challenge encountered:** 

Encountered compilation errors(`'tiedWindos' was not declared in this scope`, `'cout' in namespace 'std' does not name a type`, and `expected)`expected declaration before '}' token`) when embedding automated edge-case test blocks in `main.cpp`.

**How I investigated it:** 

Analysed compiler diagnostics highlighting line numbers near the test function.

**Solution implemented:** 

Fixed a missing closing brace and semicolon (`};`) on the `tiedWindows` vector.

**Evidence that the solution works:** 

This project builds and compiles without warnings or errors.

# 9. Generative AI Use Statement

**Option A:** Generative AI was used

**Tool or tools used:** 

Google Gemini and GitHub Copilot

**Purpose of use:** 

Debugging GCC/MinGW compiler syntax errors, Analysing results of output project , Converting and editing files to work with Windows set up (Most files were suited for Linux based on dependencies).

**Parts of the project affected:**

`main.cpp` (menu loop structuring and test harness setup), `EmotionSmoother.cpp` (container efficiency refactoring), `CODE_REVIEW.md` , `emotion_smoother.h` and `emotion_smoother.cpp`.

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