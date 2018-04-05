/**
  * Shmem gives an interface to memory mapped files,
  * and makes it easier to write and read from them.
  */
#ifndef HOLODECK_CORE_SHMEM_H_
#define HOLODECK_CORE_SHMEM_H_

#include <string>

#ifdef _WIN32
#include "AllowWindowsPlatformTypes.h"
#include <windows.h>
#include "HideWindowsPlatformTypes.h"
#elif defined __linux__
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#endif

namespace holodeck::core {

class Shmem {
 public:
  /**
    * Constructor
    * Constructs a memory mapped file with a given name, prepended
    * by "HOLODECK_MEM_".
    * @param name the name of the memory mapped file. Is prepended.
    * @param buffer_size the number of bytes to allocated for the file.
    * @param uuid a UUID for the Holodeck instance to allow multiple shared
    * blocks on one machine.
    */
  explicit Shmem(const std::string& name, unsigned buffer_size,
                 const std::string& uuid);

  Shmem(const Shmem&) = delete;

  Shmem& operator=(const Shmem&) = delete;

  ~Shmem() = default;

  /**
    * GetPtr
    * Gets a pointer to the start of the memory mapped file.
    * @return a void pointer to the start of the memory buffer.
    */
  void* GetPtr() const { return mem_ptr_; }

  /**
    * Size
    * Gets the size of the allocated memory mapped file.
    * @return the size in bytes of the file.
    */
  size_t Size() const { return mem_size_; }

 private:
  std::string mem_path_;
  size_t mem_size_;
  void* mem_ptr_;

#ifdef _WIN32
  HANDLE mem_file_;
#elif defined __linux__
  int mem_file_;
#endif
};  // Shmem

}  // namespace holodeck::core

#endif  // HOLODECK_CORE_SHMEM_H_
