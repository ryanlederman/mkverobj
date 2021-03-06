#include <stdio.h>
#include <stdlib.h>
#include "version.h"

int main (int argc, char** argv)
{
    const version_resource* res = get_version_resource();
    printf("major: %u, minor: %u, build: %u, notes: '%s'\n",
        res->major, res->minor, res->build, res->notes);
    return EXIT_SUCCESS;
}
