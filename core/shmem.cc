#include "core/shmem.h"

namespace holodeck::core {

constexpr std::string HOLODECK_BASE_PATH = "/HOLODECK_MEM";

Shmem::Shmem(const std::string &name, int bufferSize,
                           const std::string &uuid) :
    _memPath(HOLODECK_BASE_PATH + uuid + "_" + name), _memSize(bufferSize) {
#ifdef _Win32
  std::wstring STemp = std::wstring(MemPath.begin(), MemPath.end());
  LPCWSTR WindowsMemPath = STemp.c_str();
  MemFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE,
                              0, this->MemSize, WindowsMemPath);
  MemPointer = static_cast<void*>(MapViewOfFile(MemFile, FILE_MAP_ALL_ACCESS,
                                                0, 0, this->MemSize));
#elif defined __linux__
  _memFile = shm_open(_memPath.c_str(), O_CREAT | O_RDWR | O_TRUNC, 0777);
  ftruncate(_memFile, _memSize);
  _memPointer = mmap(nullptr, _memSize, PROT_READ | PROT_WRITE, MAP_SHARED,
                     _memFile, 0);
#endif
}

}  // namespace holodeck::core
