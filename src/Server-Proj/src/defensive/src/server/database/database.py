from os.path import join
from pathlib import Path
from sqlite3 import connect

from utils.models import File, User


class Database:
    def __init__(self, in_memory: bool = False):
        """
        Constuctor of a unique Database object

        Args:
            in_memory (bool, optional): For testing purposes create in-memory DB. Defaults to False.
        """
        if not in_memory:
            self.conn = connect(join(Database.__get_path(), "defensive.db"), check_same_thread=False)
        else:
            self.conn = connect("file::memory:?cache=shared", check_same_thread=False)

        self.cursor = self.conn.cursor()
        self.create_tables_if_not_exists()
        self.users: dict[str, User] = self.get_all_users()
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
id TEXT PRIMARY KEY,
file_name TEXT UNIQUE,
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
            users[user[UID]] = User(user[NAME], user[PUBKEY], user[AES_KEY])

        return users

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
        
    def update_publickey(self, user_uid, new_publickey):
        """
        Update the public key for a given user.
        :param user_uid: The user ID whose public key needs to be updated.
        :param new_publickey: The new public key value.
        """
        update_query = "UPDATE users SET publickey = ? WHERE id = ?"

        try:
            self.cursor.execute(update_query, (new_publickey, user_uid))
            self.cursor.connection.commit()
            self.users[user_uid].publickey = new_publickey
            print(f"Public key updated successfully for user with ID {user_uid}")
        except Exception as e:
            print(f"Error updating public key: {e}")

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
