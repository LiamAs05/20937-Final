from os.path import join
from pathlib import Path
from sys import stderr


class Connutils:
    @staticmethod
    def __get_path() -> str:
        """
        Internal function to obtain absolute path of file
        Returns:
            str: path of connutils.py
        """
        return Path(__file__).parent.resolve()

    @staticmethod
    def port() -> int:
        """
        Reads port from "port.info"
        if "port.info" doesn't exist return 1357
        Returns:
            int: port number to connect to
        """
        full_path = Connutils.__get_path()
        try:
            with open(join(full_path, "port.info"), "r") as f:
                return int(f.read())
        except FileNotFoundError:
            stderr.write(f"WARNING: port.info not found in {full_path}\n")
            return 1357
