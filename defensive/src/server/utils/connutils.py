from os.path import join
from pathlib import Path
from selectors import EVENT_WRITE, EVENT_READ, DefaultSelector
from socket import socket
from sys import stderr
from types import SimpleNamespace


class Connutils:
    def __init__(self) -> None:
        # Setting up selector and socket
        self.sel = DefaultSelector()
        self.sock = socket()
        self.sock.bind(("localhost", self.__port()))
        self.sock.listen()
        self.sock.setblocking(False)
        self.sel.register(self.sock, EVENT_READ, None)

    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc_value, tb):
        self.sel.close()
        self.sock.close()

        if exc_type is not None:
            print(exc_value)

        return True

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

    @staticmethod
    def display_functionality(conn, mask):
        try:
            conn.send(
                b"""Shalom Mr. User!
1. Register
2. Login
3. Update Public Key
4. Send File
"""
            )
            x = conn.recv(1024)
            print("Recieved:", x)
        except Exception as e:
            print(e, file=stderr)
            conn.close()

    def accept_client(self, sock) -> None:
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

        self.sel.register(
            conn,
            EVENT_READ | EVENT_WRITE,
            data=SimpleNamespace(addr=addr, inb=b"", outb=b""),
        )

    def service_connection(self, key, mask):
        sock = key.fileobj

        data = key.data
        if mask & EVENT_READ:
            recv_data = sock.recv(1024)  # Should be ready to read
            if recv_data:
                data.outb += recv_data
            else:
                print(f"Closing connection to {data.addr}")
                self.sel.unregister(sock)
                sock.close()
        if mask & EVENT_WRITE:
            if data.outb:
                print(f"Echoing {data.outb!r} to {data.addr}")
                sent = sock.send(data.outb)  # Should be ready to write
                data.outb = data.outb[sent:]

    def start_selector(self) -> None:
        """_summary_
        Starts an infinite loop that listens to client connections
        When a client connects to the server, it creates a thread for it
        The thread calls "accept_client()"
        """
        while True:
            events = self.sel.select()
            for key, mask in events:
                if key.data is None:
                    self.accept_client(key.fileobj)
                else:
                    self.service_connection(key, mask)
