/**
  * HolodeckClient is a class which makes it easy to use SharedMemory objects
  * to communicate with a Holodeck Server.
  */
#ifndef HOLODECK_CORE_HOLODECK_CLIENT_H_
#define HOLODECK_CORE_HOLODECK_CLIENT_H_

#include <string>
#include <map>
#include <memory>

#include "core/shmem.h"

#ifdef _Win32
#include "AllowWindowsPlatformTypes.h"
#include "Windows.h"
#include "HideWindowsPlatformTypes.h"

const std::string kLoadingSemaphorePath = "Global\\HOLODECK_LOADING_SEM";
const std::string kServerLockPath = "Global\\HOLODECK_SEMAPHORE_SERVER";
const std::string kClientLockPath = "Global\\HOLODECK_SEMAPHORE_CLIENT";
#elif defined __linux__
#include <sys/mman.h>
#include <unistd.h>
#include <semaphore.h>

const std::string kLoadingSemaphorePath = "/HOLODECK_LOADING_SEM";
const std::string kServerLockPath = "/HOLODECK_SEMAPHORE_SERVER";
const std::string kClientLockPath = "/HOLODECK_SEMAPHORE_CLIENT";
#endif

namespace holodeck {

class HolodeckClient {
 public:
  /**
    * Default Constructor
    * When HolodeckClient is created, it creates the semaphores needed for
    * communicating with a HolodeckServer.
    * @param uuid a uuid for communicating with the relevant HolodecKServer.
    */
  explicit HolodeckClient(const std::string& uuid);

  HolodeckClient(const HolodeckClient&) = delete;

  HolodeckClient& operator=(const HolodeckClient&) = delete;

  ~HolodeckClient() = default;

  /**
    * SubscribeSensor
    * Subscribes a sensor. Creates a shared memory block for the sensor.
    * If a sensor already exists in this project with the same name and
    * same agent, the block is overwritten with the new sensor.
    * @param agent_name the name of the agent this sensor belongs to.
    * @param sensor_name the name of the sensor.
    * @param buffer_size the size to allocate in bytes.
    * @return a pointer to the start of the assigned memory. Returns nullptr
    * if it failed.
    */
  void* SubscribeSensor(const std::string& agent_name,
                        const std::string& sensor_key,
                        int buffer_size);

  /**
    * SubscribeActionSpace
    * Subscribes an action space. Creates a shared memory block for the
    * action space.
    * @param agent_name the agent to subscribe space for.
    * @param buffer_size the size of the buffer to allocate in bytes.
    * @return a pointer to the start of the assigned memory. Returns nullptr
    * if it failed.
    */
  void* SubscribeActionSpace(const std::string& agent_name, int buffer_size);

  /**
    * SubscribeSetting
    * Subscribes a setting. Creates a shared memory block for the setting.
    * @param setting_name the name of the setting.
    * @param buffer_size the size of the buffer to allocate in bytes.
    * @return a pointer to the start of the assigned memory.
    */
  void* SubscribeSetting(const std::string& setting_name, int buffer_size);

  /**
    * Acquire
    * Acquires the semaphore to allow the next tick to occur. Will block until
    * the server releases.
    */
  void Acquire();

  /**
    * Release
    * Releases the semaphore to allow the server to tick.
    */
  void Release();

 private:
  /**
    * MakeKey
    * Makes the key for subscribing sensors.
    * @param agent_name the name of the agent.
    * @param sensor_name the name of the sensor.
    * @return a key for this agent/sensor for putting it in the map.
    */
  std::string MakeKey(const std::string& agent_name,
                      const std::string& sensor_name) const;

  std::map<std::string, std::unique_ptr<Shmem>> sensors_;
  std::map<std::string, std::unique_ptr<Shmem>> action_spaces_;
  std::map<std::string, std::unique_ptr<Shmem>> settings_;

  std::string uuid_;

#ifdef _Win32
  HANDLE server_lock_;
  HANDLE client_lock_;
#elif defined __linux__
  sem_t* server_lock_;
  sem_t* client_lock_;
#endif
};  // class HolodeckClient

}  // namespace holodeck

#endif  // HOLODECK_CORE_HOLODECK_CLIENT_H_
