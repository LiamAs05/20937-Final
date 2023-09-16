from utils.connutils import Connutils
from utils.models import User
from database.database import Database


def main():
    port = Connutils.port()
    database = Database()


if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print("Ctrl+C pressed, exiting...")
