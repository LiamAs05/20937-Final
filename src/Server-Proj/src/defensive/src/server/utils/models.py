from datetime import datetime
from os import urandom


class User:
    def __init__(self, name: str, pub: bytes, aes_key: bytes, uid: bytes = None):
        """
        Constructor of a User object

        Args:
            name (str): Username
            pub (bytes): User public key
            aes_key (bytes): User AES key
        """
        self.uid_bytes: bytes = urandom(16) if not uid else uid
        self.uid: str = self.uid_bytes.hex()
        self.name: str = name
        self.pubkey: bytes = pub
        self.last_seen: str = str(datetime.now())
        self.aes_key: bytes = aes_key

    def get_data(self) -> tuple:
        """
        Return the data of a user object as a tuple.
        Serves as a utility function for the add_user function of database.py

        Returns:
            tuple: User data in tuple form
        """
        return (self.uid, self.name, self.pubkey, self.last_seen, self.aes_key)

    def __str__(self) -> str:
        """
        Return object fields as string for readability and displayability
        """
        return rf"""ID: {self.uid}
Name: {self.name}
Last Seen: {str(self.last_seen)}"""


class File:
    def __init__(self, uid: str, filename: str, path: str, verified: bool):
        """
        Constructor of a File object

        Args:
            uid (str): UID of file owner
            filename (str): Name of file
            path (str): Full path of file
            verified (bool): Is file verified
        """
        self.uid: str = uid
        self.filename: str = filename
        self.path: str = path
        self.verified: bool = verified

    def get_data(self) -> tuple:
        """
        Returns a File object in tuple form.
        Similar to user.get_data()

        Returns:
            tuple: File data in tuple form
        """
        return (self.uid, self.filename, self.path, self.verified)

    def __str__(self):
        """
        Return object fields as string for readability and displayability
        """
        return rf"""ID: {self.uid}
Filename: {self.filename}
Path: {self.path}
Verified: {self.verified}"""
