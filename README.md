# ak2itool
An expanded version of the example originally packaged with powerslaves.
This rewritten version was created with the express purpose of supporting clones
as well as the acecard 2i.

It should be noted that this code is brand new, compared to the old example and
therefore needs testing which it has not recieved yet.

## Usage
```bash
ak2itool dump <file> [length [address] ]
```
Reads the card's flash and dumps it to <file>. If *length* is provided, then the
output wil be truncated to *length* bytes. If *address* is provided, then it
starts reading from that address.

Aliases: `ak2itool read <file> [length [address] ]`

```bash
ak2itool flash <file> [length [address] ]
```
Flashes the contents of <file> to the card's flash. If *length* is provided,
then only *length* bytes will be flashed. If address is provided, then it starts
flashing to that address in flash.

Aliases: `ak2itool write <file> [length [address] ]`

In general, only the first character of a command is checked at the moment, so
a command like `ak2itool f flash.bin` would work. This *is* subject to change,
as soon as I find a good cross-platform argument parser. A length of 0 produces
the default length.

## Supported Cards
 - Acekard 2i revision 44
 - Acekard 2i revision 81
 - R4i Ultra -- This appears to be identical to the ak2i, meaning it'll be caught by the same code.
 - R4i Gold 3DS -- Untested, I heard from someone that the powersaves doesn't like communicating with it?

If you have a cart that you think could be supported, open an issue! Please
provide a way of obtaining the software used to write the firmware, if you do so.

## Hacking
The architecture is a little bit weird. Instantiated instances of the Flashcart
class are automatically added to a vector which is iterated over to find the
card that is currently inserted. It's designed so that you can make as few
changes as possible to get a new card up and running.

You need to provide the following methods:
 - `bool setup()`
 - `void cleanup()`
 - `void formatReadCommand(uint8_t *cmdbuf, uint32_t address)`
 - `void formatEraseCommand(uint8_t *cmdbuf, uint32_t address)`
 - `void formatWriteCommand(uint8_t *cmdbuf, uint32_t address, uint8_t value)`

`setup` preforms basic initialization, such as unlocking the flash, and returns
true if the card present is what it handles. This is the only function in your
class that will be called if it returns false.

`cleanup` cleans up. Does stuff like locking the flash afterwards.

The `format*Command` family of fumctions expect you to take a pointer to a
buffer, as well as some arguments, and write the equivilant command into the
buffer.

Alternatively, you should be able to override the `*Flash` functions if the card
really does act that different from the default. This is untested though, and
it's quite possible I did something wrong to make that difficult. I don't
normally write C++.
