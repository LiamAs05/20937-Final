from dataclasses import dataclass
from enum import Enum
from typing import Union


class ResponseCodes(Enum):
    REGISTER_SUCCESS = 2100
    REGISTER_FAIL = 2101
    PUBKEY_SUCCESS = 2102
    VALID_CRC = 2103
    MESSAGE_SUCCESS = 2104
    LOGIN_SUCCESS = 2105
    LOGIN_FAIL = 2106
    GENERAL_ERROR = 2107


class RequestCodes(Enum):
    REGISTER = 1025
    PUBKEY = 1026
    LOGIN = 1027
    SEND_FILE = 1028
    VALID_CRC = 1029
    INVALID_CRC = 1030
    FINAL_INVALID_CRC = 1031


@dataclass
class RequestHeaders:
    id: bytes[16]
    version: bytes[1]
    code: RequestCodes
    payload_size: bytes[4]


@dataclass
class ResponseHeaders:
    version: bytes[1]
    code: RequestCodes
    payload_size: bytes[4]


class Parser:
    def __bytes_to_code(b: bytes) -> RequestCodes:
        return RequestCodes(int.from_bytes(b, "little"))

    def __code_to_bytes(
        code: ResponseCodes
        ) -> bytes:
        return code.value.to_bytes(2, "little")

    def __parse_headers(msg: bytes) -> RequestHeaders:
        return RequestHeaders(
            msg[:16],
            msg[16:17],
            Parser.__bytes_to_code([msg[17:19]]),
            msg[19:23],
        )

    def parse_message_content(msg: bytes) -> bytes:
        headers = Parser.__parse_headers(msg)
        msg = msg[:23]

        match headers.code:
            case RequestCodes.REGISTER:
                pass
            case RequestCodes.PUBKEY:
                pass
            case RequestCodes.LOGIN:
                pass
            case RequestCodes.SEND_FILE:
                pass
            case RequestCodes.VALID_CRC:
                pass
            case RequestCodes.INVALID_CRC:
                pass
            case RequestCodes.FINAL_INVALID_CRC:
                pass
            case _:
                pass
