#include "core/shmem.h"

namespace holodeck::core {

constexpr std::string kHolodeckBasePath = "/HOLODECK_MEM";

Shmem::Shmem(const std::string& name, unsigned buffer_size,
             const std::string& uuid) :
    mem_path_(kHolodeckBasePath + uuid + "_" + name), mem_size_(buffer_size) {
#ifdef _Win32
  // TODO(joshgreaves): Check this code
  std::wstring s_temp = std::wstring(mem_path_.begin(), mem_path_.end());
  LPCWSTR windows_mem_path = STemp.c_str();
  mem_file_ = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE,
                                0, mem_size_, windows_mem_path);
  LPVOID windows_mem_ptr = MapViewOfFile(mem_file_, FILE_MAP_ALL_ACCESS,
                                         0, 0, mem_size_);
  mem_ptr_ = static_cast<void*>(windows_mem_ptr);
#elif defined __linux__
  mem_file_ = shm_open(mem_path_.c_str(), O_CREAT | O_RDWR | O_TRUNC, 0777);
  ftruncate(mem_file_, mem_size_);
  mem_ptr_ = mmap(nullptr, mem_size_, PROT_READ | PROT_WRITE, MAP_SHARED,
                  mem_file_, 0);
#endif
}

}  // namespace holodeck::core
