#include "Client.h"

/// <summary>
/// Constructor for the client class
/// </summary>
Client::Client() : ip(std::string()), port(0), name(std::string()), path(std::string())
{
    get_transfer_info();  // Obtain transfer information from a file

    startup();           // Initialize the Winsock library
    resolveAddress();    // Resolve the server's address
    connect();           // Connect to the server

    if (get_me_info() == ME_INFO_MISSING)
    {
	    // TODO register
    }
    else
    {
	    // TODO Login
    }

    
}

/// <summary>
/// Send a message to the server
/// </summary>
/// <param name="buf">Content of the message</param>
/// <param name="len">Length of buf</param>
void Client::send(const char* buf, const int len) const
{
    int iResult = 0;

    REPEAT_THREE_TIMES(
        {
            iResult = ::send(ConnectSocket, buf, len, 0);
            if (iResult > 0)
                break;
        }
    )

        if (iResult == SOCKET_ERROR) {
            printf("Server Responded with an Error:\n"
                "send failed: %d\n", WSAGetLastError());
            closesocket(ConnectSocket);
            WSACleanup();
            _exit(ERR);
        }
}

/// <summary>
/// Receive a message from the server
/// </summary>
/// <param name="buf">Buffer to receive the message</param>
/// <param name="len">Length of buf</param>
void Client::recv(char* buf, const int len) const
{
    int iResult = 0;
    REPEAT_THREE_TIMES(
        {
            iResult = ::recv(ConnectSocket, buf, len, 0);
            if (iResult > 0)
                break;
        }
    )

        if (iResult == 0) {
            printf("Connection closed\n");
        }
        else if (iResult == SOCKET_ERROR) {
            printf("Server Responded with an Error:\n"
                "recv failed: %d\n", WSAGetLastError());
            _exit(ERR);
        }
}

/// <summary>
/// Read transfer information from a file and extract the server's IP address and port.
/// </summary>
void Client::get_transfer_info()
{
	constexpr u_short ip_and_port_index = 0;
	constexpr u_short name_index = 1;
	constexpr u_short path_index = 2;

    const std::string transfer_info_path("C:/Users/liamd/Desktop/20937-Final/src/Client-Proj/x64/Debug/transfer.info");
	const std::string content = Utils::read_file(transfer_info_path);
    std::vector<std::string> lines = Utils::split_lines(content);

	// Extract the server's IP address from the file content
    ip = lines[ip_and_port_index].substr(0, lines[ip_and_port_index].find(':'));

    // Extract and convert the server's port from the file content
    const unsigned long long port_start = lines[ip_and_port_index].find(':') + 1;
    const unsigned long long port_size = lines[ip_and_port_index].find('\n') - (lines[ip_and_port_index].find(':') + 1);
    const std::string port_str = lines[ip_and_port_index].substr(port_start, port_size);
    port = static_cast<u_short>(std::stoi(port_str));

    // Extract name
    name = lines[name_index];

    // Extract file path
    path = lines[path_index];
}

bool Client::get_me_info()
{
    constexpr u_short name_index = 0;
    constexpr u_short unique_id_index = 1;
    constexpr u_short private_key_index = 2;

    const std::string transfer_info_path("");
    const std::string content = Utils::read_file(transfer_info_path);

    if (content.empty())
    {
        std::cout << "me.info does not exist, registering..." << std::endl;
        return ME_INFO_MISSING;
    }

    std::vector<std::string> lines = Utils::split_lines(content);
    name = content[name_index];
    unique_id = content[unique_id_index];
    private_key = content[private_key_index];

    std::cout << "me.info exists, logging in as " << name << std::endl;

    return ME_INFO_EXISTS;
}

/// <summary>
/// Initialize the Winsock library for network communication.
/// </summary>
void Client::startup()
{
    // non-fallible code, as it is local
    WSADATA wsaData;
    const int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        _exit(ERR);
    }
}

/// <summary>
/// Resolve the server's IP address and configure the client's socket.
/// </summary>
void Client::resolveAddress()
{
    ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ConnectSocket == INVALID_SOCKET) {
        printf("Error at socket(): %ld\n", WSAGetLastError());
        WSACleanup();
        _exit(ERR);
    }

    clientService.sin_family = AF_INET;
    inet_pton(AF_INET, ip.c_str(), &clientService.sin_addr.s_addr);
    clientService.sin_port = htons(port);
}

/// <summary>
/// Attempt to establish a connection with the server.
/// If the connection fails, retry up to three times.
/// </summary>
void Client::connect()
{
    int iResult = 0;

    // fallible code, hence we repeat three times
    REPEAT_THREE_TIMES(
        {
            iResult = ::connect(ConnectSocket,
                reinterpret_cast<SOCKADDR*>(&clientService),
                sizeof(clientService));
            if (iResult == NOERROR) break;
        }
    )

        // If the fallible code failed, send an error message
            if (iResult == SOCKET_ERROR) {
                closesocket(ConnectSocket);
                printf("Server Responded with an Error:\n"
                    "Unable to connect to the server: %ld\n", WSAGetLastError());
                WSACleanup();
                _exit(ERR);
            }
}
