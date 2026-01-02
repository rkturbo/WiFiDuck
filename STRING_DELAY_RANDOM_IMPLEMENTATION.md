# STRING_DELAY_RANDOM Implementation Summary

## Overview
This document describes the implementation of the `STRING_DELAY_RANDOM x y` command for the WiFi Duck ATmega32u4 duckparser.

## Feature Description
The `STRING_DELAY_RANDOM x y` command allows Ducky Scripts to specify a per-character random typing delay between x and y milliseconds (inclusive). This makes typed strings appear more human-like by varying the delay between keystrokes.

## Implementation Details

### Files Modified
1. **atmega_duck/duckparser.cpp**
   - Added three new state variables:
     - `int stringDelayMin` - Minimum delay for random mode
     - `int stringDelayMax` - Maximum delay for random mode
     - `bool useRandomDelay` - Flag to track if random delay mode is active
   
   - Modified `type()` function to:
     - Check `useRandomDelay` flag
     - Use `random(stringDelayMin, stringDelayMax + 1)` when random mode is active
     - Fall back to fixed `stringDelay` when random mode is disabled
   
   - Added command parsing for `STRING_DELAY_RANDOM`:
     - Parses two arguments (min and max)
     - Validates that max >= min (both are unsigned int, so always >= 0)
     - Sets the random delay range and enables random mode
     - Disables fixed delay mode
   
   - Updated `STRING_DELAY` and `DEFAULT_STRING_DELAY` commands to:
     - Disable random delay mode when setting fixed delays

2. **atmega_duck/atmega_duck.ino**
   - Added `randomSeed(analogRead(0))` in setup() for better random number generation

### Command Syntax
```
STRING_DELAY_RANDOM <min> <max>
```
- `min`: Minimum delay in milliseconds (>= 0)
- `max`: Maximum delay in milliseconds (>= min)

### Behavior
- Takes effect immediately when parsed
- Persists until changed by another STRING_DELAY* command
- Mutually exclusive with STRING_DELAY and DEFAULT_STRING_DELAY
- Each character in subsequent STRING commands will have a random delay between min and max ms

### Edge Cases Supported
1. `STRING_DELAY_RANDOM 0 10` - Random delay from 0-10ms (very fast with slight variation)
2. `STRING_DELAY_RANDOM 100 100` - Always 100ms delay (effectively fixed)
3. Switching between random and fixed modes - each command disables the other
4. Invalid arguments (max < min) - silently ignored, keeps previous settings

### Backward Compatibility
- All existing STRING_DELAY and DEFAULT_STRING_DELAY commands work unchanged
- Setting STRING_DELAY or DEFAULT_STRING_DELAY automatically disables random mode
- No changes to existing command behavior

## Test Scripts Created
1. **test_string_delay_random.script** - Basic functionality tests
2. **test_backward_compat.script** - Backward compatibility tests

## Example Usage
```
REM Set random delay between 10-50ms per character
STRING_DELAY_RANDOM 10 50
STRING This text will be typed with random delays

REM Switch to fixed delay
STRING_DELAY 25
STRING This text has fixed 25ms delays

REM Switch back to random
STRING_DELAY_RANDOM 20 80
STRING Random typing again
```

## Technical Notes
- Uses Arduino's built-in `random(min, max)` function which returns [min, max)
- We add 1 to max to make it inclusive: `random(min, max+1)`
- Random seed is initialized once in setup() using noise from analogRead(0)
- The `useRandomDelay` boolean flag ensures 0 is a valid minimum delay value
- Type consistency: Uses unsigned int for delay values matching toInt() return type

## Code Review & Security
- Code review completed successfully
- Type consistency issues addressed
- CodeQL security scan passed (no C++ issues detected)
- No security vulnerabilities introduced
