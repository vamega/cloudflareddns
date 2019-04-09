#include <iostream>
#include <array>

#include <ares.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <time.h>

void dns_callback(void *arg, int status, int timeouts, struct hostent *host) {
    if (status == ARES_SUCCESS)
    {
        std::cout << host->h_length << "\n";
        std::cout << host->h_name << "\n";

        for(int i = 0; i < host->h_length; ++i) {
            struct in_addr addr = *(in_addr*)host->h_addr_list[i];
            std::cout << inet_ntoa(addr) << std::endl;
        }
    }
    else
        std::cout << "lookup failed: " << status << '\n';
}

void main_loop(ares_channel &channel) {
    int nfds, count;
    fd_set readers, writers;
    timeval tv, *tvp;

    while (1) {
        FD_ZERO(&readers);
        FD_ZERO(&writers);
        nfds = ares_fds(channel, &readers, &writers);
        if (nfds == 0)
            break;
        tvp = ares_timeout(channel, NULL, &tv);
        count = select(nfds, &readers, &writers, NULL, tvp);
        ares_process(channel, &readers, &writers);
    }

}

int main() {
    struct ares_options opts = {0};
    int optmask = 0;

    opts.flags = ARES_FLAG_NOSEARCH | ARES_FLAG_NOALIASES;
    optmask |= ARES_OPT_FLAGS;

    std::array<struct in_addr, 4> nameservers;

    inet_aton("208.67.222.222", &nameservers[0]); // resolver1.opendns.com
    inet_aton("208.67.220.220", &nameservers[1]); // resolver2.opendns.com

    opts.servers = nameservers.data();
    opts.nservers = nameservers.size();

    optmask |= ARES_OPT_SERVERS;

    int res;
    ares_channel channel;
    if ((res = ares_init_options(&channel, &opts, optmask)) != ARES_SUCCESS) {
        std::cout << "ares feiled: " << res << '\n';
        return 1;
    }


    ares_gethostbyname(channel, "myip.opendns.com", AF_INET, dns_callback, NULL);
    main_loop(channel);

    return 0;
}
