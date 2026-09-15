# foo_stalker mod

This is part of an old mod for the games _S.T.A.L.K.E.R. Shadow of Chernobyl_, _Clear Sky_, and _Call of Pripyat_. It used to be made up of two parts:
1. A small game mod that read the file fb2k.ltx from the game and displayed player events recorded in it as game notifications.
2. A Foobar 2000 plugin that read and wrote the file from the player.

Alas, the game mod seems to have been lost.

## Operation

The plugin finds the game's window, monitors it for keystrokes, and, when Scroll Lock is on, forwards them to Foobar 2000. At the same time, it writes Foobar 2000 events to the game file `fb2k.ltx`, where they can be seen by the game's scripts.