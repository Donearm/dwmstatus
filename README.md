# dwmstatus

This project is a fork of the [suckless project][2] by the same name.

## About

I started using dwm not too long ago, and noticed that there was really no kind
of nice, clean status implementation. All of them were in Python or some shell
language. Then I looked at the dwm site and there was suddenly this new
dwmstatus project up there. By default it just showed systemd load and time,
and coming from i3 I was expecting a bit more. So I started editing this with
two motives: learning C and making a good status bar. Now it does a bit more
than show 3 differently formatted timezones and a 5-10-15 minute system load. I
added some patches from the site to make it show network speed, temperature and
a heavily edited battery meter (with time remaining and status) that I threw
together. I also took some stuff from i3status and put it in here, notably the
ip address check.

If this thing is of use to you, then I guess I'm doing something right.

## Instalation and Use

This compiles fine with both clang and gcc for me. Note, it does have most of
the same makedepends as dwm.

Running `make` will compile dwmstatus and you can run it as you would any C
program, `./dwmstatus`. There is documentation on how to run it in the man page.
Just as dwm is configured with a `config.h` file, so is this status bar. There
are a few things you can do just editing this config file, like setting
location, where your battery is located, etc. To edit the order that things are
displayed, you will have to edit dwmstatus.c's `status()` function.

You can also add functions by placing them in the includes folder and adding a
`#include "includes/yourfile.c"` in the `dwmstatus.c` file.

If you're on Arch, then there are packages on the aur, `dwmstatus-ks` and
`dwmstatus-ks-git`, that you can use to build the status bar.

## Bugs

Please add bugs to the [bitbucket project][1] or email me at
1007380[at]gmail[Ðot]com.

## Copyright Info

See LICENSE.

[1]: https://bitbucket.org/KaiSforza/dwmstatus
[2]: http://dwm.suckless.org/dwmstatus
