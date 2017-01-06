#!/bin/bash
output_file_name="results/timeseries_results.csv"


echo "Creating temp/ and results/ folder. Delete temp/ folder when tests are completed."
mkdir -p temp/ results/

echo "num_files,source_x_size,source_y_size,source_z_size,tiles_x_count,tiles_y_count,tiles_z_count,buffer_z_size,separate_time_us,virtual_time_us" > $output_file_name

n_frames_per_file=100;
frame_dimension_length=128;
frame_increase_step=128;
for i in {0..14}
do
    # Dimension of image (x and y)
    frame_dimension_length=$(($frame_dimension_length+$frame_increase_step));
    echo "Executing tests for frame size "$frame_dimension_length"x"$frame_dimension_length

    for num_files in {1..20}
    do
        echo "     Generating input files for num_files "$num_files

        # Generate the input files.
        ./bin/hdf5_test_prepare temp/virtual.hdf5 temp/source_%d.hdf5 $frame_dimension_length $frame_dimension_length $n_frames_per_file 1 1 $num_files 10

        # Repeat the measurments 10 times to have an average time.
        for repeat_measurment in {1..10}
        do
            echo "     Executing tests for num_files "$num_files

            # Execute tests and dump result into CSV file.
            ./bin/hdf5_test_execute temp/virtual.hdf5 temp/source_%d.hdf5 $frame_dimension_length $frame_dimension_length $n_frames_per_file 1 1 $num_files 10 >> $output_file_name
        done
    done
done
