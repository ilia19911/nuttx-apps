/****************************************************************************
 * apps/canutils/canlib/canlib_getloopback.c
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <sys/ioctl.h>
#include <debug.h>
#include <errno.h>

#include <nuttx/can/can.h>

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: canlib_getloopback
 *
 * Description:
 *   Wrapper for CANIOC_GET_CONNMODES.
 *
 * Input Parameter:
 *   fd       - file descriptor of an opened can device
 *   loopback - pointer to a buffer to store the current loopback mode state.
 *
 * Returned Value:
 *   Zero (OK) is returned on success.  Otherwise -1 (ERROR)
 *   is returned with the errno variable set to indicate the
 *   nature of the error.
 *
 ****************************************************************************/

int canlib_getloopback(int fd, FAR bool *loopback)
{
  int ret;
  struct canioc_connmodes_s connmodes;

  ret = ioctl(fd, CANIOC_GET_CONNMODES, (unsigned long)&connmodes);
  if (ret != OK)
    {
      canerr("CANIOC_GET_CONNMODES failed, errno=%d\n", errno);
      return ret;
    }

  *loopback = connmodes.bm_loopback;
  return ret;
}
