# Divergence-Meter
Based on [Tom Titors divergence meter project](http://brotoro.com/dm/index.html), re-written in C.                                                

I wanted to learn embedded C and the divergence meter is a cool project, so I decided to try and reverse engineer Tom Titors assembly code into C to be able to add some features and learn along the way.

Features to add/currently working on.

- [ ] I2C error handling (Error flag in place, just need to decide if I want to just display an error, or work on resending the read/write request. Will see how much memory I have to work with when the rest of the program is finished.
- [X] Tube Display
- [ ] Animations
- [ ] User settings input
- [ ] World line generation
- [ ] Optimization and bug fixes
- [ ] Daylight savings
- [ ] Cross fading (maybe? Would be cool but this is not important)

27-10-23 | 
  Can display digits to all tubes(Probably, still need to insert the rest of my nixie tubes to be certain)
  I2C doesn't appear to be working. Really hard to debug with only one LED on the board. Will connect the rest of my nixie tubes and just use those to display error messages :)

28-10-23 |
  T6 1,2,4,9 digits not displaying correctly, need to check serial to parrallel drivers soldering job (most likey not enough solder.........) rest of numbers in tubes work so probably not a defective tube. I2C wasn't properly initializing... fixed. BIT_CHECK macro used to check if bit was set and then returned its position, which is annoying when I was trying to display the byte for debugging and so I changed it to return just a 0 or 1.

30-10-23 |
  Added clock display and the decimal point switching per second animation. Added I2C error flags, not currently in use. Added RTC error flag, displaying error 666 when RTC is not starting correctly, does not stop other functions from working which is nice.
  T6 still not properly displaying digits even after solder joint touch ups, will swap it to a different position to test, hopefully its not a tube issue.
  Update: turns out the tube leads were not making proper contact and my soldering job for the serial to parallel drivers was a-ok. After fiddling around with the tube all numers in T6 (now T1) are displaying correctly.

31-10-23 |
  RTC error handling appears to be working, tested without backup battery and got the correct error, plugged battery back in, cycled power and RTC started back up. Nice. So error handling works for DS1307, couldn't tell you if it works for DS3232.

Known bugs:
- [ ] Slight tube flickering when clock updates per second. Why? not sure... tried changing how HVE gets enabled and it didn't like it at all. More testing needed.

To test:
- [ ] Error handling on a DS3232

El. Psy. Congroo.
