Inputs: 0. start

1. reset
2. previous spot
3. next spot
4. previous setting
5. next setting
6. setting value decrease button
7. setting value increase button

Settings:

```
 - position:         int
 - spotTime:         int (default = 1000)
 - spotActivity:     { On, Off-On-Off, Off-On, On-Off, Off, On-Off-On, Strobe }
 - travelToTime:     int (default = 200)
 - travelToActivity: { Off, On-Off-On, On-Off, Off-On, On, Off-On-Off, Strobe }
```

Controller:

```
___________________________________________________________________
|                                                                 |
|   /\         ___________________________                        |
|  /||\       |                          |           /\           |
|   ||        |                          |           ||           |
|             |                          |       <--    -->       |
|   ||        |                          |           ||           |
|  \||/       |__________________________|           \/           |
|   \/                                                            |
|_________________________________________________________________|
```

arrows on the left: previous spot, next spot
joystick-like arrows on the right: settings selector (vertical arrows), value increase/decrease (horizontal arrows)
display shall be located between the buttons

Controller shall be located on separate board connected by cable for convenience.
