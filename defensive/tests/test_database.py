import pytest

from defensive.src.server.database.database import Database, User


# Define a fixture to create a Database instance for testing
@pytest.fixture
def test_database():
    return Database(in_memory=True)


def test_add_user(test_database):
    # Create a test user
    test_user = User(name="Mr", pub=b"testkey", aes_key=b"aeskey")
    # Add the user to the database
    test_database.add_user(test_user)
    # Check if the user exists in the database
    assert 1 in test_database.users

    # Create a test user
    test_user = User(name="Fancy", pub=b"testkey", aes_key=b"aeskey")
    # Add the user to the database
    test_database.add_user(test_user)
    # Check if the user exists in the database
    assert 2 in test_database.users

    # Create a test user
    test_user = User(name="Donut", pub=b"testkey", aes_key=b"aeskey")
    # Add the user to the database
    test_database.add_user(test_user)
    # Check if the user exists in the database
    assert 3 in test_database.users


def test_get_all_users(test_database):
    test_database.clear()
    test_add_user(test_database)

    users = test_database.get_all_users()
    assert len(users) == 3


def test_clear(test_database):
    test_user = User(name="Test User", pub=b"testkey", aes_key=b"aeskey")
    # Add the user to the database
    test_database.add_user(test_user)
    # Check if the user exists in the database
    test_database.clear()
    assert len(test_database.users) == 0
