/**
  * Shmem gives an interface to memory mapped files,
  * and makes it easier to write and read from them.
  */
#ifndef CORE_SHMEM_H_
#define CORE_SHMEM_H_

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
	  * @param bufferSize the number of bytes to allocated for the file.
	  * @param uuid a UUID for the Holodeck instance to allow multiple shared
	  * blocks on one machine.
	  */
  explicit Shmem(const std::string &name, int bufferSize,
                        const std::string &uuid);

  Shmem(const Shmem &) = delete;

  Shmem &operator=(const Shmem &) = delete;

  ~Shmem() = default;

  /**
    * getPtr
    * Gets a pointer to the start of the memory mapped file.
    * @return a void pointer to the start of the memory buffer.
    */
  void *getPtr() const { return _memPointer; }

  /**
    * size
    * Gets the size of the allocated memory mapped file.
    * @return the size in bytes of the file.
    */
  size_t size() const { return _memSize; }

 private:
  std::string _memPath;
  size_t _memSize;
  void *_memPointer;

#ifdef _WIN32
  HANDLE _memFile;
#elif defined __linux__
  int _memFile;
#endif
};  // Shmem

}  // namespace holodeck::core

#endif  // CORE_SHMEM_H_
