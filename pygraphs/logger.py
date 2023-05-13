import sys
from typing import IO

class Logger:
    def __init__(self, name):
        self._name = name

    def info(self, *loggedContent):
        self._log(sys.stdout, *loggedContent)

    def warning(self, *loggedContent):
        self._log(sys.stdout, *loggedContent)

    def severe(self, *loggedContent):
        self._log(sys.stderr, *loggedContent)

    @staticmethod
    def set_stream(stream: IO):
        Logger._stream: IO = stream

    def _log(self, stream: IO, *loggedContent):
        out = stream if Logger._stream is None else Logger._stream
        print(f"[{self._name}]:", *loggedContent, file=out)

Logger._stream: IO = None
