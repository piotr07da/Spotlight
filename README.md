# Spotlight
Spotlight is a project of a device used to highlight targets on a shooting range. This is achieved by using a beam of light that cycles between different targets. The light can be turned on and off, ramped up or down, or turned to strobe mode.

# Projects
There are three projects in this repository.

## Spotlight
The Spotlight project is a C++ project that consists main software used in the device.

## SpotlightMainCircuit
The SpotlightMainCircuit is a KiCad project of the device's circuit board.

## SpotlightDesktopClient
The SpotlightDesktopClient project is an Electron project written in TypeScript. It is used to connect with the device for diagnostic purposes, mainly for debugging the sound trigger feature of the device.

# Hardware
The Spotlight is build of:
- Main unit. This contains a Particle Photon device, stepper motor controler, voltage regulators, operation amplifiers, and a microphone.
- Controller with buttons and display. This allows the user to configure a sequence of targets to highlight.
- Light unit. This consists of a power LED mounted on a shaft of a stepper motor. It can be pointed at any direction.

# How to use it

## Configuration

### Controller
To configure the device we use controller unit.
```
 _____________________________________________
|                                             |
|         __________________                  |
|   /\   |                  |       /\        |
|   ||   |                  |       ||        |
|        |                  |   <==    ==>    |
|   ||   |                  |       ||        |
|   \/   |__________________|       \/        |
|                                             |
|_____________________________________________|
```

There is a display in the middle of the controller.

There are two buttons on the left:
- UP - if applicable it directly jumps to the previous spot (target), staying in the same setting (for example direction of light)
- DOWN - if applicable it directly jumps to the next spot (target), staying in the same setting

There are four buttons on the right:
- UP - used to select the previous position in the menu
- DOWN - used to select the next position in the menu
- LEFT - exists from the current menu option
- RIGHT - enters the selected menu position

### How to configure
First we need to configure a number of spots (shooting targets). We do that by going to the Global Options (select it using UP/DOWN arrows on the right). After selecting Global Options enter it (using RIGHT arrow on the right).
After that we can exit to the main menu using LEFT arrow on the right.
No we can select Spots menu option and enter it. In this menu we can switch between different spots using arrows on the left.
To configure a spot we need to enter one of the available settings:
- Direction - this setting allows to use UP and DOWN arrows to physically rotate the light. To make it easy the light will be turned on automatically to allow to direct it precisly at the target
- Spot Time - this setting tells for how many milliseconds the light will be directed at the target
- Spot Activity - this setting tells how the light will behave. There are different modes:
  - turned on
  - turned off
  - ramping from off to on
  - ramping from on to off
  - ramping from off through on and again to off
  - ramping from on through off and again to on
  - strobe
- Travel Time - this setting tells how much time it will take for the light to turn toward desingated direction
- Travel Activity - this setting tells how light will behave during the travel (the same light behaviors as in Spot Activity option)




