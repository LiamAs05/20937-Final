from utils.connutils import Connutils


def main():
    port = Connutils.port()


if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print("Ctrl+C pressed, exiting...")
