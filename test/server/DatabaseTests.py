import pytest

from server.database.database import Database
from server.utils.models import User

# Define a fixture to create a Database instance for testing
@pytest.fixture
def test_database():
    return Database(db_name="test.db")

def test_add_user(test_database):
    # Create a test user
    test_user = User(name="TestUser", publickey=b"testkey", aes_key=b"aeskey")
    # Add the user to the database
    test_database.add_user(test_user)
    # Check if the user exists in the database
    assert test_user.uid in test_database.users

def test_get_all_users(test_database):
    # Retrieve all users from the database
    users = test_database.get_all_users()
    # You can add assertions here to check if the retrieved users match the expected results

# Add more test functions for other methods as needed
