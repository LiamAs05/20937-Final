from datetime import datetime


class User:
    uid: int = 1

    def __init__(self, name: str, pub: bytes, aes_key: bytes):
        self.id = Database.uid
        Database.uid += 1
        self.name: str = name
        self.pubkey: bytes = pub
        self.last_seen: datetime = datetime.now()
        self.aes_key: bytes = aes_key

    def __str__(self):
        return rf"""ID: {self.id}
Name: {self.name}
Last Seen: {str(self.last_seen)}"""


class File:
    def __init__(self, uid: int, filename: str, path: str, verified: bool):
        self.id: str = uid
        self.filename: str = filename
        self.path: str = path
        self.verified: bool = verified
