from sqlite3 import connect
from os.path import join
from pathlib import Path
from utils.models import User


class Database:
    def __init__(self):
        self.conn = connect(join(Database.__get_path(), "defensive.db"))
        self.cursor = self.conn.cursor()
        self.create_tables_if_not_exists()
        self.users: dict[int, User] = self.get_all_users()

    def create_tables_if_not_exists(self):
        create_table_query = """
CREATE TABLE IF NOT EXISTS users (
    id INTEGER PRIMARY KEY,
    name TEXT NOT NULL,
    publickey BLOB NOT NULL,
    last_seen TEXT,
    aes_key BLOB
)"""

        # Execute the table creation query
        self.cursor.execute(create_table_query)
        self.cursor.connection.commit()

    def get_all_users(self) -> dict[int, User]:
        users = {}

        select_all_users_query = "SELECT * FROM users"

        # Execute the query to retrieve all users
        self.cursor.execute(select_all_users_query)

        # Fetch all the results
        existing_users = self.cursor.fetchall()

        # Display or process the retrieved users as needed
        for user in existing_users:
            users[user[0]] = User(user[1], user[2], user[4])

        return users

    def add_user(self, user: User):
        insert_query = "INSERT INTO users (id, name, publickey, last_seen, aes_key) VALUES (?, ?, ?, ?, ?)"
        self.cursor.execute(insert_query, user.get_data())
        self.cursor.connection.commit()
        self.users[user.uid] = user

    @staticmethod
    def __get_path():
        return Path(__file__).parent.resolve()
