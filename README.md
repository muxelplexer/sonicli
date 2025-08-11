# sonicli - TUI subsonic client

I was looking for a TUI based SubSonic client but i was unable to find much. Most
of the nice clients seem to be Go based and graphical. This is why i decied to
create *sonicli*. This will also allow me to seperate the core library into a C
linkable library as well at some point.

## Requirements

You will need to have a fairly recent compiler and CMake version due to the usage
of new and fancy modules. Otherwise all dependencies are vendored internally from
the `ext/` directory.
