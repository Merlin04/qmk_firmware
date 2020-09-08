# Pangolin

![Pangolin](imgur.com image replace me!)

Pangolin is a 7 key keyboard modeled after the BAT that provides all the functionality of a TKL keyboard through chording. See [this guide](https://www.infogrip.com/docs/pdf/BAT.Guide.Left.pdf) for an overview of the keymap (the code that defines it isn't very easy to read). All of the chords from the BAT keyboard are implemented except for keypad mode, and instead of clearing the keyboard's settings the reset chord resets the controller to be programmed.

* Keyboard Maintainer: [KeyPCB](https://keypcb.xyz)
* Hardware Supported: Pro Micro
* Hardware Availability: None

Make example for this keyboard (after setting up your build environment):

    cd keyboards/handwired/pangolin/keymaps/default
    rm combos.def
    python3 generate_combos.py > combos.def
    cd ../../../../..
    make handwired/pangolin:default

Flashing example for this keyboard:

    make handwired/pangolin:default:flash

See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).
