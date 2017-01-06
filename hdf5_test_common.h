#ifndef HDF5_TEST_COMMON
#define HDF5_TEST_COMMON

#if __STDC_VERSION__ >= 199901L
#define _XOPEN_SOURCE 600
#else
#define _XOPEN_SOURCE 500
#endif /* __STDC_VERSION__ */

#include <time.h>
#include "hdf5_common.h"

struct TestInputParameters{
    // Argument 1: Virtual dataset filename.
    char* virtual_dataset_filename;

    // Argument 2:Template to be used for filenames.
    char* data_filename_template;

    // Argument 3,4,5: Source image size.
    int source_x_size;
    int source_y_size;
    int source_z_size;

    // Argument 6,7,8: Virtual tiles count.
    int tiles_x_count;
    int tiles_y_count;
    int tiles_z_count;

    // Argument 9: Buffer Z size
    int buffer_z_size;

    // Calculated attributes.
    int num_files;
    hsize_t source_dataset_size[3];
    hsize_t virtual_dataset_tiles_count[3];
    hsize_t virtual_dataset_size[3];
};

// Parse input parameters for test.
int parse_test_parameters(int argc, char *argv[], struct TestInputParameters *input_parameters);

// Start nanosecond resolution timer.
struct timespec timer_start();
// Stop nanosecond resolution timer. Get microseconds elapsed back.
long timer_end(struct timespec start_time);

#endif
