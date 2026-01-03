# REPEAT Two-Argument Implementation Summary

## Overview
This document describes the implementation of the two-argument `REPEAT <times> <lines>` command for the WiFi Duck ESP-side duckscript interpreter.

## Feature Description
The `REPEAT` command now supports an optional second argument to repeat the last N lines M times, handled entirely on the ESP side. This is backwards compatible with the existing single-argument `REPEAT <times>` command which continues to work as before (ATmega-driven).

### Command Forms
1. **Single-argument (existing behavior)**: `REPEAT <times>` - ATmega repeats the last single line
2. **Two-argument (new behavior)**: `REPEAT <times> <lines>` - ESP resends the last N lines M times

## Implementation Details

### Files Modified
1. **esp_duck/duckscript.cpp**
   - Added line history circular buffer:
     - `LineHistory lineHistory[MAX_HISTORY_LINES]` - Stores up to 256 lines
     - `historyCount` - Number of lines currently in history
     - `historyIndex` - Current position in circular buffer
   
   - Added ESP-side repeat state variables:
     - `inEspRepeat` - Flag indicating we're in ESP-side repeat mode
     - `espRepeatTimes` - Total number of times to repeat
     - `espRepeatLines` - Number of lines to repeat
     - `espRepeatCurrentTime` - Current repetition number
     - `espRepeatCurrentLine` - Current line within repetition
   
   - Added helper functions:
     - `addToHistory()` - Adds a line to the circular history buffer
     - `parseRepeatCommand()` - Parses REPEAT command and returns type (0/1/2 args)
     - `sendNextEspRepeatLine()` - State machine to send next line in repeat sequence
   
   - Modified `nextLine()` function to:
     - Check if in ESP-side repeat mode first
     - Parse REPEAT commands to detect 1 vs 2 arguments
     - Handle two-arg REPEAT on ESP side (don't send to ATmega)
     - Store all non-REPEAT commands in line history
     - Continue sending single-arg REPEAT to ATmega for backward compatibility
   
   - Modified `stopAll()` function to:
     - Reset ESP-side repeat state
     - Free all lines in history buffer

### Command Syntax
```
REPEAT <times> [<lines>]
```
- `<times>`: Number of times to repeat (required, must be > 0)
- `<lines>`: Number of previous lines to repeat (optional, must be > 0 and <= history size)

### Behavior
#### Single-Argument (Backward Compatible)
```
STRING Hello
REPEAT 3
```
- Sends `STRING Hello` to ATmega
- Sends `REPEAT 3` to ATmega
- ATmega repeats the last line 3 times (total 4 executions)

#### Two-Argument (New ESP-side)
```
STRING Line 1
STRING Line 2
REPEAT 3 2
```
- Sends `STRING Line 1` to ATmega
- Sends `STRING Line 2` to ATmega
- ESP intercepts `REPEAT 3 2`
- ESP resends the last 2 lines (Line 1 and Line 2) 3 times
- REPEAT command itself is NOT sent to ATmega

### State Machine for Asynchronous Execution
The ESP-side repeat uses a state machine to work with the asynchronous communication:
1. When `REPEAT <times> <lines>` is encountered:
   - Set `inEspRepeat = true`
   - Store times and lines parameters
   - Send first line immediately
2. When ATmega finishes processing a line (callback):
   - `nextLine()` is called
   - Checks `inEspRepeat` flag
   - If true, calls `sendNextEspRepeatLine()` instead of reading from file
3. `sendNextEspRepeatLine()` sends next line in sequence
4. When all repetitions complete:
   - Reset state
   - Continue reading from file

### Line History Buffer
- Circular buffer with 256 slots (MAX_HISTORY_LINES)
- Stores every non-REPEAT command
- Oldest lines are overwritten when buffer is full
- Each entry stores the raw command string and its length
- Memory is managed with malloc/free

### Usage Examples

#### Example 1: Basic Two-Arg REPEAT
```
STRING First line
STRING Second line
REPEAT 2 2
```
Output:
```
First line
Second line
First line
Second line
First line
Second line
```

#### Example 2: Mixed with Single-Arg REPEAT
```
STRING Line A
REPEAT 1         REM Single-arg: ATmega repeats last line 1 time
STRING Line B
STRING Line C
REPEAT 2 2       REM Two-arg: ESP repeats last 2 lines 2 times
```
Output:
```
Line A
Line A
Line B
Line C
Line B
Line C
Line B
Line C
```

#### Example 3: Repeat with Commands
```
STRING Text
ENTER
DELAY 500
REPEAT 3 3       REM Repeat the STRING, ENTER, and DELAY
```
This repeats all three commands (STRING, ENTER, DELAY) 3 times.

#### Example 4: Edge Case - Repeat 1 line 1 time
```
STRING Only once
REPEAT 1 1
```
Outputs "Only once" twice total (original + 1 repeat).

### Edge Cases Supported
1. `REPEAT 0 2` - Invalid (times must be > 0), skips command
2. `REPEAT 3 0` - Invalid (lines must be > 0), skips command
3. `REPEAT 3 300` - Invalid (lines > history), skips command
4. `REPEAT 1 1` - Valid, repeats last 1 line 1 time
5. Mixing single-arg and two-arg REPEAT in same script
6. Two-arg REPEAT at start of script with no history - skips command

### Backward Compatibility
- Single-argument `REPEAT <times>` continues to work exactly as before
- Sent to ATmega for processing
- Uses existing `repeatNum` variable on ATmega side
- No changes to ATmega code required
- All existing scripts remain fully compatible

### Memory Management
- Line history uses dynamic allocation (malloc/free)
- Each line allocates len+1 bytes for null terminator
- Old lines are freed before overwriting in circular buffer
- All history is freed when script stops
- `prevMessage` is maintained separately for single-arg REPEAT compatibility

### Performance Considerations
- Parsing overhead is minimal (simple string comparison and integer parsing)
- No impact on single-arg REPEAT performance
- Two-arg REPEAT has overhead of copying lines from history
- Circular buffer prevents memory growth beyond 256 lines

## Test Scripts Created
1. **test_repeat_two_arg.script** - Comprehensive tests for two-argument REPEAT

## Technical Notes
- Uses asynchronous state machine to integrate with existing callback system
- Each line waits for ATmega completion before sending next
- ESP-side REPEAT commands are never sent to ATmega (intercepted on ESP)
- Line history includes newlines/EOL characters from original commands
- Compatible with all existing duckscript commands (STRING, DELAY, etc.)

## Comparison: Single-Arg vs Two-Arg REPEAT

| Aspect | REPEAT <times> | REPEAT <times> <lines> |
|--------|----------------|------------------------|
| Processing location | ATmega | ESP |
| Lines repeated | Last 1 line | Last N lines |
| Sent to ATmega | Yes | No |
| Uses line history | No | Yes |
| Backward compatible | N/A | Yes |
| Max lines | 1 | 256 |

## Code Review & Security
- Code review pending
- Memory safety: All allocations are checked and properly freed
- Buffer overflow protection: Circular buffer with fixed size
- Integer overflow: Not a concern with reasonable script sizes
- CodeQL security scan: Pending
