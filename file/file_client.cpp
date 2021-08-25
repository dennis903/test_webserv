#include <iostream>
#include <cstdlib>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 39

void		error_handling(std::string message)
{
	std::cout << message << std::endl;
	exit(1);
}

int			main(int argc, char *argv[])
{
	int		sd;
	FILE	*fp;
	char	buf[BUF_SIZE];
	int		read_cnt;
	struct sockaddr_in serv_adr;

	if (argc != 3)
	{
		std::cout << "error" << std::endl;
		exit(1);
	}
	fp = fopen("text.txt", "wb");
	sd = socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_adr.sin_port = htons(atoi(argv[2]));

	connect(sd, (struct sockaddr *)&serv_adr, sizeof(serv_adr));

	while ((read_cnt = read(sd, buf, BUF_SIZE)) != 0)
		fwrite((void *)buf, 1, read_cnt, fp);
	
	std::cout << "Received file data" << std::endl;
	write(sd, "Thank you", 10);
	fclose(fp);
	close(sd);
	return (0);
}