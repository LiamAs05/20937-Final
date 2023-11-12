from utils.server import Server
from sys import exit, stderr


def main():
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
