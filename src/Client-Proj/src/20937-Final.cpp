#include "Client.h"
#include "RequestBuilder.h"
int main()
{
   /* char recvbuf[1024] = {0};
    const char* sendbuf = R"(Hello world!)";
    auto c = Client();*/
	//c.send(sendbuf, strlen(sendbuf) + 1);
   char name[255] = {0};
   char id[16] = { 0 };
   constexpr char version= '1';
   RequestBuilder s(id, version);
   s.build_req_register(name);
}