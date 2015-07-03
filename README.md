tibia-capi
===

The goal of tibia-capi is to produce C header files which define the functions addresses and memory
layout of the Tibia client.

These can be used to compile a shared library that calls the native Tibia functions. As a proof of
concept, I have written a library to automatically log into the game on startup. To load it into
Tibia's address space, use `LD_PRELOAD`

```
make
LD_PRELOAD=/path/to/tibia-hook.so ./Tibia
```

It's as simple as that
