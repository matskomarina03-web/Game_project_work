# Nonogram / Picross Qt Game

This is a Qt Widgets project for Qt Creator using CMake and a `.ui` form. It implements a three-screen Nonogram game:

- Round Selection Screen with locked and unlocked rounds.
- Game Screen with a puzzle grid, row and column hints, lives, timer, coins, paid hint, verify, check, reset, and exit controls.
- Result Screen with win/loss message, time, mistakes, hints, coins, stars, repeat, menu, and next round buttons.

## Open in Qt Creator

1. Open Qt Creator.
2. Select `File` -> `Open File or Project`.
3. Open `CMakeLists.txt`.
4. Configure a Desktop Qt kit and let Qt Creator run CMake.
5. Press Run.

## File Structure

- `CMakeLists.txt` - Qt Creator / CMake project file.
- `mainwindow.ui` - visual interface layout.
- `mainwindow.h/.cpp` - screen transitions, input, lives, timer, checking, and result logic.
- `round.h/.cpp` - round data and Nonogram hint generation.
- `main.cpp` - application startup.

## Game Rules

- Round 1 is unlocked at launch.
- After completing round N, round N + 1 opens.
- Locked rounds are disabled in the round selection screen.
- Each round starts with 3 lives.
- Pressing `Check` on an incorrect board removes 1 life.
- Pressing `Verify` gives visual feedback without removing lives:
  - green flash = correctly filled cell
  - red flash = wrong filled cell or wrong X mark
- Pressing `Hint` opens one correct cell and costs 10 coins.
- When the puzzle is solved, the final picture is revealed in a special color before the result screen opens.
- Completing a round gives 1-3 stars and coins. Fast completion gives extra coins.
- The daily mission gives +20 coins after completing one round.
- 0 lives means defeat.
- Stars:
  - 3 stars: no mistakes, no hints, fast completion
  - 2 stars: completed without hints, with up to 2 mistakes
  - 1 star: completed with hints or many mistakes
