#!/bin/bash

exec_path=$(pwd)

echo $exec_path

search_directory=""

if [[ $exec_path == *"sensor-network/scripts"* ]]; then
  search_directory="../build"
elif [[ $exec_path == *"sensor-network" ]]; then
  search_directory="./build"
else
  search_directory="/"
fi

platform=$exec_path/$(find "$search_directory" -name "platform-service")

sensor=$exec_path/$(find "$search_directory" -name "sensor-service")

echo $sensor
echo $platform

cpu_temp_args=" --service=cpu-temp --addr=0.0.0.0:50051"
cpu_usage_args=" --service=cpu-usage --addr=0.0.0.0:50052"

cpu_temp_sensor="$sensor$cpu_temp_args"
cpu_usage_sensor="$sensor$cpu_usage_args"

osascript -e "tell app \"Terminal\"
    do script \"$cpu_temp_sensor\"
end tell"

osascript -e "tell app \"Terminal\"
    do script \"$cpu_usage_sensor\"
end tell"

osascript -e "tell app \"Terminal\"
    do script \"$platform\"
end tell"