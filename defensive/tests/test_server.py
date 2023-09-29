import pytest
from multiprocessing import Process
from time import sleep
from socket import socket
from defensive.src.server.utils.connutils import Connutils


# Define a fixture to create a Database instance for testing
@pytest.fixture
def test_conn():
    return Connutils()


def test_connection(test_conn):
    p = Process(target=test_conn.start_selector, args=())
    p.start()
    s = socket()
    s.connect(("localhost", 3333))
    text = s.recv(1024)
    s.close()
    p.terminate()
    assert b"Shalom" in text
