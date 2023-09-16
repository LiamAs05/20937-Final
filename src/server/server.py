from database.database import Database
from utils.connutils import Connutils
from utils.models import User


def main():
    port = Connutils.port()
    database = Database()


if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print("Ctrl+C pressed, exiting...")
