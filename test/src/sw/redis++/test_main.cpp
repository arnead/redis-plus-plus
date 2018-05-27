/**************************************************************************
   Copyright (c) 2017 sewenew

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
 *************************************************************************/

#include <unistd.h>
#include <chrono>
#include <iostream>
#include <sw/redis++/redis++.h>

namespace {

void print_help();

sw::redis::ConnectionOptions parse_options(int argc, char **argv);

}

int main(int argc, char **argv) {
    try {
        auto opts = parse_options(argc, argv);
    } catch (const sw::redis::Error &e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }

    return 0;
}

namespace {

void print_help() {
    std::cerr << "Usage: test_redis++ -h host -p port "
        << "[-s socket_timeout_in_milliseconds]" << std::endl;
}

sw::redis::ConnectionOptions parse_options(int argc, char **argv) {
    std::string host;
    int port = 0;
    std::chrono::milliseconds socket_timeout{};
    int opt = 0;
    while ((opt = getopt(argc, argv, "h:p:s:")) != -1) {
        switch (opt) {
        case 'h':
            host = optarg;
            break;

        case 'p':
            port = std::stoi(optarg);
            break;

        case 's':
            socket_timeout = std::chrono::milliseconds(std::stoi(optarg));
            break;

        default:
            print_help();
            throw sw::redis::Error("Failed to parse connection options");
            break;
        }
    }

    if (host.empty() || port <= 0 || socket_timeout < std::chrono::milliseconds{0}) {
        print_help();
        throw sw::redis::Error("Invalid connection options");
    }

    sw::redis::ConnectionOptions opts;
    opts.host = host;
    opts.port = port;
    opts.socket_timeout = socket_timeout;

    return opts;
}

}
