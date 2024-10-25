#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"

ServerConfig server_load_config(int argc, char** argv)
{
    ServerConfig config = {
        .port = 6000,
    };

    for(size_t i = 1; i < argc; i+=2)
    {
        if(strcmp(argv[i], "-port") == 0)
        {
            if(i + 1 >= argc)
            {
                fprintf(stderr, "ERROR: Invalid number of arguments for flag '-port'\n");
                exit(EXIT_FAILURE);
            }

            config.port = (NetworkPort)atoi(argv[i+1]);
        }
        else
        {
            fprintf(stderr, "ERROR: \'%s\' is not a valid flag\n", argv[i]);
            exit(EXIT_FAILURE);
        }
    }

    return config;
}