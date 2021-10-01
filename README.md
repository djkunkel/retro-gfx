# retro-gfx
A tiny, simple VESA graphics library for DOS

# Purpose
1. This is a purely educational exercise for me and my friends to learn more about low level programming and the systems we grew up using as kids.

# Building

1. Use Smaller C for Windows with NASM.
2. See build.sh in each example for how to build.
3. NASM (https://www.nasm.us/) and Smaller C (https://github.com/alexfru/SmallerC) must be in your path.
4. I build with the Windows x64 builds of NASM and Smaller C

# Where does it run?
1. DOSBox-X
2. A couple of real DOS machines that have VESA compliant video adapters and support the linear framebuffer and mode 0x101 (640x480x8) for now.
3. It runs in UNREAL mode (https://en.wikipedia.org/wiki/Unreal_mode) so EMM386 or Windows must not be loaded in order for it to work.


# Acknowledgements

Thanks to Alexey Frunze for creating Smaller C which has allowed this experimental effort to exist. 
Many of the routines were used directly and some modified from his example code that is included with https://github.com/alexfru/SmallerC
