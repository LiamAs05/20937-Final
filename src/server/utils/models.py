from datetime import datetime


class User:
    uid: int = 1

    def __init__(self, name: str, pub: bytes, aes_key: bytes):
        self.uid = User.uid
        User.uid += 1
        self.name: str = name
        self.pubkey: bytes = pub
        self.last_seen: str = str(datetime.now())
        self.aes_key: bytes = aes_key

    def get_data(self):
        return (self.uid, self.name, self.pubkey, self.last_seen, self.aes_key)

    def __str__(self):
        return rf"""ID: {self.uid}
Name: {self.name}
Last Seen: {str(self.last_seen)}"""


class File:
    def __init__(self, uid: int, filename: str, path: str, verified: bool):
        self.uid: int = uid
        self.filename: str = filename
        self.path: str = path
        self.verified: bool = verified
