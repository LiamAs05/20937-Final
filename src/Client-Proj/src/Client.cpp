#include "Client.h"

/// <summary>
/// Constructor for the client class
/// </summary>
Client::Client() :
    ip(std::string()), port(0), name(std::string(size_name, 0)),
	path(std::string(size_name, 0)), unique_id_str(std::string()),
	unique_id_bytes(std::array<char, size_req_client_id>()), private_key(std::string()),
	req_builder(RequestBuilder("Place Holder", 3)),
	res_parser(ResponseParser(this)), private_key_wrapper(RSAPrivateWrapper())
{
    get_transfer_info();  // Obtain transfer information from a file

    startup();           // Initialize the Winsock library
    resolveAddress();    // Resolve the server's address
    connect();           // Connect to the server

    establish_server_connectivity();    // Login or Register to server
    req_builder.set_client_id(unique_id_bytes.data());
    std::cout << unique_id_str;
}

/// <summary>
/// Send a message to the server
/// </summary>
/// <param name="buf">Content of the message</param>
/// <param name="len">Length of buf</param>
void Client::send(const char* buf, const unsigned len) const
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
            _exit(err);
        }
}

/// <summary>
/// Receive a message from the server
/// </summary>
/// <param name="buf">Buffer to receive the message</param>
/// <param name="len">Length of buf</param>
void Client::recv(char* buf, const unsigned len) const
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
            _exit(err);
        }
}

std::string Client::get_name()
{
    return name;
}

void Client::set_name(const std::string& name)
{
    this->name = name;
}

std::string Client::get_id()
{
    return this->unique_id_str;
}

void Client::set_id(const std::string& id)
{
    this->unique_id_str = id;
    std::vector<char> id_vec = Utils::hex_val(id);
    std::copy_n(id_vec.begin(), size_req_client_id, unique_id_bytes.begin());
}

void Client::set_id(std::array<char, size_req_client_id>& id)
{
    std::copy_n(id.begin(), size_req_client_id, this->unique_id_bytes.begin());
	std::vector<unsigned char> vector_id(size_req_client_id);
    std::copy_n(id.begin(), id.size(), vector_id.begin());
    this->unique_id_str = Utils::hex_str(vector_id);
}

void Client::establish_server_connectivity()
{
    if (me_info_missing == get_me_info())
    {
        register_as_new_client();
        req_builder.set_client_id(unique_id_bytes.data());
    	create_me_info();
    }
    else
    {
        req_builder.set_client_id(unique_id_bytes.data());
        std::vector c = req_builder.build_req_login(name.data());
    	send(c.data(), c.size());
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
	const std::string content = Utils::read_file("src/transfer.info");
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

    const std::string me_info_path("src/me.info");
	const std::string content = Utils::read_file(me_info_path);

    if (content.empty())
    {
        std::cout << "me.info does not exist, registering..." << std::endl;
        return me_info_missing;
    }

    const std::vector<std::string> lines = Utils::split_lines(content);
    name = lines[name_index];
    set_id(lines[unique_id_index]);
    private_key = lines[private_key_index];

    std::cout << "me.info exists, logging in as " << name << std::endl;

    return me_info_exists;
}

void Client::create_me_info()
{
    std::stringstream content;
    content << name << "\n" << unique_id_str << "\n" << "somekey";
    Utils::write_file("src/me.info", content.str());
}

void Client::register_as_new_client()
{
    std::vector req(req_builder.build_req_register(name.data()));

	send(req.data(), static_cast<unsigned>(req.size()));
    recv(req.data(), size_res_headers);

    if (!res_parser.parse_response(req))
    {
        _exit(err);
    }
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
        _exit(err);
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
        _exit(err);
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
                _exit(err);
            }
}
