# Wire Loop Game (Arduino)

A simple, family-friendly **wire loop** skill game built with Arduino. It uses an I2C 16×2 LCD for on-screen prompts and a piezo speaker for win/lose jingles. RGB button LEDs indicate state, while two result LEDs show win/lose. The game measures how long the player takes between the start signal and either **win** (reaching the goal) or **lose** (touching the loop).

> Tested on Arduino Uno-style boards. Adjust pin numbers if you use a different board.

---

## 📊 Status & Info

![Last Commit](https://img.shields.io/github/last-commit/BlindTrevor/BuzzOff)
![Issues](https://img.shields.io/github/issues/BlindTrevor/BuzzOff)
![Repo Size](https://img.shields.io/github/repo-size/BlindTrevor/BuzzOff)

---

## Features

- Start countdown (3…2…1… GO!) with tones and LED cues
- Live time display while the game is running
- **Win** and **Lose** detection with dedicated LED indicators
- Short win/lose melodies on a piezo speaker
- Clean separation of logic and display; easy to customize

---

## Bill of Materials

- Arduino Uno (or compatible)
- 16×2 I2C LCD (commonly address **0x27**)
- Piezo speaker/buzzer
- Momentary push button (start)
- RGB LED for the start button (common cathode assumed)
- 2 × single-color LEDs for result indicators (Win/Fail)
- Wire loop + probe (electrically connected to sense touches)
- Resistors appropriate for your LEDs (e.g., 220–330 Ω)
- Jumper wires and breadboard or perfboard

---

## Pin Assignments

| Signal            | Arduino Pin |
|-------------------|-------------|
| Wire Loop Sense   | D2          |
| Win Target Sense  | D3          |
| Start Button      | D4          |
| Button LED – Red  | D5          |
| Button LED – Green| D6          |
| Button LED – Blue | D7          |
| Speaker           | D8          |
| Button LED – Common (C) | D9   |
| Result LED – A (Win)    | D10  |
| Result LED – C (Lose)   | D11  |
| I2C LCD SDA       | A4 (Uno)   |
| I2C LCD SCL       | A5 (Uno)   |

> **Inputs (D2/D3/D4)** use `INPUT_PULLUP`. Wire your loop/probe, win target, and button so that an **active** event pulls the input **LOW**.

---

## Wiring Notes

- **LCD**: Connect `VCC` to 5V, `GND` to GND, `SDA` to A4, `SCL` to A5 (on Uno). Many 1602 I2C backpacks use address `0x27`; change if yours is different.
- **Button**: One side to GND, the other to D4. `INPUT_PULLUP` removes the need for an external pull-up resistor.
- **Wire loop & probe**: Arrange so that a touch creates a path to GND, pulling **D2 LOW**.
- **Win target**: Similar to the loop, touching the finish pad should pull **D3 LOW**.
- **LEDs**: Use resistors. The RGB button LED is treated as **R/G/B anodes** with a **common cathode** on `D9` (or vice versa if your LED is common anode—see customization).
- **Speaker**: Piezo from `D8` to GND (series resistor ~100 Ω optional).

---

## Software Requirements

- **Arduino IDE** (or Arduino CLI)
- **Libraries**:
  - `LiquidCrystal_I2C` – for 16×2 I2C LCD
  - `pitches.h` – Arduino example header defining musical note constants used with `tone()`

### Installing `LiquidCrystal_I2C`

1. Open Arduino IDE → **Sketch › Include Library › Manage Libraries…**
2. Search for **LiquidCrystal_I2C**
3. Install a commonly used version (e.g., by Marco Schwartz or Frank de Brabander). If your LCD does not display correctly, try an alternative library or adjust the address.

### Getting `pitches.h`

`pitches.h` is the header used in Arduino **tone** examples. If you don't already have one, you can use the included template in this repo or copy from the Arduino example (`File › Examples › 02.Digital › toneMelody`).

---

## How to Build & Upload

1. Download this repository (ZIP provided below) and extract it.
2. Open `src/WireLoopGame.ino` in Arduino IDE.
3. Connect your board, select the right **Board** and **Port**.
4. Click **Upload**.

If your LCD shows gibberish or stays blank, verify the I2C address (`0x27` vs `0x3F`) and contrast. Update `LiquidCrystal_I2C lcd(0x27, 16, 2);` accordingly.

---

## Gameplay

1. The display shows **“Next Player!”** while idle.
2. Press the **Start** button. You’ll see **“Get Ready!”** and a **3–2–1** countdown with beeps.
3. At **GO**, the timer starts. Keep the probe from touching the loop!
4. **Win**: touch the **finish target** → green indication, win melody, final time shown.
5. **Lose**: touch the **loop** → red indication, lose melody, final time shown.

---

## Customization

- **I2C Address**: Change `0x27` to your LCD’s address.
- **LED Polarity**: If you have a **common anode** RGB LED, invert LED writes (HIGH/LOW) or wire the common to **5V** and drive R/G/B **LOW** to turn them on.
- **Melodies**: Edit the `melody[]` and `noteDurations[]` arrays in `winMusic()` and `loseMusic()`.
- **Update Rate**: LCD refresh is throttled to ~8 Hz (every 125 ms). Adjust `LcdUpdateMillis` logic if needed.

---


## License

MIT License. Do what you like—just include attribution.
