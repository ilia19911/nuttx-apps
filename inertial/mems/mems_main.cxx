//***************************************************************************
// apps/examples/helloxx/helloxx_main.cxx
//
// SPDX-License-Identifier: Apache-2.0
//
// Licensed to the Apache Software Foundation (ASF) under one or more
// contributor license agreements.  See the NOTICE file distributed with
// this work for additional information regarding copyright ownership.  The
// ASF licenses this file to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance with the
// License.  You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
// WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
// License for the specific language governing permissions and limitations
// under the License.
//
//***************************************************************************

//***************************************************************************
// Included Files
//***************************************************************************

#include "fcntl.h"

#include <nuttx/config.h>

#include "nuttx/sensors/lsm6dsv.h"
#include "protocol.hxx"

#include <cstdio>
#include <debug.h>

//***************************************************************************
// Definitions
//***************************************************************************
// Configuration ************************************************************

// Debug ********************************************************************
// Non-standard debug that may be enabled just for testing the constructors

#ifdef CONFIG_DEBUG_FEATURES
#define CONFIG_DEBUG_CXX
#endif

#ifdef CONFIG_DEBUG_CXX
#define cxxinfo _info
#else
#define cxxinfo(x...)
#endif
#include "protocol.hxx"

int fd_imu[4];

static struct lsm6dsv_sensor_data_s ReadImuData(int fd) {
  struct lsm6dsv_sensor_data_s data;
  int cnt = read(fd, &data, sizeof(struct lsm6dsv_sensor_data_s));
  if (cnt == sizeof(struct lsm6dsv_sensor_data_s)) {
  }
  return data;
}
static void onPacket(uint16_t type, const uint8_t *data, size_t len,
                     void *userdata) {
  SimpleProtocol *proto = static_cast<SimpleProtocol *>(userdata);
  switch (type) {
  case 1001:
    struct lsm6dsv_sensor_data_s sensors_data[4];
    for (int i = 0; i < 4; i++) {
      sensors_data[i] = ReadImuData(fd_imu[i]);
    }
    proto->send(type, (uint8_t *)sensors_data, sizeof(sensors_data));
    break;
  }
}

extern "C" int main(int argc, FAR char *argv[]) {
  printf("MEMS App Started\n");
  SimpleProtocol proto("/dev/ttyS1");

  if (!proto.open()) {
    printf("Failed to open /dev/ttyS0\n");
    return -1;
  }

  for (int i = 0; i < 4; i++) {
    char name[] = "/dev/imuX";
    name[8] = '0' + i;
    fd_imu[i] = open(name, O_RDONLY);
    if (fd_imu[i] < 0) {
      printf("Failed to open /dev/imu0\n");
      return -1;
    }
  }

  proto.setCallback(onPacket, &proto);
  proto.runLoop(); // бесконечный цикл

  return 0;
}