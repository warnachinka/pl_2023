#!/usr/bin/env python3

import sys
import locale
from app import Application
from logger import Logger

if __name__ == "__main__":
    locale.setlocale(locale.LC_ALL, "ru_RU.UTF-8")
    if len(sys.argv) < 2:
        print("Please provide a filename!\n")
        sys.exit(-1)

    with open('log.txt', 'w') as log:
        Logger.set_stream(log)
        Application().run(sys.argv[1])
