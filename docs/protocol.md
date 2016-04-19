# ESP side Protocol documentation

General notes:

  - first letter of command is the command itself, everything after are params of the command
  - PIXEL_ID is actual ID of pixel on the strip, numbered from 1 to max (120 currently)
  - R, G, B are color values, in range of 0 to 255
  - except for level command, you have to commit all the changes afterwards, with `c 1` comand

Supported commands

  - `s PIXEL_ID R G B` :=> set color of single pixel
  - `r R G B PIXEL_ID(start) PIXEL_ID(end)` :=> set color of range of pixels
    - PIXEL_ID (start) or (end) is in range 1 to 120 (or number of LEDs currently available)
  - `l LEVEL R G B` :=> set audio level visual
    - `LEVEL` in range 1 to 120 (or number of LEDs currently available)
  - `c 0` :=> clear all LEDs
  - `c 1` :=> commit changes to the LEDs
