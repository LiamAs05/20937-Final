from os.path import join
from pathlib import Path
from threading import Thread
from socket import socket
from sys import stderr
from utils.RequestParser import Parser


class Server:
    def __init__(self) -> None:
        self.server_sock = socket()
        self.port = self.__get_port()
        self.server_sock.bind(("localhost", self.port))
        self.threads: list[Thread] = []
        self.parser = Parser()

    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc_value, tb):
        self.server_sock.close()

        if exc_type is not None:
            print(exc_value)

        for t in self.threads:
            t.join()

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
    def __get_port() -> int:
        """
        Reads port from "port.info"
        if "port.info" doesn't exist return 1357
        Returns:
            int: port number to connect to
        """
        full_path = Server.__get_path()
        try:
            with open(join(full_path, "port.info"), "r") as f:
                return int(f.read())
        except FileNotFoundError:
            stderr.write(f"WARNING: port.info not found in {full_path}\n")
            return 1357

    def handle_client(self, conn, addr) -> int:
        print(rf"Started talking to {addr}.")
        try:
            while True:
                res = self.parser.parse_message_content(conn.recv(1024))
                conn.send(res)
        except Exception as e:
            print(e, file=stderr)
        finally:
            conn.close()

    def start(self) -> None:
        """_summary_
        Starts an infinite loop that listens to client connections
        When a client connects to the server, it creates a thread for it
        The thread calls "accept_client()"
        """
        print(f"Listening on port {self.port}")
        while True:
            self.server_sock.listen()
            conn, addr = self.server_sock.accept()
            self.threads.append(
                t := Thread(target=Server.handle_client, args=(self, conn, addr))
            )
            t.start()
