#include <iostream>
#include <cstdlib>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>

void		error_handling(std::string message)
{
	std::cout << message << std::endl;
	exit(1);
}

int			main(int argc, char *argv[])
{
	int		serv_sock;
	int		clnt_sock;
	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	struct timeval timeout;
	socklen_t adr_sz;
	int		fd_max;
	int		str_len;
	fd_set	reads;
	fd_set	copy_reads;
	int		fd_num;
	int		i;
	char	buf[1024];

	if (argc != 2)
	{
		std::cout << "error" << std::endl;
		exit(1);
	}
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));
	if (bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
		error_handling("bind error");
	if (listen(serv_sock, 5) == -1)
		error_handling("listen error");

	FD_ZERO(&reads); //read(fd_set)의 모든 비트를 0으로 초기화
	FD_SET(serv_sock, &reads); //reads(fd_set)에 전달된 serv_sock(fd의 번호)를 등록한다.
	fd_max = serv_sock;
	while (1)
	{
		copy_reads = reads;
		timeout.tv_sec = 5; //timeout시간을 지정
		timeout.tv_usec = 500;
		if ((fd_num = select(fd_max, &copy_reads, 0, 0, &timeout)) == -1) //지정한 fd의 값이 바뀔때까지 대기한다. timeout시간이 지나면 자동으로 0을 반환
			break ;
		if (fd_num == 0)
			continue ;
		for (i = 0; i < fd_max + 1; i++) //select 함수가 1이상 반환됬을때 실행하는 반복문, 1이상 반환되었으므로 FD_ISSET함수를 호출하여 상태변화가 있었던 파일 디스크립트를 찾는다.
		{
			if (FD_ISSET(i, &copy_reads))
			{
				if (i == serv_sock)
				/*
					상태변화가 확인되면 제일 먼저 서버 소켓에서 변화가 있었는지 확인한다. 그리고 소켓의 상태변화가 맞으면 이어서 연결요청에 대한 수락의 과정(accept)을 진행한다.
					특히, 63행을 통해서 fd_set형 변수 read에 클라이언트와 연결된 소켓의 파일 다스크립터 정보를 등록함에 주목해야한다!!!!
				*/
				{
					adr_sz = sizeof(clnt_addr);
					clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &adr_sz);
					/*
						accept(int sock, struct sockaddr *addr, socklen_t *addr_len);
						accept함수는 연결요청 대기 큐에서 대기중인 클라이언트의 연결요청을 수락하는 기능의 함수이다.
						따라서 accept함수는 호출성공 시 내부적으로 데이터 입출력에 사용할 소켓을 생성하고, 그 소켓의 파일 디스크립터를 반환한다.
						중요한 점은 소켓이 자동으로 생성되어 연결요청을 한 클라이언트 소켓에 연결까지 이루어진다는 점이다.
					*/
					if (clnt_sock == -1)
						error_handling("accept error");
					FD_SET(clnt_sock, &reads);
				}
				if (fd_max < clnt_sock)
					fd_max = clnt_sock;
				std::cout << "connected client : " << clnt_sock << std::endl;
			}
			else
			{
				str_len = read(i, buf, 1024);
				if (str_len == 0) //수신한 데이터가 eof인 경우 소켓을 종료하고 변수 reads에서 해당정보를 삭제한다.
				{
					FD_CLR(i, &reads);
					close(i);
					std::cout << "closed client : " << i << std::endl;
				}
				else
				{
					write(i, buf, str_len);
				}
			}
		}
	}
	close(serv_sock);
	return (0);
}
