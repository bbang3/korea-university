#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/ip.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h> // Implement I/O multiplexing with epoll()
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/select.h>

#define MAX_EVENT 100
#define MAX_BUFFER_SIZE 1024

void http_handler(int client_fd, char *request, const char *rsrc_path);   // handle HTTP requests
void fill_header(char *header, int status, long len, char *content_type); // fill header based on status code, content length, and content type(str)
void find_mime(char *ct_type, char *uri);                                 // return content type corresponding to given uri
void handle_404(int client_fd);                                           // handler for 400 Bad Request
void handle_400(int client_fd);                                           // handler for 404 Not Found

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        return -1;
    }

    const int PORT_NUM = atoi(argv[1]); // port number
    char rsrc_path[50];
    strcpy(rsrc_path, argv[2]); // resources path

    struct sockaddr_in serv_addr, clnt_addr; // server/client address,
    socklen_t clnt_addrlen = sizeof(clnt_addr);

    // Server address configuration
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(PORT_NUM);

    // Create listen socket
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd == -1)
    {
        return -1;
    }

    // bind socket and address
    if (bind(listen_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
    {
        return -1;
    }

    // listening
    if (listen(listen_fd, 5) == -1)
    {
        return -1;
    }

    // create epoll file descriptor
    int epoll_fd = epoll_create(100);
    if (epoll_fd == -1)
    {
        return -1;
    }

    // add listen socket to event pool
    struct epoll_event event;
    event.events = EPOLLIN; // notice when receive data
    event.data.fd = listen_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_fd, &event) == -1)
    {
        return -1;
    }

    struct epoll_event events[MAX_EVENT]; //
    memset(events, 0, sizeof(events));
    while (1)
    {
        // wait until event occurs
        int event_cnt = epoll_wait(epoll_fd, events, MAX_EVENT, -1);
        if (event_cnt == -1)
        {
            return -1;
        }

        for (int i = 0; i < event_cnt; i++) // check current events
        {
            if (events[i].data.fd == listen_fd) // event on listen socket
            {
                // accept new connection and create client socket
                int client_fd = accept(listen_fd, (struct sockaddr *)&clnt_addr, &clnt_addrlen);
                if (client_fd == -1)
                {
                    return -1;
                }

                // add client socket to epoll monitoring list
                event.events = EPOLLIN; // notice when receive data
                event.data.fd = client_fd;
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &event) == -1)
                {
                    return -1;
                }
            }
            else // event on client socket (e.g. HTTP requests)
            {
                int client_fd = events[i].data.fd;

                char buffer[MAX_BUFFER_SIZE] = {0};
                int bytes = read(client_fd, buffer, sizeof(buffer)); // read message

                if (bytes == -1) // if error
                {
                    handle_400(client_fd);
                }
                else if (bytes == 0) // request for connection close
                {
                    // delete client socket from epoll monitoring list and close socket
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_fd, NULL);
                    close(client_fd);
                }
                else // actual HTTP request
                {
                    http_handler(client_fd, buffer, rsrc_path);
                }
            }
        }
    }
    // close listening socket and epoll file descriptor
    close(listen_fd);
    close(epoll_fd);

    return 0;
}

void http_handler(int client_fd, char *request, const char *rsrc_path)
{
    char header[MAX_BUFFER_SIZE], buffer[MAX_BUFFER_SIZE];

    // split request by space -> first token contains method, second contains uri
    char *method = strtok(request, " ");
    char *uri = strtok(NULL, " ");

    // if request is malformed, return 400 Bad Request
    if (method == NULL || uri == NULL || strcmp(method, "GET") != 0)
    {
        handle_400(client_fd);
        return;
    }

    char safe_uri[MAX_BUFFER_SIZE];
    char local_uri[MAX_BUFFER_SIZE] = {0};
    struct stat st;

    strcpy(safe_uri, uri);
    if (!strcmp(safe_uri, "/")) // request for "/" means request for "index.html"
        strcpy(safe_uri, "/index.html");

    // local uri: concat resource path and request uri
    strcpy(local_uri, rsrc_path);
    strcat(local_uri, safe_uri);
    if (stat(local_uri, &st) == -1) // if no file exists for current uri, return 404 Not Found
    {
        handle_404(client_fd);
        return;
    }
    if (S_ISDIR(st.st_mode)) // if uri is directory, return 404 Not Found
    {
        handle_404(client_fd);
        return;
    }

    int fd = open(local_uri, O_RDONLY); // if file exists, read file
    if (fd == -1)
    {
        handle_400(client_fd); // error occurs while reading file, return 400 Bad Request
        return;
    }

    int content_len = st.st_size;
    char content_type[40];
    find_mime(content_type, local_uri);                  // find content-type for corresponding uri
    fill_header(header, 200, content_len, content_type); // fill header with 200 status code and corresponding content type and content length
    write(client_fd, header, strlen(header));            // send HTTP response header

    // send HTTP response body that contains corresponding file (use loop due to the files whose size exceeds the buffer size)
    int cnt;
    while ((cnt = read(fd, buffer, sizeof(buffer))) > 0)
        write(client_fd, buffer, cnt);
}

void fill_header(char *header, int status, long len, char *content_type)
{
    // HTTP header format for response
    // Persistent Implementation - notify HTTP/1.1 and use Connection: Keep-alive header
    const char HEADER_FMT[] = "HTTP/1.1 %d %s\nConnection: Keep-Alive\nContent-Length: %ld\nContent-Type: %s\n\n";
    char status_text[40];
    switch (status) // status text
    {
    case 200:
        strcpy(status_text, "OK");
        break;
    case 404:
        strcpy(status_text, "Not Found");
        break;
    case 400:
    default:
        strcpy(status_text, "Bad Request");
        break;
    }
    sprintf(header, HEADER_FMT, status, status_text, len, content_type); // fill header
}

void find_mime(char *ct_type, char *uri)
{
    char *ext = strrchr(uri, '.'); // extract file extension from uri
    if (!strcmp(ext, ".html"))     // determine content-type based on extension
        strcpy(ct_type, "text/html");
    else if (!strcmp(ext, ".jpg") || !strcmp(ext, ".jpeg"))
        strcpy(ct_type, "image/jpeg");
    else if (!strcmp(ext, ".png"))
        strcpy(ct_type, "image/png");
    else if (!strcmp(ext, ".css"))
        strcpy(ct_type, "text/css");
    else if (!strcmp(ext, ".js"))
        strcpy(ct_type, "text/javascript");
    else
        strcpy(ct_type, "text/plain");
}

void handle_400(int client_fd)
{
    char header[MAX_BUFFER_SIZE];
    const char SERVER_ERROR_CONTENT[] = "<h1>400 Bad Request</h1>\n";    // 400 Response body
    fill_header(header, 400, sizeof(SERVER_ERROR_CONTENT), "text/html"); // fill header with 400 status code

    write(client_fd, header, strlen(header));                             // send HTTP response header
    write(client_fd, SERVER_ERROR_CONTENT, sizeof(SERVER_ERROR_CONTENT)); // send HTTP response body
}

void handle_404(int client_fd)
{
    char header[MAX_BUFFER_SIZE];
    const char NOT_FOUND_CONTENT[] = "<h1>404 Not Found</h1>\n";      // 404 Response body
    fill_header(header, 404, sizeof(NOT_FOUND_CONTENT), "text/html"); // fill header with 404 status code

    write(client_fd, header, strlen(header));                       // send HTTP response header
    write(client_fd, NOT_FOUND_CONTENT, sizeof(NOT_FOUND_CONTENT)); // send HTTP response body
}
