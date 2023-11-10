from defensive.src.server.database.database import Database
from defensive.src.server.utils.connutils import Server
from sys import exit, stderr


def main():
    database = Database()  # Start DB Connection
    with Server() as connection:
        connection.start()


if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:  # Clean exit on keyboard interrupt
        print("Ctrl+C pressed, exiting...")
    except Exception as e:
        print(e, file=stderr)
    finally:
        exit(0)
