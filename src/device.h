#pragma once

#include <cstdint>
#include <vector>

using std::uint8_t;
using std::uint32_t;

class Flashcart {
    public: // TODO: Is there more I should think about?
        Flashcart();
        virtual bool setup() = 0;
        virtual void readFlash(uint32_t address, uint32_t length, uint8_t *buf);
        virtual void eraseFlash(uint32_t address, uint32_t length);
        virtual void writeFlash(uint32_t address, uint32_t length, const uint8_t *buf);
        virtual void cleanup() = 0;

protected:
        static const uint8_t NTR_cmdDummy[8];
        static const uint8_t NTR_cmdChipid[8];

        static void reset();
        static void waitFlashBusy();

        virtual void formatReadCommand(uint8_t *cmdbuf, uint32_t address) = 0;
        virtual void formatEraseCommand(uint8_t *cmdbuf, uint32_t address) = 0;
        virtual void formatWriteCommand(uint8_t *cmdbuf, uint32_t address, uint8_t value) = 0;
};

extern std::vector<Flashcart*> flashcart_list;
