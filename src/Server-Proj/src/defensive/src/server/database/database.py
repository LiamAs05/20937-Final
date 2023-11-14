from os.path import join
from pathlib import Path
from sqlite3 import connect, Error
from enum import Enum
from utils.models import File, User


class Database:
    def __init__(self, in_memory: bool = False):
        """
        Constuctor of a unique Database object

        Args:
            in_memory (bool, optional): For testing purposes create in-memory DB. Defaults to False.
        """
        if not in_memory:
            self.conn = connect(
                join(Database.__get_path(), "defensive.db"), check_same_thread=False
            )
        else:
            self.conn = connect("file::memory:?cache=shared", check_same_thread=False)

        self.cursor = self.conn.cursor()
        self.create_tables_if_not_exists()
        self.users: dict[str, User] = self.get_all_users()
        self.files: dict[str, File] = self.get_all_files()
        self.usernames: list = [x.name for x in list(self.users.values())]

    def create_tables_if_not_exists(self) -> None:
        """
        Creates both users and files tables, unless they exist.
        """
        create_users_table_query = """CREATE TABLE IF NOT EXISTS users (
id TEXT PRIMARY KEY,
name TEXT NOT NULL,
publickey BLOB NOT NULL,
last_seen TEXT,
aes_key BLOB)"""

        create_files_table_query = """CREATE TABLE IF NOT EXISTS files (
id TEXT,
file_name TEXT PRIMARY KEY,
path_name TEXT,
verified BOOLEAN)"""

        self.cursor.execute(create_users_table_query)
        self.cursor.execute(create_files_table_query)
        self.cursor.connection.commit()

    def get_all_users(self) -> dict[int, User]:
        """
        Reads users table from database and returns a dictionary
        Of user ids mapped to user objects

        Returns:
            dict[int, User]: dict of uid -> User
        """

        class UserAttributes(Enum):
            UID = 0
            NAME = 1
            PUBKEY = 2
            AES_KEY = 4

        users = dict()

        select_all_users_query = "SELECT * FROM users"

        # Execute the query to retrieve all users
        self.cursor.execute(select_all_users_query)

        # Fetch all the results
        existing_users = self.cursor.fetchall()

        # process the retrieved users as needed
        for user in existing_users:
            users[user[UserAttributes.UID.value]] = User(
                user[UserAttributes.NAME.value],
                user[UserAttributes.PUBKEY.value],
                user[UserAttributes.AES_KEY.value],
                bytes.fromhex(user[UserAttributes.UID.value])
            )

        return users

    def get_all_files(self) -> dict[int, File]:
        """
        Reads files table from the database and returns a dictionary
        Of file ids mapped to file objects

        Returns:
            dict[int, File]: dict of file_id -> File
        """

        class FileAttributes(Enum):
            ID = 0
            FILE_NAME = 1
            PATH_NAME = 2
            VERIFIED = 3

        files = dict()

        select_all_files_query = "SELECT * FROM files"

        # Execute the query to retrieve all files
        self.cursor.execute(select_all_files_query)

        # Fetch all the results
        existing_files = self.cursor.fetchall()

        # Process the retrieved files as needed
        for file in existing_files:
            files[file[FileAttributes.ID.value]] = File(
                file[FileAttributes.FILE_NAME.value],
                file[FileAttributes.PATH_NAME.value],
                file[FileAttributes.VERIFIED.value],
            )

        return files

    def add_user(self, user: User) -> None:
        """
        Adds a user to the Users table in the database
        And to the users dict in the Object
        Args:
            user (User): User object containing all necessary data
        """
        insert_query = "INSERT INTO users (id, name, publickey, last_seen, aes_key) VALUES (?, ?, ?, ?, ?)"
        self.cursor.execute(insert_query, user.get_data())
        self.cursor.connection.commit()
        self.users[user.uid] = user
        self.usernames.append(user.name)
        print(f"Added user: {user.name} with unique id: {user.uid}")

    def add_file(self, file: File):
        """
        Add a file to the 'files' table.

        Parameters:
        - conn: SQLite connection object
        - id: File ID (TEXT)
        - file_name: File name (TEXT, PRIMARY KEY)
        - path_name: File path (TEXT)
        - verified: Boolean indicating whether the file is verified or not
        """
        try:
            # Insert a new record into the 'files' table
            self.cursor.execute(
                """
                INSERT INTO files (id, file_name, path_name, verified)
                VALUES (?, ?, ?, ?)
            """,
                file.get_data(),
            )

            # Commit the changes to the database
            self.cursor.connection.commit()
            self.files[file.uid] = file
            print(f"File '{file.filename}' added successfully.")

        except Error as e:
            print(f"Error adding file: {e}")

    def update_publickey(self, user_uid, new_publickey):
        """
        Update the public key for a given user.
        :param user_uid: The user ID whose public key needs to be updated.
        :param new_publickey: The new public key value.
        """
        update_query = "UPDATE users SET publickey = ? WHERE id = ?"
        try:
            self.users[user_uid.hex()].pubkey = new_publickey
            self.cursor.execute(update_query, (new_publickey, user_uid.hex()))
            self.cursor.connection.commit()

            print(f"Public key updated successfully for user with ID {user_uid}")
        except Exception as e:
            print(f"Error updating public key: {e}")
            
    def update_aes(self, user_uid, new_aes):
        """
        Update the AES key for a given user.
        :param user_uid: The user ID whose AES key needs to be updated.
        :param new_aes_key: The new AES key value.
        """
        update_query = "UPDATE users SET aes_key = ? WHERE id = ?"

        try:
            self.users[user_uid.hex()].aes_key = new_aes
            self.cursor.execute(update_query, (new_aes, user_uid.hex()))
            self.cursor.connection.commit()

            print(f"AES key updated successfully for user with ID {user_uid}")
        except Exception as e:
            print(f"Error updating AES key: {e}")

    @staticmethod
    def __get_path() -> str:
        """
        Internal function to obtain absolute path of file
        Returns:
            str: path of database.py
        """
        return Path(__file__).parent.resolve()

    def clear(self) -> None:
        """
        Removes all users from the Users table in the database
        Resets UID to 1
        Deletes all data from all structs
        """
        delete_query = "DELETE FROM users"
        self.cursor.execute(delete_query)
        self.cursor.connection.commit()
        self.users = {}
        User.uid = 1
