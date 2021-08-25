#include <iostream>
#include <cstdlib>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
void		error_handling(std::string str)
{
    std::cout << str << std::endl;
    exit(1);
}

int			main(int argc, char *argv[])
{
	int		sock;
	char	message[BUF_SIZE];
	int		str_len;
	struct sockaddr_in	serv_adr;

	if (argc != 3)
	{
		std::cout << "Usage : " << argv[0] << " <IP> <PORT>" << std::endl;
		exit(1);
	}
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1)
		error_handling("socket() error");
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_adr.sin_port = htons(atoi(argv[2]));
	if (connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("connect() error");
	else
		std::cout << "Connecting........" << std::endl;
	while (1)
	{
		fputs("Input message(Q to quit) : ", stdout);
		fgets(message, BUF_SIZE, stdin);
		if (!strcmp(message, "q\n"))
			break ;
		str_len = write(sock, message, strlen(message));
		int recv_len = 0;
		int recv_cnt = 0;
		while (recv_len < str_len)
		{
			recv_cnt = read(sock, message, BUF_SIZE - 1);
			message[str_len] = 0;
			recv_len += recv_cnt;
			std::cout << "Message from server : " << message << std::endl;
		}
	}
	close(sock);
	return (0);
}
