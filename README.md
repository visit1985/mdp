
## mdp - A command-line based markdown presentation tool.

![image](https://cloud.githubusercontent.com/assets/2237222/4280231/d63178fa-3d2a-11e4-88a6-2b8e3608c4ca.gif)

---

*How to get started:*

mdp needs the ncursesw headers to compile.
So make sure you have them installed:
- on Ubuntu/Debian you need `libncursesw5` and `libncursesw5-dev`
- on Cygwin you need `libncursesw10` and `libncurses-devel`

Now download and install mdp:

    $ git clone https://github.com/visit1985/mdp.git
    $ cd mdp
    $ make
    $ make install
    $ mdp sample.md

- On Arch you can use the existing [AUR package](https://aur.archlinux.org/packages/mdp-git/).

---

*How to use it:*

_Horizontal rulers are used as slide separator._

_Supports basic markdown formating:_

- line wide formating
    - headlines
    - code
    - quotes

- in-line formating
    - bold text
    - underlined text

_Supports headers prefixed by @ symbol._

- first two header lines are displayed as title and author
  in top and bottom bar

_Review sample.md for more details._

---

*Controls:*

- h, j, k, l, Cursor keys,
  Space, Enter, Backspace - next/previous slide
- Home - go to first slide
- End - go to last slide
- 1-9 - go to slide n
- q - exit


---

*How to debug it:*

To make a debug version of `mdp`, just type:

    $ make DEBUG=1
