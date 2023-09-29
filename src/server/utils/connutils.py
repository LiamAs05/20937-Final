from os.path import join
from pathlib import Path
from selectors import EVENT_READ, DefaultSelector
from socket import socket
from sys import stderr


class Connutils:
    def __init__(self) -> None:
        # Setting up selector and socket
        self.sel = DefaultSelector()
        self.sock = socket()

        # Setting up server and callback funcs
        self.sock.bind(("localhost", self.__port()))
        self.sock.listen()
        self.sock.setblocking(False)
        self.sel.register(self.sock, EVENT_READ, self.accept_client)

    @staticmethod
    def __get_path() -> str:
        """
        Internal function to obtain absolute path of file
        Returns:
            str: path of connutils.py
        """
        return Path(__file__).parent.resolve()

    @staticmethod
    def __port() -> int:
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

    def accept_client(self, sock, mask) -> None:
        """_summary_
        Callback function for selector
        Accepts new client into the server
        Args:
            sock (socket): server socket
            mask (_type_): _description_ TODO
        """
        conn, addr = sock.accept()
        print("accepted", addr)
        conn.setblocking(False)
        self.sel.register(self.sock, EVENT_READ, self.display_functionality)

    def display_functionality(self, conn, mask):
        data = conn.recv(1000)  # Should be ready

        if data:
            print("echoing", repr(data), "to", conn)
            conn.send(data)  # Hope it won't block
        else:
            print("closing", conn)
            self.sel.unregister(conn)
            conn.close()

    def start_selector(self) -> None:
        """_summary_
        Starts an infinite loop that listens to client connections
        When a client connects to the server, it creates a thread for it
        The thread calls "accept_client()"
        """
        while True:
            events = self.sel.select()
            for key, mask in events:
                callback = key.data
                callback(key.fileobj, mask)
