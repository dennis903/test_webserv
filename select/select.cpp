#include <iostream>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>

int     main()
{
    fd_set  reads, temps;
    int result, str_len;
    char buf[30];
    struct timeval timeout;

    FD_ZERO(&reads);
    FD_SET(0, &reads);

    while (1)
    {
        temps = reads;
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;
        result = select(1, &temps, 0, 0, &timeout);
        if (result == -1)
        {
            std::cout << "select() error" << std::endl;
            break ;
        }
        else if (result == 0)
        {
            std::cout << "Time out !" << std::endl;
        }
        else
        {
            if (FD_ISSET(0, &temps))
            {
                str_len = read(0, buf, 30);
                buf[str_len] = 0;
                std::cout << "message from console : " << buf << std::endl;
            }
        }
    }
}
