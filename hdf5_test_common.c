#include "hdf5_test_common.h"
#include <stdlib.h>

int parse_test_parameters(int argc, char *argv[], struct TestInputParameters *input_parameters){
    if (argc != 10){
        printf("\nUsage: %s [virtual_dataset_filename] [source_files_template] [source_x_size] [source_y_size] [source_z_size] [tiles_x_count] [tiles_y_count] [tiles_z_count] [n_frames_buffer]\n\n", argv[0]);
        printf("* virtual_dataset_filename: Filename of the virtual dataset to read.\n");
        printf("* source_files_template: Template for reading HDF5 files used as source for the virual dataset. Needs an integer parameter.\n");
        printf("\tExample: /home/local/temp/storage_%%d.hdf5\n");
        printf("* source_x_size: X size of each frame in source file.\n");
        printf("* source_y_size: Y size of each frame in source file.\n");
        printf("* source_z_size: Number of frames in each source file.\n");
        printf("* tiles_x_count: Number of source frames to combine on X axis in virtual dataset.\n");
        printf("* tiles_y_count: Number of source frames to combine on Y axis in virtual dataset.\n");
        printf("* tiles_z_count: Number of source files to combine on Y axis.\n");
        printf("* n_frames_buffer: Number of frames to read into buffer. (source_z_size %% n_frames_buffer) must be zero.\n");
        printf("\nExample: %s virtual.hdf5 source_%%d.hdf5 2 2 2 2 2 1 1\n\n", argv[0]);
        return -1;
    }

    // Argument 1: Virtual dataset filename.
    input_parameters->virtual_dataset_filename = argv[1];

    // Argument 2:Template to be used for filenames.
    input_parameters->data_filename_template = argv[2];

    // Argument 3,4,5: Source image size.
    input_parameters->source_x_size = atoi(argv[3]);
    input_parameters->source_y_size = atoi(argv[4]);
    input_parameters->source_z_size = atoi(argv[5]);

    // Argument 6,7,8: Virtual tiles count.
    input_parameters->tiles_x_count = atoi(argv[6]);
    input_parameters->tiles_y_count = atoi(argv[7]);
    input_parameters->tiles_z_count = atoi(argv[8]);

    // Argument 9: Buffer Z size
    input_parameters->buffer_z_size = atoi(argv[9]);
    if ((input_parameters->source_z_size) % input_parameters->buffer_z_size) {
        printf("The source_z_size must to be divisable by n_frames_buffer.\n");
        return -1;
    }

    // Number of individual files.
    input_parameters->num_files = input_parameters->tiles_x_count * input_parameters->tiles_y_count * input_parameters->tiles_z_count;

    // Size of source dataset.
    input_parameters->source_dataset_size[2] = input_parameters->source_x_size;
    input_parameters->source_dataset_size[1] = input_parameters->source_y_size;
    input_parameters->source_dataset_size[0] = input_parameters->source_z_size;

    // Number of tiles in virtual dataset.
    input_parameters->virtual_dataset_tiles_count[2] = input_parameters->tiles_x_count;
    input_parameters->virtual_dataset_tiles_count[1] = input_parameters->tiles_y_count;
    input_parameters->virtual_dataset_tiles_count[0] = input_parameters->tiles_z_count;

    // Size of virtual dataset.
    input_parameters->virtual_dataset_size[2] = input_parameters->source_x_size * input_parameters->tiles_x_count;
    input_parameters->virtual_dataset_size[1] = input_parameters->source_y_size * input_parameters->tiles_y_count;
    input_parameters->virtual_dataset_size[0] = input_parameters->source_z_size * input_parameters->tiles_z_count;

    return 0;
}

struct timespec timer_start(){
    struct timespec start_time;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_time);
    return start_time;
}

long timer_end(struct timespec start_time){
    struct timespec end_time;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_time);

    long diff_in_seconds = end_time.tv_sec - start_time.tv_sec;
    long diff_in_nano_seconds = end_time.tv_nsec - start_time.tv_nsec;

    // If the difference in nanoseconds is less than 1, we already changed the second from the epoch.
    // Substract 1 second and add the equivalent number of nanoseconds.
    if (diff_in_nano_seconds<0){
        diff_in_seconds -= 1;
        diff_in_nano_seconds += 1000000000;
    }

    // Convert time to micro seconds.
    long diff_in_micro_seconds = ((diff_in_seconds*1000000000) + diff_in_nano_seconds) / 1000;

    return diff_in_micro_seconds;
}
