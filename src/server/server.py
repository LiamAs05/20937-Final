from database.database import Database
from utils.connutils import Connutils

# from utils.models import User


def main():
    database = Database()  # Start DB Connection
    connection = Connutils()
    connection.start_selector()


if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:  # Clean exit on keyboard interrupt
        print("Ctrl+C pressed, exiting...")
