#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"

ClientConfig client_load_config(int argc, char** argv)
{
    ClientConfig config = {
        .ip = NETWORK_IP_LOCALHOST,
        .port = 6000,
    };

    for(int i = 1; i < argc; i++)
    {
        char* flag = argv[i];

        if(strcmp(flag, "-ip") == 0)
        {
            i++;

            if(i >= argc)
            {
                fprintf(stderr, "ERROR: Invalid number of arguments for flag '%s'!\n", flag);
                exit(EXIT_FAILURE);
            }

            if(!network_str_to_ip(argv[i], &config.ip))
            {
                fprintf(stderr, "ERROR: '%s' is not a valid ip!\n", argv[i]);
                exit(EXIT_FAILURE);
            }
        }
        else if(strcmp(flag, "-port") == 0)
        {
            i++;

            if(i >= argc)
            {
                fprintf(stderr, "ERROR: Invalid number of arguments for flag '%s'!\n", flag);
                exit(EXIT_FAILURE);
            }

            config.port = atoi(argv[i]);
        }
        else
        {
            fprintf(stderr, "ERROR: '%s' is not a valid flag!\n", flag);
            exit(EXIT_FAILURE);
        }
    }
    
    return config;
}