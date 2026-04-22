#include "mems.h"
#include <cstdio>

extern "C" void mems_cpp_init(void) {
  printf("C++ init\n");
}

extern "C" void mems_cpp_run(void) {
  printf("C++ run\n");
}
/*#include <stdio.h>
#include "protocol/protocol.h"
#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>
#include <time.h>


// Попробуем типичный формат (часто встречается в NuttX)
struct imu_sample_s
{
  float accel_x;
  float accel_y;
  float accel_z;

  float gyro_x;
  float gyro_y;
  float gyro_z;

  float temperature;
};

protocol proto;


extern "C" void mems_cpp_init(void){
  int fd = open("/dev/imu0", O_RDONLY);
  if (fd < 0)
  {
    printf("Failed to open /dev/imu0\n");
    return -1;
  }

  printf("IMU opened\n");

  // while (1)
  {


    struct imu_sample_s sample{};

    ssize_t n = read(fd, &sample, 12);
    if (n != sizeof(sample))
    {
      printf("read error: %d\n", (int)n);
      // continue;
    }

    printf("ACC:  %f %f %f\n",
           sample.accel_x,
           sample.accel_y,
           sample.accel_z);

    printf("GYRO: %f %f %f\n",
           sample.gyro_x,
           sample.gyro_y,
           sample.gyro_z);

    printf("TEMP: %f\n\n", sample.temperature);

    usleep(100000); // 100 ms
  }

  close(fd);


  return 0;
}
extern "C" void mems_cpp_run(void){
  proto.test_send();
}

*/

