#include "core/holodeck_client.h"

namespace holodeck::core {

HolodeckClient::HolodeckClient(const std::string& uuid) : uuid_(uuid) {
#ifdef _Win32
  // TODO(joshgreaves): Check this code
  auto loading_semaphore = OpenSemaphore(EVENT_ALL_ACCESS,
                                        false,
                                        *(kLoadingSemaphorePath + uuid_));
  ReleaseSemaphore(loading_semaphore, 1, NULL);
  server_lock_ = CreateSemaphore(NULL, 1, 1,
                                 *(kServerLockPath + uuid_));
  client_lock_ = CreateSemaphore(NULL, 0, 1,
                                 *(kClientLockPath + uuid_));
#elif defined __linux__
  sem_unlink(kServerLockPath.c_str());
  sem_unlink(kClientLockPath.c_str());

  auto loading_semaphore = sem_open((kLoadingSemaphorePath + uuid_).c_str(),
                                    O_CREAT, 0777, 0);
  sem_post(loading_semaphore);

  server_lock_ = sem_open((kServerLockPath + uuid_).c_str(),
                          O_CREAT, 0777, 1);
  client_lock_ = sem_open((kClientLockPath + uuid_).c_str(),
                          O_CREAT, 0777, 0);
  sem_unlink(kLoadingSemaphorePath.c_str());
#endif
}

void* HolodeckClient::SubscribeSensor(const std::string& agent_name,
                                      const std::string& sensor_key,
                                      int buffer_size) {
  std::string key = MakeKey(agent_name, sensor_key);
  sensors_[key] = std::make_unique<Shmem>(key, buffer_size, uuid_);
  return sensors_[key]->GetPtr();
}

void* HolodeckClient::SubscribeActionSpace(const std::string& agent_name,
                                           int buffer_size) {
  action_spaces_[agent_name] = std::make_unique<Shmem>(agent_name,
                                                       buffer_size, uuid_);
  return action_spaces_[agent_name]->GetPtr();
}

void* HolodeckClient::SubscribeSetting(const std::string& setting_name,
                                       int buffer_size) {
  settings_[setting_name] = std::make_unique<Shmem>(setting_name,
                                                    buffer_size);
  return settings_[setting_name]->GetPtr();
}

void HolodeckClient::Acquire() {
#ifdef _Win32
  // TODO(joshgreaves): Check this code
  WaitForSingleObject(client_lock_, INFINITE);
#elif defined __linux__
  sem_wait(client_lock_);
#endif
}

void HolodeckClient::Release() {
#ifdef _Win32
  // TODO(joshgreaves): Check this code
  ReleaseSemaphore(server_lock_, 1, NULL);
#elif defined __linux__
  sem_post(server_lock_);
#endif
}

std::string HolodeckClient::MakeKey(const std::string& agent_name,
                                    const std::string& sensor_name) const {
  return agent_name + "_" + sensor_name;
}

}  // namespace holodeck::core
