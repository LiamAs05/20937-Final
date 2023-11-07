import pytest

from defensive.src.server.utils.parser import Parser


def test_parse_bytes():
    msg = b'\x00'*15 + b'\x01' + b'\x03' + bytes(1025) + b'\x00\x00\x00\xFF'
    headers = Parser.parse_headers(msg)
    print(headers)