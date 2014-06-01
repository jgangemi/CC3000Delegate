CC3000Delegate
==============

simple delegate around the [AdaFruit CC3000 Library](https://github.com/adafruit/Adafruit_CC3000_Library) to handle setting up a connection to an access point, etc.

**imporant**: until [issue 74](https://github.com/adafruit/Adafruit_CC3000_Library/pull/74) is resolved, you **must** use this fork of the library:

 https://github.com/jacobrosenthal/Adafruit_CC3000_Library
 
which has been patched to support Arduino's `Client` interface. in order to establish a remote connection, use the `connect` method on the client returned from the `getClient()` call.
