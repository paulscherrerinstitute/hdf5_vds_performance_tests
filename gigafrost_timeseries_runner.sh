#!/bin/bash
output_file_name="results/gigafrost_timeseries_results.csv"


echo "Creating temp/ and results/ folder. Delete temp/ folder when tests are completed."
mkdir -p temp/ results/

echo "num_files,source_x_size,source_y_size,source_z_size,tiles_x_count,tiles_y_count,tiles_z_count,buffer_z_size,separate_time_us,virtual_time_us" > $output_file_name

n_frames_per_file=100;
num_files=20;
frame_dimension_length=2048;
buffer_z_size=1;

echo "Generating input files for num_files "$num_files

# Generate the input files.
./bin/hdf5_test_prepare temp/virtual.hdf5 temp/source_%d.hdf5 $frame_dimension_length $frame_dimension_length $n_frames_per_file 1 1 $num_files $buffer_z_size

for buffer_z_size in 1 5 10 20 25 50 100
do
    # Repeat the measurments 10 times to have an average time.
    for repeat_measurment in {1..10}
    do
        echo "     Executing tests for num_files "$num_files" and buffer z size "$buffer_z_size

        # Execute tests and dump result into CSV file.
        ./bin/hdf5_test_execute temp/virtual.hdf5 temp/source_%d.hdf5 $frame_dimension_length $frame_dimension_length $n_frames_per_file 1 1 $num_files $buffer_z_size >> $output_file_name
    done
done
