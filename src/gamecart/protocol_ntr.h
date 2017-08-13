#include <cstdint>

// TODO
void Cart_NTRInit();
void NTR_SendCommand(const uint8_t command[8], uint32_t pageSize, uint32_t latency, void* buffer);
