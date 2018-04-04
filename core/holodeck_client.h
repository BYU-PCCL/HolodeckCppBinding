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
#define LOADING_SEMAPHORE_PATH "Global\\HOLODECK_LOADING_SEM"
#define SERVER_LOCK_PATH "Global\\HOLODECK_SEMAPHORE_SERVER"
#define CLIENT_LOCK_PATH "Global\\HOLODECK_SEMAPHORE_CLIENT"
#include "AllowWindowsPlatformTypes.h"
#include "Windows.h"
#include "HideWindowsPlatformTypes.h"
#elif defined __linux__
constexpr std::string LOADING_SEMAPHORE_PATH = "/HOLODECK_LOADING_SEM";
constexpr std::string SERVER_LOCK_PATH = "/HOLODECK_SEMAPHORE_SERVER";
constexpr std::string CLIENT_LOCK_PATH = "/HOLODECK_SEMAPHORE_CLIENT";

#include <sys/mman.h>
#include <unistd.h>
#include <semaphore.h>

#endif

namespace holodeck::core {

class HolodeckClient {
 public:
  /**
    * Default Constructor
    * When HolodeckClient is created, it creates the semaphores needed for
    * communicating with a HolodeckServer.
    * @param uuid a uuid for communicating with the relevant HolodecKServer.
    */
  explicit HolodeckClient(const std::string &uuid);

  HolodeckClient(const HolodeckClient &) = delete;

  HolodeckClient &operator=(const HolodeckClient &) = delete;

  ~UHolodeckClient() = default;

  /**
    * SubscribeSensor
    * Subscribes a sensor. Creates a shared memory block for the sensor.
    * If a sensor already exists in this project with the same name and
    * same agent, the block is overwritten with the new sensor.
    * @param agentName the name of the agent this sensor belongs to.
    * @param sensorKey the name of the sensor.
    * @param bufferSize the size to allocate in bytes.
    * @return a pointer to the start of the assigned memory.
    */
  void *subscribeSensor(const std::string &agentName,
                        const std::string &sensorKey,
                        int bufferSize);

  /**
    * subscribeActionSpace
    * Subscribes an action space. Creates a shared memory block for the
    * action space.
    * @param agentName the agent to subscribe space for.
    * @param bufferSize the size of the buffer to allocate in bytes.
    * @return a pointer to the start of the assigned memory.
    */
  void *subscribeActionSpace(const std::string &agentName, int bufferSize);

  /**
    * subscribeSetting
    * Subscribes a setting. Creates a shared memory block for the setting.
    * @param settingName the name of the setting.
    * @param bufferSize the size of the buffer to allocate in bytes.
    * @return a pointer to the start of the assigned memory.
    */
  void *subscribeSetting(const std::string &settingName, int bufferSize);

  /**
    * acquire
    * Acquires the mutex to allow the next tick to occur. Will block until
    * the client releases.
    */
  void acquire();

  /**
    * release
    * Releases the mutex to allow the client to tick.
    */
  void release();

  /**
    * isRunning
    * Gets whether the server is running.
    * @return true if the server is running.
    */
  bool isRunning() const;

 private:
  /**
    * makeKey
    * Makes the key for subscribing sensors.
    * @param agentName the name of the agent.
    * @param sensorName the name of the sensor.
    * @return a key for this agent/sensor for the map.
    */
  std::string makeKey(const std::string &agentName,
                      const std::string &sensorName) const;

  std::map<std::string, std::unique_ptr<Shmem>> _sensors;
  std::map<std::string, std::unique_ptr<Shmem>> _actionSpaces;
  std::map<std::string, std::unique_ptr<Shmem>> _settings;

  std::string _uuid;

#ifdef _Win32
  HANDLE _serverLock;
  HANDLE _clientLock;
#elif defined __linux__
  sem_t *_serverLock;
  sem_t *_clientLock;
#endif
};  // class HolodeckClient

}  // namespace holodeck::core

#endif  // HOLODECK_CORE_HOLODECK_CLIENT_H_
