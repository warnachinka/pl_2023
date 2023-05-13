import resource

from logger import Logger


_log: Logger = Logger('Memory')

@staticmethod
def get_peak_usage() -> int:
    return resource.getrusage(resource.RUSAGE_SELF).ru_maxrss

@staticmethod
def log_peak_usage():
    _log.info('Peak memory usage so far is', get_peak_usage(), 'KiB')
