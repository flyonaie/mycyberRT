/******************************************************************************
 * Copyright 2018 The Apollo Authors. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/

#ifndef CYBER_TRANSPORT_TRANSMITTER_SHM_TRANSMITTER_H_
#define CYBER_TRANSPORT_TRANSMITTER_SHM_TRANSMITTER_H_

#include <cstring>
#include <iostream>
#include <memory>
#include <string>

#include "cyber/transport/shm/notifier_factory.h"
#include "cyber/transport/shm/readable_info.h"
#include "cyber/transport/shm/segment_factory.h"

#define AERROR \
  printf("AERROR = %d\n", __FILE__)

// printf( << "acquire block failed.";

//   std::cout << "not enable." << std::endl;
// #define ADEBUG_MODULE(module) \
//   printf("111123344565= %d\n", __FILE__); \
//   VLOG(4) << LEFT_BRACKET << module << RIGHT_BRACKET << "[DEBUG] "

namespace apollo {
namespace cyber {
namespace transport {

class MessageInfo {
 public:
  MessageInfo();
  MessageInfo(const Identity& sender_id, uint64_t seq_num);
  MessageInfo(const Identity& sender_id, uint64_t seq_num,
              const Identity& spare_id);
  MessageInfo(const MessageInfo& another);
  virtual ~MessageInfo();

  MessageInfo& operator=(const MessageInfo& another);
  bool operator==(const MessageInfo& another) const;
  bool operator!=(const MessageInfo& another) const;

  bool SerializeTo(std::string* dst) const;
  bool SerializeTo(char* dst, std::size_t len) const;
  bool DeserializeFrom(const std::string& src);
  bool DeserializeFrom(const char* src, std::size_t len);

  // getter and setter
  const Identity& sender_id() const { return sender_id_; }
  void set_sender_id(const Identity& sender_id) { sender_id_ = sender_id; }

  uint64_t channel_id() const { return channel_id_; }
  void set_channel_id(uint64_t channel_id) { channel_id_ = channel_id; }

  uint64_t seq_num() const { return seq_num_; }
  void set_seq_num(uint64_t seq_num) { seq_num_ = seq_num; }

  const Identity& spare_id() const { return spare_id_; }
  void set_spare_id(const Identity& spare_id) { spare_id_ = spare_id; }

  static const std::size_t kSize;

 private:
  Identity sender_id_;
  uint64_t channel_id_ = 0;
  uint64_t seq_num_ = 0;
  Identity spare_id_;
};

template <typename M>
class ShmTransmitter {
 public:
  using MessagePtr = std::shared_ptr<M>;

  explicit ShmTransmitter(uint64_t channel_id);
  virtual ~ShmTransmitter();

  void Enable() override;
  void Disable() override;

  // bool Transmit(const MessagePtr& msg, const MessageInfo& msg_info) override;

 private:
  bool Transmit(const M& msg, const MessageInfo& msg_info);

  SegmentPtr segment_;
  uint64_t channel_id_;
  uint64_t host_id_;
  NotifierPtr notifier_;
};

template <typename M>
ShmTransmitter<M>::ShmTransmitter(uint64_t channel_id)
      segment_(nullptr),
      channel_id_(channel_id),
}

template <typename M>
ShmTransmitter<M>::~ShmTransmitter() {
  Disable();
}

template <typename M>
void ShmTransmitter<M>::Enable() {
  if (this->enabled_) {
    return;
  }

  segment_ = SegmentFactory::CreateSegment(channel_id_);
  notifier_ = NotifierFactory::CreateNotifier();
  this->enabled_ = true;
}

template <typename M>
void ShmTransmitter<M>::Disable() {
  if (this->enabled_) {
    segment_ = nullptr;
    notifier_ = nullptr;
    this->enabled_ = false;
  }
}

// template <typename M>
// bool ShmTransmitter<M>::Transmit(const MessagePtr& msg,
//                                  const MessageInfo& msg_info) {
//   return Transmit(*msg, msg_info);
// }

template <typename M>
bool ShmTransmitter<M>::Transmit(const M& msg, const MessageInfo& msg_info) {
  if (!this->enabled_) {
    // ADEBUG << "not enable.";
    AERROR;
    return false;
  }

  WritableBlock wb;
  std::size_t msg_size = message::ByteSize(msg);
  if (!segment_->AcquireBlockToWrite(msg_size, &wb)) {
    AERROR;
    return false;
  }

  ADEBUG << "block index: " << wb.index;
  if (!message::SerializeToArray(msg, wb.buf, static_cast<int>(msg_size))) {
    AERROR << "serialize to array failed.";
    segment_->ReleaseWrittenBlock(wb);
    return false;
  }
  wb.block->set_msg_size(msg_size);

  char* msg_info_addr = reinterpret_cast<char*>(wb.buf) + msg_size;
  if (!msg_info.SerializeTo(msg_info_addr, MessageInfo::kSize)) {
    AERROR;
    segment_->ReleaseWrittenBlock(wb);
    return false;
  }
  wb.block->set_msg_info_size(MessageInfo::kSize);
  segment_->ReleaseWrittenBlock(wb);

  ReadableInfo readable_info(host_id_, wb.index, channel_id_);

  // ADEBUG << "Writing sharedmem message: "
  //        << common::GlobalData::GetChannelById(channel_id_)
  //        << " to block: " << wb.index;
  AERROR;
  return notifier_->Notify(readable_info);
}

}  // namespace transport
}  // namespace cyber
}  // namespace apollo

#endif  // CYBER_TRANSPORT_TRANSMITTER_SHM_TRANSMITTER_H_
