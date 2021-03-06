#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include "version.h"

#define APP_NAME "mkverobj"
#define TMP_FILE "VERSION"

static inline bool valid_str(const char* str)
{
    return nullptr != str && 0 != *str;
}

static inline uint16_t string_to_uint16(const char* str)
{
    return static_cast<uint16_t>(std::strtoul(str, nullptr, 10));
}

static int print_usage()
{
    std::cout << "usage:" << std::endl << "\t" APP_NAME " <major> <minor> <build> <notes> <output file>"
        << std::endl;
    return EXIT_FAILURE;
}

int main(int argc, char** argv)
{
    try {
        if (argc < 6 || !valid_str(argv[1]) || !valid_str(argv[2]) ||
            !valid_str(argv[3]) || !valid_str(argv[4]) || !valid_str(argv[5]))
            return print_usage();

        version_resource res;
        res.major = string_to_uint16(argv[1]);
        res.minor = string_to_uint16(argv[2]);
        res.build = string_to_uint16(argv[3]);
        std::strncpy(res.notes, argv[4], MAX_VER_NOTES - 1);

        std::ofstream strm(TMP_FILE, std::ios::binary | std::ios::trunc);
        strm.exceptions(strm.badbit | strm.failbit);
        strm.write(reinterpret_cast<const char*>(&res), sizeof(res));
        strm.flush();

        const char* linker = getenv("LD");
        if (!linker) {
            linker = "ld";
            std::cerr << APP_NAME ": " << "WARNING: $LD is not set; using 'ld'" << std::endl;
        }

        std::stringstream cmd;
        cmd << linker << " -r -b binary -o " << argv[5] << " " << TMP_FILE;
        int sysret = system(cmd.str().c_str());

        if (0 != std::remove(TMP_FILE))
            std::cerr << APP_NAME ": WARNING: unable to remove " << TMP_FILE
                << ": " << std::strerror(errno) << std::endl;

        if (0 != sysret) {
            std::cerr << APP_NAME ": executing '" << cmd.str() << "' failed: "
                << (-1 == sysret ? -1 : WEXITSTATUS(sysret)) << std::endl;
            return EXIT_FAILURE;
        }

        return EXIT_SUCCESS;
    } catch (std::exception& ex) {
        std::cerr << APP_NAME ": " << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}
