from enum import Enum
from database.database import Database, User
from utils.crypto import encrypt_key
from os import urandom


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


class RequestHeaders:
    id: int
    version: int
    code: RequestCodes
    payload_size: int

    @staticmethod
    def __bytes_to_int(b: bytes) -> int:
        return int.from_bytes(b, "little")

    @staticmethod
    def __bytes_to_code(b: bytes) -> RequestCodes:
        return RequestCodes(int.from_bytes(b, "little"))

    def __init__(self, id, version, code, payload_size):
        self.id = self.__bytes_to_int(id)
        self.version = self.__bytes_to_int(version)
        self.code = self.__bytes_to_code(code)
        self.payload_size = self.__bytes_to_int(payload_size)


class ResponseHeaders:
    version: bytes
    code: bytes
    payload_size: bytes

    @staticmethod
    def __code_to_bytes(code: ResponseCodes) -> bytes:
        return code.value.to_bytes(2, "little")

    @staticmethod
    def __int_to_bytes(code: int, len: int) -> bytes:
        return code.to_bytes(len, "little")

    def __init__(self, version: int, code: ResponseCodes, payload_size: int):
        self.version = self.__int_to_bytes(version, 1)
        self.code = self.__code_to_bytes(code)
        self.payload_size = self.__int_to_bytes(payload_size, 4)

    def dump(self) -> bytes:
        return self.version + self.code + self.payload_size


class Parser:
    def __init__(self) -> None:
        self.db = Database()

    @staticmethod
    def parse_headers(msg: bytes) -> RequestHeaders:
        return RequestHeaders(
            msg[:16],
            msg[16:17],
            msg[17:19],
            msg[19:23],
        )

    def do_register(self, name) -> bytes:
        if name in self.db.usernames:
            return ResponseHeaders(3, ResponseCodes.REGISTER_FAIL, 0).dump()
        u = User(name, b"\x01", b"\x01")
        self.db.add_user(u)
        return (
            ResponseHeaders(3, ResponseCodes.REGISTER_SUCCESS, 16).dump() + u.uid_bytes
        )

    def do_login(self, name, uid) -> bytes:
        u: User = self.db.users.get(uid.hex())

        if not u or not len(u.pubkey) == 160:
            return ResponseHeaders(3, ResponseCodes.LOGIN_FAIL, 16).dump() + uid

        key_raw = urandom(16)
        key = encrypt_key(u.pubkey, key_raw)
        self.db.update_aes(uid, key_raw)
        self.db.update_publickey(uid, u.pubkey)

        return (
            ResponseHeaders(3, ResponseCodes.LOGIN_SUCCESS, 16 + len(key)).dump()
            + uid
            + key
        )

    def do_public_key(self, uid, name, pub_key) -> bytes:
        key_raw = urandom(16)
        key = encrypt_key(pub_key, key_raw)
        self.db.update_aes(uid, key_raw)
        self.db.update_publickey(uid, pub_key)

        return (
            ResponseHeaders(3, ResponseCodes.PUBKEY_SUCCESS, 16 + len(key)).dump()
            + uid
            + key
        )

    def parse_message_content(self, msg: bytes) -> bytes:
        headers = Parser.parse_headers(msg)
        uid = msg[:16]
        msg = msg[23:]
        name = msg[:160].decode().rstrip("\x00")
        pubkey = msg[255:]
        match headers.code:
            case RequestCodes.REGISTER:
                return self.do_register(name)
            case RequestCodes.LOGIN:
                return self.do_login(name, uid)
            case RequestCodes.PUBKEY:
                return self.do_public_key(uid, name, pubkey)
            # case RequestCodes.SEND_FILE:
            #     pass  TODO 2103
            case RequestCodes.VALID_CRC | RequestCodes.FINAL_INVALID_CRC:
                return (
                    ResponseHeaders(3, ResponseCodes.MESSAGE_SUCCESS, 16).dump() + uid
                )
            case RequestCodes.INVALID_CRC:
                pass
            case _:
                return ResponseHeaders(3, ResponseCodes.GENERAL_ERROR, 0)
