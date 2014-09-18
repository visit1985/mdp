%title: mdp - Sample Presentation
%author: Michael Göhler
%date: 2014-09-18

mdp
===

A command-line based markdown presentation tool.

-------------------------------------------------

# Supported markdown formatting's

First-level headers can be prefixed by single *#*
or underlined by *===*.

\# first-level

becomes

# first-level

-------------------------------------------------

# Supported markdown formatting's

Second-level headers can be prefixed by *##* or
underlined by *---*.

second-level
\------------

becomes

second-level
------------


-------------------------------------------------

# Supported markdown formatting's

Code blocks are automatically detected by 4
spaces at the beginning of a line.

Tabs are automatically expanded to 4 spaces
while parsing the input.

\    int main(int argc, char \*argv[]) {
\        printf("%s\\n", "Hello world!");
\    }

becomes

    int main(int argc, char *argv[]) {
        printf("%s\n", "Hello world!");
    }

-------------------------------------------------

# Supported markdown formatting's

Quotes are auto-detected by preceding *>*.

Multiple *>* are interpreted as nested quotes.

\> quote
\>> nested quote 1
\> > nested quote 2

becomes

> quote
>> nested quote 1
> > nested quote 2

-------------------------------------------------

# Supported markdown formatting's

Inline highlighting is supported as followed:

- *\** colors text as red
- *\_* underlines text

\_some\_ \*highlighted\* \_\*text\*\_

becomes

_some_ *highlighted* _*text*_

-------------------------------------------------

# Supported markdown formatting's

Backslashes force special markdown characters
like *\**, *\_*, *#* and *>* to be printed as normal
characters.

\\\*special\\\*

becomes

\*special\*

-------------------------------------------------

## More information about markdown

can be found on

_http://daringfireball.net/projects/markdown/_

-------------------------------------------------

# Support for UTF-8 special characters

Here are some examples.

ae = ä, oe = ö, ue = ü, ss = ß
upsilon = Ʊ, phi = ɸ

▛▀▀▀▀▀▀▀▀▀▜
▌rectangle▐
▙▄▄▄▄▄▄▄▄▄▟


-------------------------------------------------

# Suspend your presentation for hands-on examples

Use *Ctrl + z* to suspend the presentation.

Use *fg* to resume it.

-------------------------------------------------

## Last words

I hope you like *mdp*. But be aware, that it is
still in alpha status.

If you observe strange behavior, feel free to
open an issue on GitHub:

_https://github.com/visit1985/mdp_

