#include <iostream>
#include <cstdlib>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void	error_handling(std::string message);

int			main(int argc, char* argv[])
{
	int		sock;
	struct sockaddr_in serv_addr;
	char message[30];
	int str_len;

	if (argc != 3)
	{
		std::cout << "Usage : " << argv[0] << " <IP> <PORT> " << std::endl;
		exit(1);
	}
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1)
		error_handling("socket() error");
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port=htons(atoi(argv[2]));
	
	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
		error_handling("connect() error!");
	str_len = read(sock, message, sizeof(message) - 1);
	if (str_len == -1)
		error_handling("read() error!");
	std::cout << "Message from server : " << message << std::endl;
	close(sock);
	return (0);
}

void error_handling(std::string message)
{
	std::cout << message << std::endl;
	exit(1);
}