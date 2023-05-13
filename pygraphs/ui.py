from typing import List, Tuple
import curses

from logger import Logger


class UI:

    def __init__(self):
        self.stdscr = curses.initscr()
        curses.cbreak()
        curses.noecho()
        self.stdscr.scrollok(True)

        curses.start_color()

        curses.use_default_colors()

        curses.init_pair(1, -1, curses.COLOR_WHITE)
        curses.init_pair(2, curses.COLOR_YELLOW, -1)
        curses.init_pair(4, curses.COLOR_CYAN, -1)
        curses.init_pair(3, curses.COLOR_GREEN, -1)

        self.stdscr.keypad(True)

    def __del__(self):
        curses.endwin()

    def select(self, options: List[Tuple], exit_value):
        curses.noecho()
        curses.curs_set(0)

        selected_index = 0
        while True:
            self.stdscr.clear()

            i = 0
            for (value, label) in options:
                if selected_index == i:
                    self.stdscr.attron(curses.A_REVERSE)
                
                self.stdscr.addstr(f"{i+1}. {label}\n")

                if selected_index == i:
                    self.stdscr.attroff(curses.A_REVERSE)
                
                i += 1
            
            if (selected_index == len(options)):
                self.stdscr.attron(curses.A_REVERSE)

            self.stdscr.addstr('Выйти\n')

            if (selected_index == len(options)):
                self.stdscr.attroff(curses.A_REVERSE)

            ch = self.stdscr.getch()

            if ch == curses.KEY_UP:
                selected_index -= int(selected_index > 0)
            elif ch == curses.KEY_DOWN:
                selected_index += int(selected_index < len(options))
            elif ch == ord('\n') or ch == curses.KEY_ENTER:
                return exit_value if selected_index == len(options) else options[selected_index][0]

    def read_string(self, message: str) -> str:
        curses.echo()
        self.stdscr.clear()
        curses.curs_set(1)

        self.stdscr.clear()

        self.stdscr.addstr(f"{message}\n")

        return self.stdscr.getstr().decode(errors='ignore')

    def read_number(self, message: str) -> int:
        return int(self.read_string(message))

    def read_list(self, message: str) -> List[str]:
        return filter(None, self.read_string(message).split(' '))

    def print_message(self, message: str):
        curses.noecho()
        curses.curs_set(0)
        self.stdscr.clear()

        self.stdscr.addstr(message)
        self.stdscr.addstr('\n\nPress any key to continue...')

        self.stdscr.getch()