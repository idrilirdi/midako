# 魅蛸 (Midako)

**魅蛸** (みだこ / *Midako*) is a personal QMK userspace and keymap for
**BastardKB**'s **Charybdis** and **Dilemma** keyboards.

It draws inspiration from:

- **Charyoku** by ankostis  
  <https://github.com/ankostis/qmk_firmware/tree/charyoku/keyboards/bastardkb/charybdis/4x6/keymaps/charyoku>
- **Miryoku** by manna-harbour  
  <https://github.com/manna-harbour/miryoku>

This layout adapts Miryoku’s layer-driven philosophy to the keyboards I use, with Charyoku-style refinements and personal tweaks.

---

## Design Goals

- Very close to Miryoku in the inner keys
- First row and external columns for easy access to useful stuff
- Home-row mods
- Symbols rework with a focus on C++ development

---

## Supported Keyboards

I have to 4x6 variants of
- **Charybdis** <https://github.com/Bastardkb/Charybdis>
- **Dilemma** <https://github.com/Bastardkb/Dilemma> (v3)

---

## Building

### BastardKB QMK fork

Clone the BKB QMK repository, using either github desktop or the command line, and switch to the bkb-master branch:

```bash
git clone https://github.com/bastardkb/bastardkb-qmk
cd bastardkb-qmk
git checkout -b bkb-master origin/bkb-master
qmk git-submodule

#Set this as the local QMK repository
qmk config user.qmk_home="$(realpath .)"
```

### Userspace
Go back to your copy of this repo and activate userspace:
```bash
qmk config user.overlay_dir="$(realpath .)"
```

To compile any of the keymaps just do:
```bash
# Charybdis
qmk compile -kb bastardkb/charybdis/4x6 -km midako

# Dilemma
qmk compile -kb bastardkb/dilemma -km midako
```

### Flashing
Once you have compiled a keymap you can flash it by doing the following:

1) Put the keyboard in bootloader mode
2) Connect to PC using the usb port
3) Flash:
    * You can simply copy over the `.uf2` file
    * Alternatively you can change `compile` to `flash` in the above commands.

It is a good idea to repeat for the other half of the keyboard, although not always strictly needed.

# Acknowledgements

Thanks to:

    @manna-harbour for Miryoku

    @ankostis for Charyoku

    @bstiq for the awesome keyboard designs

