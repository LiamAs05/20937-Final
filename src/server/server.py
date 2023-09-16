from database.database import Database
from utils.connutils import Connutils
from utils.models import User


def main():
    port = Connutils.port()  # Read connection port
    database = Database()  # Start DB Connection


if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:  # Clean exit on keyboard interrupt
        print("Ctrl+C pressed, exiting...")
