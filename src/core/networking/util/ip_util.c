#include <arpa/inet.h>  // inet_addr, inet_ntop, sockaddr_in
// #include <sys/socket.h>

//! Converts byte4 and a port to a sockaddr_in struct.
struct sockaddr_in byte4_to_ip(byte4 input, int port)
{
    struct sockaddr_in send_addr;
    // memset(&send_addr, 0, sizeof(send_addr));
    send_addr.sin_family = AF_INET;
    // send_addr.sin_addr.s_addr = input.x << 24 | input.y << 16 | input.z << 8 | input.w;
    send_addr.sin_addr.s_addr = input.w << 24 | input.z << 16 | input.y << 8 | input.x;
    send_addr.sin_port = htons(port);
    return send_addr;
}

struct sockaddr_in string_to_ip(char *text, int port)
{
    struct sockaddr_in send_addr;
    memset(&send_addr, 0, sizeof(send_addr));
    send_addr.sin_family = AF_INET;
    send_addr.sin_addr.s_addr = inet_addr(text); // replace with the destination IP address
    send_addr.sin_port = htons(port);
    return send_addr;
}

char* ip4_to_string(struct sockaddr_in send_addr)
{
    char *ip_string = inet_ntoa(send_addr.sin_addr);
    in_port_t port = ntohs(send_addr.sin_port);
    char *result = malloc(strlen(ip_string) + 6);
    sprintf(result, "%s:%u", ip_string, port);
    return result;
    /*static char ip_str[INET_ADDRSTRLEN]; // buffer to hold the string representation of the IP address
    if (inet_ntop(AF_INET, &send_addr.sin_addr, ip_str, sizeof(ip_str)) == NULL)
    {
        // an error occurred, print the error and exit
        perror("inet_ntop");
        return NULL;
    }
    snprintf(str, size, "%s:%u", ip, port);
    return ip_str;*/
}

int debug_ip4(struct sockaddr_in send_addr)
{
    char ip_str[INET_ADDRSTRLEN]; // buffer to hold the string representation of the IP address
    if (inet_ntop(AF_INET, &send_addr.sin_addr, ip_str, sizeof(ip_str)) == NULL)
    {
        // an error occurred, print the error and exit
        perror("inet_ntop");
        return 1;
    }
    printf("IP address: %s\n", ip_str);
    return 0;
}