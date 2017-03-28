# snake
A terminal implementation of snake built in C with [GNU ncurses](https://www.gnu.org/software/ncurses/). This was both my first graphical (sorta) application in C, and also my first game :video_game:. It features multiple levels, with different rates of movement and score increases per food consumed. As far as I can tell, the rules I've implemented are consistent with typical snake games.

There were a few minor things I didn't love and would consider changing:

* I manipulated `halfdelay()` (combined with a timer) to get the behavior I wanted, but hard is still pretty easy. `halfdelay()` only takes integer inputs, so I'd have to come up with a different solution to address this.
* Rows are taller than columns, so vertical movement is way faster. This wouldn't be a huge deal to fix with a different timing system.
* Diamonds might not be the best character for snake body? :gem:
