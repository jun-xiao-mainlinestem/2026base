
# Mapping Natural Language Commands to Structured Instructions with Rasa NLU

This document demonstrates how to use `rasa-nlu` to recognize natural language voice commands and extract relevant structured data such as direction, distance, heading, etc.

## Intent: drive_forward

### Example Phrases and Extracted Entities

- drive forward [10](distance) inches with heading [45](heading) degrees
- move ahead [5](distance) inches turning to [90](heading)
- go forward [3](distance) inches facing [180](heading) degrees
- drive [6](distance) inches and set heading to [135](heading)
- forward [12](distance) inches heading [270](heading)

### Notes

- `distance` and `heading` are extracted as entities.
- This approach allows flexible command phrasing while mapping to the same `drive_forward` intent.
- The system can handle variants like:
  - "go forward 5 inches with a heading of 90"
  - "drive 3 inches, heading 180"
  - "move 6 inches and face 135 degrees"

## Usage

These utterances and entity definitions can be included in `nlu.md` for training with `rasa-nlu`.

---

Generated on 2025-07-30
