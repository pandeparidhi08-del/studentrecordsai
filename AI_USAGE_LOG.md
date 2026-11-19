# AI Usage Log

| AI tool | Prompt / question asked | How the response was checked and used |
| --- | --- | --- |
| ChatGPT | “Suggest simple CGPA-based academic advice categories for a student record program, with clear thresholds and messages that can be implemented using C if-else statements. Keep it supportive and do not imply an official academic decision.” | Used the idea of four bands: Excellent, Good, Needs Improvement, and Requires Immediate Attention. I reviewed the wording and selected explicit thresholds (9.0, 7.5, and 6.0) before implementing the decision in `academicAdvisor()` using ordinary C conditionals. The program itself calculates CGPA and generates the final recommendation; it does not call an AI service. |
| ChatGPT | “Suggest test cases for averaging available semester SGPA values and for CGPA-based advice.” | Checked the examples against the arithmetic mean of the non-zero available SGPA slots and included three distinct advice cases in the sample output. |

The AI suggestions were treated as a starting point. Thresholds and messages are configurable educational examples and should be reviewed against course expectations. I should be able to explain the CGPA calculation, branch order, and each message during the viva.
