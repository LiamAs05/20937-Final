import pytest

from defensive.src.server.utils.parser import (
    Parser,
    RequestCodes,
    ResponseHeaders,
    ResponseCodes,
)


@pytest.fixture
def test_parser():
    pass


def test_parse_req():
    def code_to_bytes(code: ResponseCodes) -> bytes:
        return code.value.to_bytes(2, "little")

    msg = (
        b"\x01"
        + b"\x00" * 15
        + b"\x03"
        + code_to_bytes(RequestCodes.LOGIN)
        + b"\xFF\x00\x00\x00"
    )
    headers = Parser.parse_headers(msg)
    assert (
        headers.id == 1
        and headers.version == 3
        and headers.code == RequestCodes.LOGIN
        and headers.payload_size == 255
    )


def test_build_res():
    h = ResponseHeaders(3, ResponseCodes.LOGIN_SUCCESS, 255)
    assert (
        h.version == b"\x03"
        and h.payload_size == b"\xFF\x00\x00\x00"
        and h.code == b"\x39\x08"
    )

    assert h.dump() == b"\x03\x39\x08\xFF\x00\x00\x00"
