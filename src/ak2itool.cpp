#include <cstdio>

using std::puts;
using std::printf;
using std::FILE;
using std::fopen;
using std::fread;
using std::fwrite;
using std::fclose;

#include <cstdlib>

using std::exit;

#include <cstring>

using std::memset;
using std::memcpy;

extern "C" { // TODO: fix this in the powerslaves header itself.
#include <powerslaves.h>
}
#include <device.h>

enum behavior {
    AK2I_DUMP_FLASH,
    AK2I_WRITE_FLASH,
    DEVICE_AUTO = AK2I_DUMP_FLASH
};

struct args {
    unsigned flash_len;
    const char *flash_filename;
    enum behavior beh;
};

static void usage(const char *progname) {
    printf( "Usage: %s {read,dump,write,flash} <file> [length]\n", progname);
    puts(   "    read: dumps the ak2i firmware to a file <file>\n"
            "    dump: alias for read\n"
            "    write: flashes the ak2i firmware from a file <file>\n"
            "    flash: alias for write\n"
            "    length: an optional argument that limits the length read or written.");
}

static void parse_args(int argc, char *argv[], struct args *arg) {
    if (argc < 3) {
        puts("Not enough arguments!");
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    switch (argv[1][0]) { // Switch on first character of command.
        case 'd':
        case 'r':
            arg->beh = AK2I_DUMP_FLASH;
            arg->flash_len = 0x1000000;
            break;
        case 'f':
        case 'w':
            arg->beh = AK2I_WRITE_FLASH;
            arg->flash_len = 0x20000; // TODO: Change this default as necesssary?
            break;
        default:
            puts("Incorrect command!");
            usage(argv[0]);
            exit(EXIT_FAILURE);
    }

    arg->flash_filename = argv[2];

    if (argc > 3) {
        arg->flash_len = strtol(argv[3], NULL, 0);
    }
}

int main(int argc, char *argv[]) {
    struct args arg;
    parse_args(argc, argv, &arg);

    uint8_t *buffer = new uint8_t[arg.flash_len];
    if (!buffer) {
        puts("Memory allocation failure!");
        exit(EXIT_FAILURE);
    }
    memset(buffer, 0, arg.flash_len);

    //if (powerslaves_select(nullptr)) {
    //    puts("Failed to initialize powerslaves!");
    //    exit(EXIT_FAILURE);
    //}

    Flashcart *cart = Flashcart::detectCart();

    if (cart == nullptr) {
        puts("No ak2i (clone) found!");
        exit(EXIT_FAILURE);
    }

    switch (arg.beh) {
        case AK2I_DUMP_FLASH: {
            FILE *dumpfile = fopen(arg.flash_filename, "wb");
            if (!dumpfile) {
                printf("Failed to open file %s for writing!\n", arg.flash_filename);
                exit(EXIT_FAILURE);
            }
            cart->readFlash(0, arg.flash_len, buffer);
            cart->cleanup();

            fwrite(buffer, arg.flash_len, 1, dumpfile);
            fclose(dumpfile);
        } break;

        case AK2I_WRITE_FLASH: {
            FILE *flashfile = fopen(arg.flash_filename, "rb");
            if (!flashfile) {
                printf("Failed to open file %s for reading!\n", arg.flash_filename);
                exit(EXIT_FAILURE);
            }
            fread(buffer, arg.flash_len, 1, flashfile);
            fclose(flashfile);
            puts("Flashing cartridge. This may take a while.\n");
            //cart->eraseFlash(0, arg.flash_len);
            cart->writeFlash(0, arg.flash_len, buffer);

            // TODO: verify the flash?
            cart->cleanup();
        } break;

        default:
            puts("Invalid behavior! This should never happen.\nIf it does, please submit a bug report.");
            exit(EXIT_FAILURE);
    }

    delete[] buffer;
    powerslaves_exit();

    return EXIT_SUCCESS;
}
