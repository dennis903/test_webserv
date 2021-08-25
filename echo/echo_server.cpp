#include <iostream>
#include <cstdlib>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
void		error_handling(std::string message);

int			main(int argc, char *argv[])
{
	int					serv_sock;
	int					clnt_sock;
	char				message[BUF_SIZE];
	struct sockaddr_in	serv_adr;
	struct sockaddr_in	clnt_adr;
	socklen_t			clnt_adr_sz;
	int					str_len;
	int					option;
	int					optlen;
	if (argc != 2)
	{
		std::cout << "Usage : " << argv[0] << " <port>" << std::endl;
		exit(1);
	}
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (serv_sock == -1)
		error_handling("socket() error");
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));
	optlen = sizeof(option);
	option = true;
	setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, (void *)&option, optlen);

	if (bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind() error");
	if (listen(serv_sock, 5) == -1)
		error_handling("listen() error");
	clnt_adr_sz = sizeof(clnt_adr);
	for (int i = 0; i < 5; i++)
	{
		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
		if (clnt_sock == -1)
			error_handling("accept error");
		else
			std::cout << "Connected client " << i << std::endl;
		while((str_len = read(clnt_sock, message, BUF_SIZE)) != 0)
			write(clnt_sock, message, str_len);
		close(clnt_sock);
	}
	close(serv_sock);
	return (0);
}

void		error_handling(std::string str)
{
	std::cout << str << std::endl;
	exit(1);
}