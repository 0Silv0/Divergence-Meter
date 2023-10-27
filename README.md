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

27-10-23
  Can display digits to all tubes( Probably, still need to insert the rest of my nixie tubes to be certain)
  I2C doesn't appear to be working. Really hard to debug with only one LED on the board. Will connect the rest of my nixie tubes and just use those to display error messages :)

El. Psy. Congroo.
