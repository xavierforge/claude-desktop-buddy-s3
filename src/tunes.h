// tunes.h — tune slot registry. Lives in src/.
//
// Drop your converted .h files into src/tunes/; the filename picks the role:
//
//   tunes/bgm.h      background music while tasks are running (loops)
//   tunes/alert.h    played when a permission prompt arrives
//   tunes/approve.h  feedback for pressing approve
//   tunes/deny.h     feedback for pressing deny
//   tunes/done.h     played when a task completes (auto-returns to BGM)
//
// Conversion: python midi2tones.py yourfile.mid bgm > src/tunes/bgm.h
//
// A missing file leaves its slot nullptr; playback calls skip it
// automatically (the original beep still sounds).

#pragma once
#include "tune_player.h"

// __has_include("path") is a compile-time check: 1 if the file exists,
// 0 otherwise. A missing file is not a compile error — the slot just
// becomes a null pointer.

#if __has_include("tunes/bgm.h")
  #include "tunes/bgm.h"
  [[maybe_unused]] static const Tune* SLOT_BGM = &TUNE_BGM;
#else
  [[maybe_unused]] static const Tune* SLOT_BGM = nullptr;
#endif

#if __has_include("tunes/alert.h")
  #include "tunes/alert.h"
  [[maybe_unused]] static const Tune* SLOT_ALERT = &TUNE_ALERT;
#else
  [[maybe_unused]] static const Tune* SLOT_ALERT = nullptr;
#endif

#if __has_include("tunes/approve.h")
  #include "tunes/approve.h"
  [[maybe_unused]] static const Tune* SLOT_APPROVE = &TUNE_APPROVE;
#else
  [[maybe_unused]] static const Tune* SLOT_APPROVE = nullptr;
#endif

#if __has_include("tunes/deny.h")
  #include "tunes/deny.h"
  [[maybe_unused]] static const Tune* SLOT_DENY = &TUNE_DENY;
#else
  [[maybe_unused]] static const Tune* SLOT_DENY = nullptr;
#endif

#if __has_include("tunes/done.h")
  #include "tunes/done.h"
  [[maybe_unused]] static const Tune* SLOT_DONE = &TUNE_DONE;
#else
  [[maybe_unused]] static const Tune* SLOT_DONE = nullptr;
#endif
