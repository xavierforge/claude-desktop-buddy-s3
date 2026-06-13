# claude-desktop-buddy (M5StickC Plus **S3** port)

A Claude desk pet that lives on an **M5StickC Plus S3**, reacts to your Claude
sessions, and lets you approve permission prompts from its physical buttons.
Building on the upstream firmware and its S3 port (see the fork note below),
this branch adds:

- **🎵 A music engine.** Five optional, hot-swappable tune slots — looping BGM
  while a task runs, a jingle when it finishes, and feedback sounds on
  approve/deny/alert. Bring your own MIDI or WAV; missing slots fall back to
  beeps. Selectable timbre and volume. See [Music](#music-tune-slots).
- **⌨️ Terminal Claude Code support.** A companion host daemon,
  [buddy-bridge](https://github.com/xavierforge/buddy-bridge), drives the Stick
  from plain `claude` in a terminal — **no desktop app required**. It survives
  device reboots and reconnects on its own. See [Pairing](#pairing).

> **Unofficial fork.** This branch ports the upstream firmware from the
> original M5StickC Plus (ESP32) to the newer **M5StickC Plus S3** (ESP32-S3).
> Upstream explicitly does not accept board-port PRs — see
> [CONTRIBUTING.md](CONTRIBUTING.md). For the protocol reference and the
> original M5StickC Plus support, see [anthropics/claude-desktop-buddy](https://github.com/anthropics/claude-desktop-buddy).
>
> **What changed for the S3:**
> - `platformio.ini` targets `esp32-s3-devkitc-1` with `M5Unified` (not `M5StickCPlus`)
> - New `src/m5_compat.h` shim maps the old `M5.Axp.*`/`M5.Beep.*`/`M5.Imu.*`/`M5.Rtc.*` APIs onto the M5Unified equivalents
> - RTC struct field names switched to lowercase (`.hours`, `.month`, `.weekDay`)
> - Power button handled via `M5.BtnPWR.wasClicked()` instead of `M5.Axp.GetBtnPress()`
> - LED moved from G10 → **G19**
> - `src/data.h` no longer reads from USB `Serial` — on ESP32-S3 with native USB CDC, `Serial.available()` can deadlock `dataPoll` when no host is actively draining. BLE is the only data channel on S3.
>
> **Flashing an S3 for the first time:**
> StickS3 has no GPIO-0 boot button. To enter download mode: plug in USB,
> then **long-press the side power button until the green LED flashes**
> (3-5 seconds). Only then will `pio run -t upload` succeed. After the
> first flash, `ARDUINO_USB_CDC_ON_BOOT=1` usually lets subsequent uploads
> reset into download mode automatically.

### Running on a StickS3

<p align="center">
  <img src="docs/s3/approval.jpg" alt="Approval prompt for a Bash command on M5StickS3" width="240">
  <img src="docs/s3/pet-stats.jpg" alt="Pet stats page showing mood, fed, energy, level and token counters" width="240">
  <img src="docs/s3/credits.jpg" alt="Credits page showing hardware line M5StickC Plus S3 / ESP32-S3 + BMI270" width="240">
</p>

Left: live approval prompt (`Bash` tool waiting on `git config user.name; git config user.email`). Middle: pet stats page after a few approvals. Right: Info → Credits page identifying the hardware as StickS3.

---

Claude for macOS and Windows can connect Claude Cowork and Claude Code to
maker devices over BLE, so developers and makers can build hardware that
displays permission prompts, recent messages, and other interactions. We've
been impressed by the creativity of the maker community around Claude -
providing a lightweight, opt-in API is our way of making it easier to build
fun little hardware devices that integrate with Claude.

> **Building your own device?** You don't need any of the code here. See
> **[REFERENCE.md](REFERENCE.md)** for the wire protocol: Nordic UART
> Service UUIDs, JSON schemas, and the folder push transport.

As an example, we built a desk pet on ESP32 that lives off permission
approvals and interaction with Claude. It sleeps when nothing's happening,
wakes when sessions start, gets visibly impatient when an approval prompt is
waiting, and lets you approve or deny right from the device.

<p align="center">
  <img src="docs/device.jpg" alt="M5StickC Plus running the buddy firmware" width="500">
</p>

## Hardware

The firmware targets ESP32 with the Arduino framework. As written, it
depends on the M5StickCPlus library for its display, IMU, and button
drivers—so you'll need that board, or a fork that swaps those drivers for
your own pin layout.

## Flashing

Install
[PlatformIO Core](https://docs.platformio.org/en/latest/core/installation/),
then:

```bash
pio run -t upload
```

If you're starting from a previously-flashed device, wipe it first:

```bash
pio run -t erase && pio run -t upload
```

Once running, you can also wipe everything from the device itself: **hold A
→ settings → reset → factory reset → tap twice**.

## Pairing

The Stick is a single-central BLE device — exactly one host owns it at a time.
Pick the host that matches how you use Claude:

- **Desktop app** — drives the Stick for the sessions *it* runs (Claude Cowork
  and Code inside the app). Set up below.
- **Terminal `claude`** — to drive the Stick from plain `claude` in a terminal,
  run the companion daemon [buddy-bridge](https://github.com/xavierforge/buddy-bridge)
  instead. It becomes the BLE central in the app's place, so **no desktop app
  is needed**; its README covers install and first-pair. (Already connected via
  the app? "Forget" the Stick there first — only one central at a time.)

### Via the desktop app

To pair your device with Claude, first enable developer mode (**Help →
Troubleshooting → Enable Developer Mode**). Then, open the Hardware Buddy
window in **Developer → Open Hardware Buddy…**, click **Connect**, and pick
your device from the list. macOS will prompt for Bluetooth permission on
first connect; grant it.

<p align="center">
  <img src="docs/menu.png" alt="Developer → Open Hardware Buddy… menu item" width="420">
  <img src="docs/hardware-buddy-window.png" alt="Hardware Buddy window with Connect button and folder drop target" width="420">
</p>

Once paired, the bridge auto-reconnects whenever both sides are awake.

If discovery isn't finding the stick:

- Make sure it's awake (any button press)
- Check the stick's settings menu → bluetooth is on

## Controls

|                         | Normal               | Pet         | Info        | Approval    |
| ----------------------- | -------------------- | ----------- | ----------- | ----------- |
| **A** (front)           | next screen          | next screen | next screen | **approve** |
| **B** (right)           | scroll transcript    | next page   | next page   | **deny**    |
| **Hold A**              | menu                 | menu        | menu        | menu        |
| **Power** (left, short) | toggle screen off    |             |             |             |
| **Power** (left, ~6s)   | hard power off       |             |             |             |
| **Shake**               | dizzy                |             |             | —           |
| **Face-down**           | nap (energy refills) |             |             |             |

The screen auto-powers-off after 30s of no interaction (kept on while an
approval prompt is up). Any button press wakes it.

## ASCII pets

Eighteen pets, each with seven animations (sleep, idle, busy, attention,
celebrate, dizzy, heart). **Settings → ascii pet** cycles them with a counter
shown as `n/18`; the choice persists to NVS.

The counter is 1-based on screen, so the menu number is the table index + 1:

| # | Pet | # | Pet | # | Pet |
|---|-----|---|-----|---|-----|
| 1 | capybara | 7 | octopus | 13 | axolotl |
| 2 | duck     | 8 | owl     | 14 | cactus  |
| 3 | goose    | 9 | penguin | 15 | robot   |
| 4 | blob     | 10 | turtle | 16 | rabbit  |
| 5 | cat      | 11 | snail  | 17 | mushroom |
| 6 | dragon   | 12 | ghost  | 18 | chonk   |

(If a GIF character is installed, the counter's last position is the GIF;
the ASCII species occupy 1–18 ahead of it.)

## GIF pets

If you want a custom GIF character instead of an ASCII buddy, drag a
character pack folder onto the drop target in the Hardware Buddy window. The
app streams it over BLE and the stick switches to GIF mode live. **Settings
→ delete char** reverts to ASCII mode.

A character pack is a folder with `manifest.json` and 96px-wide GIFs:

```json
{
  "name": "bufo",
  "colors": {
    "body": "#6B8E23",
    "bg": "#000000",
    "text": "#FFFFFF",
    "textDim": "#808080",
    "ink": "#000000"
  },
  "states": {
    "sleep": "sleep.gif",
    "idle": ["idle_0.gif", "idle_1.gif", "idle_2.gif"],
    "busy": "busy.gif",
    "attention": "attention.gif",
    "celebrate": "celebrate.gif",
    "dizzy": "dizzy.gif",
    "heart": "heart.gif"
  }
}
```

State values can be a single filename or an array. Arrays rotate: each
loop-end advances to the next GIF, useful for an idle activity carousel so
the home screen doesn't loop one clip forever.

GIFs are 96px wide; height up to ~140px stays on a 135×240 portrait screen.
Crop tight to the character — transparent margins waste screen and shrink
the sprite. `tools/prep_character.py` handles the resize: feed it source
GIFs at any sizes and it produces a 96px-wide set where the character is the
same scale in every state.

The whole folder must fit under 1.8MB —
`gifsicle --lossy=80 -O3 --colors 64` typically cuts 40–60%.

See `characters/bufo/` for a working example.

If you're iterating on a character and would rather skip the BLE round-trip,
`tools/flash_character.py characters/bufo` stages it into `data/` and runs
`pio run -t uploadfs` directly over USB.

## The seven states

| State       | Trigger                     | Feel                        |
| ----------- | --------------------------- | --------------------------- |
| `sleep`     | bridge not connected        | eyes closed, slow breathing |
| `idle`      | connected, nothing urgent   | blinking, looking around    |
| `busy`      | sessions actively running   | sweating, working           |
| `attention` | approval pending            | alert, **LED blinks**       |
| `celebrate` | level up (every 50K tokens) | confetti, bouncing          |
| `dizzy`     | you shook the stick         | spiral eyes, wobbling       |
| `heart`     | approved in under 5s        | floating hearts             |

## Music (tune slots)

The buddy can score your sessions: looping background music while a task
runs, a jingle when one finishes, and feedback sounds on permission prompts.
Sound is **opt-in per slot** — drop in only the tunes you want, leave the
rest out, and the missing ones fall back to the original beeps.

Five slots, each an optional header in `src/tunes/`:

```
src/
├── tune_types.h      shared structs (ToneEvent / Tune)
├── tune_player.h     BGM loop + jingle interrupt + auto-return to BGM
├── tunes.h           the slot registry (which file fills which slot)
├── tunes/            <- drop your own tunes here (gitignored: personal media)
│   ├── bgm.h         background music while tasks run (loops)
│   ├── alert.h       a permission prompt arrived
│   ├── approve.h     you pressed approve
│   ├── deny.h        you pressed deny
│   └── done.h        a task finished (auto-returns to BGM)
└── main.cpp          triggers the slots on task-state edges
```

**Auto-skip when a slot is empty.** `tunes.h` resolves each slot at compile
time with `__has_include("tunes/bgm.h")` (a C++17 preprocessor check): file
present → include it and point the slot at the tune; absent → the slot is
`nullptr`. Every player call is guarded (`if (t && (t->len || t->pcm))`), so
calling a null slot is simply a no-op. A fresh checkout with an empty
`tunes/` compiles and runs fine — slots just stay silent (the beeps remain).

**Two source formats**, both converted to a header by a tool in `tools/`:

| Format | Tool | Good for |
|--------|------|----------|
| Tone events | `midi2tones.py` (voice-aware, 3-channel chiptune) | melodies, BGM |
| 8-bit PCM | `wav2raw.py` (16 kHz mono) | recorded SFX, voices |

```bash
python tools/midi2tones.py song.mid bgm > src/tunes/bgm.h
python tools/midi2tones.py ping.mid alert > src/tunes/alert.h
python tools/wav2raw.py buzz.wav deny > src/tunes/deny.h  # optional gain 0.05-1.0

# fill any subset of bgm/alert/approve/deny/done, then:
pio run -t upload
```

The second argument is the slot name; the converters emit matching symbols
(`TUNE_BGM`, `TUNE_ALERT`, …) so five tunes never collide.

**Playback model.** A jingle (alert/approve/done) interrupts the looping BGM
and hands back to it when it ends. **Settings → wave** toggles a soft
sine/square timbre and **→ volume** is a 5-step level (square runs one tier
quieter to keep the speaker from browning out the rail). **Hold B** skips the
playing jingle — or stops BGM if that's what's playing — without muting the
feedback sounds that follow.

> The desktop app feeds task state automatically. For terminal `claude`,
> [buddy-bridge](https://github.com/xavierforge/buddy-bridge) feeds the same
> state over BLE (see [Pairing](#pairing)) so BGM/done work there too.

## Project layout

```
src/
  main.cpp       — loop, state machine, UI screens
  buddy.cpp      — ASCII species dispatch + render helpers
  buddies/       — one file per species, seven anim functions each
  ble_bridge.cpp — Nordic UART service, line-buffered TX/RX
  character.cpp  — GIF decode + render
  data.h         — wire protocol, JSON parse
  xfer.h         — folder push receiver
  stats.h        — NVS-backed stats, settings, owner, species choice
  tune_*.h       — tune slot registry + player (see Music)
  tunes/         — optional per-slot tune headers (gitignored)
characters/      — example GIF character packs
tools/           — generators and converters
```

## Availability

The BLE API is only available when the desktop apps are in developer mode
(**Help → Troubleshooting → Enable Developer Mode**). It's intended for
makers and developers and isn't an officially supported product feature.
