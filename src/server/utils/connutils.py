from os.path import join
from pathlib import Path
from sys import stderr


class Connutils:
    @staticmethod
    def __get_path():
        return Path(__file__).parent.resolve()

    @staticmethod
    def port():
        full_path = Connutils.__get_path()
        try:
            with open(join(full_path, "port.info"), "r") as f:
                return int(f.read())
        except FileNotFoundError:
            stderr.write(f"WARNING: port.info not found in {full_path}\n")
            return 1357
