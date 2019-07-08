
## mdp - A command-line based markdown presentation tool.

![image](https://cloud.githubusercontent.com/assets/2237222/5810237/797c494c-a043-11e4-9dbd-959cab4055fa.gif)

---

***How to get started:***

mdp needs the ncursesw headers to compile.
So make sure you have them installed:

- on Raspbian (Raspberry Pi) you need `libncurses5-dev` and `libncursesw5-dev`

Now download and install mdp:

    $ git clone https://github.com/visit1985/mdp.git
    $ cd mdp
    $ make
    $ make install
    $ mdp sample.md

- On Arch Linux, you can use the existing [package](https://www.archlinux.org/packages/community/x86_64/mdp/).
- on Cygwin you can use the existing [package](https://cygwin.com/cgi-bin2/package-grep.cgi?grep=mdp.exe) from the setup program.
- On Debian, you can use the existing [DEB package](https://tracker.debian.org/pkg/mdp-src), or run `apt-get install mdp`.
- On FreeBSD, you can use the port [misc/mdp](http://www.freshports.org/misc/mdp).
- On OS-X, use the existing [Homebrew Formula](http://brewformulas.org/Mdp) by running `brew install mdp`.
- On Slackware, grab the SlackBuild here: (http://slackbuilds.org/apps/mdp/), or run `sbopkg -i mdp`.
- On Ubuntu, you can use the existing [DEB package](https://launchpad.net/ubuntu/+source/mdp-src), or run `apt-get install mdp`.

Most terminals support 256 colors only if the TERM variable is
set correctly. To enjoy mdp's color fading feature:

    $ export TERM=xterm-256color

---

***How to use it:***

Horizontal rulers are used as slide separator.

Supports basic markdown formating:

- line wide markup
    - headlines
    - code
    - quotes
    - unordered list

- in-line markup
    - bold text
    - underlined text
    - code

Supports headers prefixed by @ symbol.

- first two header lines are displayed as title and author
    in top and bottom bar

Review sample.md for more details.

---

***Controls:***

- h, j, k, l, Arrow keys,
    Space, Enter, Backspace,
    Page Up, Page Down - next/previous slide
- Home, g - go to first slide
- End, G - go to last slide
- 1-9 - go to slide n
- r - reload input file
- q - exit


---

***How to debug it:***

To make a debug version of `mdp`, just type:

    $ make DEBUG=1
