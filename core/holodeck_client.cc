#include "core/holodeck_client.h"

namespace holodeck::core {

HolodeckClient::HolodeckClient(const std::string &uuid) : _uuid(uuid) {
#ifdef _Win32
  auto _loadingSemaphore = OpenSemaphore(EVENT_ALL_ACCESS,
                                        false,
                                        *(LOADING_SEMAPHORE_PATH + UUID));
  ReleaseSemaphore(_loadingSemaphore, 1, NULL);
  this->LockingSemaphore1 = CreateSemaphore(NULL, 1, 1,
                                            *(SERVER_LOCK_PATH + UUID));
  this->LockingSemaphore2 = CreateSemaphore(NULL, 0, 1,
                                            *(CLIENT_LOCK_PATH + UUID));
#elif defined __linux__
  sem_unlink(SERVER_LOCK_PATH.c_str());
  sem_unlink(CLIENT_LOCK_PATH.c_str());

  auto LoadingSemaphore = sem_open((LOADING_SEMAPHORE_PATH + _uuid).c_str(),
                                   O_CREAT, 0777, 0);
  sem_post(LoadingSemaphore);

  _serverLock = sem_open((SERVER_LOCK_PATH + _uuid).c_str(),
                         O_CREAT, 0777, 1);
  _clientLock = sem_open((CLIENT_LOCK_PATH + _uuid).c_str(),
                         O_CREAT, 0777, 0);
  sem_unlink(LOADING_SEMAPHORE_PATH.c_str());
#endif
}

void *HolodeckClient::subscribeSensor(const std::string &agentName,
                                      const std::string &sensorKey,
                                      int bufferSize) {
  std::string key = makeKey(agentName, sensorKey);
  _sensors[key] = std::make_unique<Shmem>(key, bufferSize, _uuid);
  return _sensors[key]->getPtr();
}

void *HolodeckClient::subscribeActionSpace(const std::string &agentName,
                                           int bufferSize) {
  _actionSpaces[agentName] = std::make_unique<Shmem>(agentName,
                                                            bufferSize, _uuid);
  return _actionSpaces[agentName]->getPtr();
}

void *HolodeckClient::subscribeSetting(const std::string &settingName,
                                       int bufferSize) {
  _settings[settingName] = std::make_unique<Shmem>(settingName,
                                                          bufferSize);
  return _settings[settingName]->getPtr();
}

void HolodeckClient::acquire() {
  #ifdef _Win32
  WaitForSingleObject(this->LockingSemaphore1, INFINITE);
  #elif defined __linux__
  sem_wait(_clientLock);
  #endif
}

void HolodeckClient::release() {
  #ifdef _Win32
  ReleaseSemaphore(this->LockingSemaphore2, 1, NULL);
  #elif defined __linux__
  sem_post(_serverLock);
  #endif
}

bool HolodeckClient::isRunning() const {
  return false;
}

std::string HolodeckClient::makeKey(const std::string &agentName,
                                    const std::string &sensorName) const {
  return agentName + "_" + sensorName;
}

}  // namespace holodeck::core
