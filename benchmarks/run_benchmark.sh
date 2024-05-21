#!/bin/bash

executable=./render.out
num_runs=5
output_file=./benchmarks/benchmark_results.csv

echo "num_processors,run,topology,load_balanced,time" >$output_file

processors=(1 2 4 6 8 16)
configurations=("grid" "grid_nlb" "hypercube" "hypercube_nlb" "linear")

for np in "${processors[@]}"; do
	for config in "${configurations[@]}"; do
		for run in $(seq 1 $num_runs); do
			echo "Running with $np processors, configuration $config, run $run"
			output=$(mpirun --oversubscribe -np $np $executable $config no)
			time=$(echo "$output" | grep "Elapsed time" | awk '{print $3}')
			topology="grid"
			load_balanced="true"
			if [ "$config" == "grid_nlb" ]; then
				topology="grid"
				load_balanced="false"
			elif [ "$config" == "hypercube" ]; then
				topology="hypercube"
				load_balanced="true"
			elif [ "$config" == "hypercube_nlb" ]; then
				topology="hypercube"
				load_balanced="false"
			elif [ "$config" == "linear" ]; then
				topology="linear"
				load_balanced="false"
			fi
			echo "$np,$run,$topology,$load_balanced,$time" >>$output_file
		done
	done
done

echo "Benchmarking completed. Results saved to $output_file."
