Inputs: 0. start

1. reset
2. previous position
3. next position
4. previous setting
5. next setting
6. setting value decrease button
7. setting value increase button
8. setting value sensitivity potentiometer

Settings:

```
 - position:         int
 - spotTime:         int (default = 1000)
 - spotActivity:     { On, Off, Ramp_01, Ramp_10, Bell_010 }
 - travelToTime:     int (default = 200)
 - travelToActivity: { Off, On, Ramp_10, Ramp_01, Bell_101 }
```

Controller:

```
___________________________________________________________________
|                                                                 |
|   /\                                                            |
|  /||\                     __                       /\           |
|   ||                    /    \                     ||           |
|                        |      |                <--    -->       |
|   ||                    \ __ /                     ||           |
|  \||/                                              \/           |
|   \/                                                            |
|_________________________________________________________________|
```

arrows on the left: 2. and 3. switches
circle in the center: sensitivity potentiometer
joystick-like arrows in the right: settings selector (horizontal arrows), value increase/decrease (vertical arrows)
display shall be located above the controllers

Controller shall be located on separate board connected by cable for convenience.
