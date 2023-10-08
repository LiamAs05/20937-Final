from defensive.src.server.database.database import Database
from defensive.src.server.utils.connutils import Connutils
from sys import exit


def main():
    database = Database()  # Start DB Connection
    with Connutils() as connection:
        connection.start_selector()


if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:  # Clean exit on keyboard interrupt
        print("Ctrl+C pressed, exiting...")
    finally:
        exit(0)
