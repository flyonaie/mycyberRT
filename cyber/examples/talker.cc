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

#include "cyber/examples/proto/examples.pb.h"

#include "cyber/cyber.h"
#include "cyber/time/rate.h"
#include "cyber/time/time.h"

using apollo::cyber::Rate;
using apollo::cyber::Time;
using apollo::cyber::examples::proto::Chatter;

int main(int argc, char *argv[]) {
  // init cyber framework
  std::cout << "1234566677" << std::endl;
  AINFO << "0000000.";
  apollo::cyber::Init(argv[0]);
  printf("INIIIIIIIIIIIIIIIIIIIIII\n");
  // AINFO << "111111100000.";
  // create talker node
  auto talker_node = apollo::cyber::CreateNode("talker");
  printf("apollo::cyber::CreateNode.........\n");
  AINFO << "22222200000.";
   printf("apollo::cyber::CreateNode22222222.........\n");
  // create talker
  auto talker = talker_node->CreateWriter<Chatter>("channel/chatter");
  printf("apollo::cyber::CreateNode3333333333333.........\n");
  AINFO << "3333333300000.";
  printf("apollo::cyber::CreateNode44444444444444.........\n");
  Rate rate(1.0);
  printf("apollo::cyber::CreateNode55555555555555555.........\n");
  uint64_t seq = 0;
  AINFO << "4444444000000.";
  printf("apollo::cyber::CreateNode666666666666666666662.........\n");
  while (apollo::cyber::OK()) {
    
    auto msg = std::make_shared<Chatter>();
    msg->set_timestamp(Time::Now().ToNanosecond());
    msg->set_lidar_timestamp(Time::Now().ToNanosecond());
    msg->set_seq(seq);
    msg->set_content("Hello, apollo!");
    talker->Write(msg);
    AINFO << "talker sent a message! No. " << seq;
    seq++;
    printf("apollo::cyber::CreateNodeseq = %d\n", seq);
    rate.Sleep();
  }
  return 0;
}

int main1() {
  // init cyber framework
  std::cout << "1234566677222222222" << std::endl;
  // AINFO << "0000000.";
  return 0;
}
