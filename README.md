# Divergence-Meter
Based on [Tom Titors divergence meter project](http://brotoro.com/dm/index.html), re-written in C.                                                

I wanted to learn embedded C and the divergence meter is a cool project, so I decided to try and reverse engineer Tom Titors assembly code into C to be able to add some features and learn along the way.

Features to add/currently working on.

- [ ] I2C error handling
- [X] Tube Display
- [ ] Animations
- [ ] User settings input
- [ ] World line generation
- [ ] Optimization and bug fixes
- [ ] Daylight savings
- [ ] Cross fading (maybe? Would be cool but this is not important)

27-10-23 | 
  Can display digits to all tubes( Probably, still need to insert the rest of my nixie tubes to be certain)
  I2C doesn't appear to be working. Really hard to debug with only one LED on the board. Will connect the rest of my nixie tubes and just use those to display error messages :)

28-10-23 |
  T6 1,2,4,9 digits not displaying correctly, need to check serial to parrallel drivers soldering job (most likey not enough solder.........) rest of numbers in tubes work so probably not a defective tube. I2C wasn't properly initializing... fixed. BIT_CHECK macro used to check if bit was set and then returned its position, which is annoying when I was trying to display the byte for debugging and so I changed it to return just a 0 or 1.

El. Psy. Congroo.
