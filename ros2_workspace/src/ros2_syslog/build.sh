#!/usr/bin/env bash
SCRIPT_DIRECTORY="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

find_ros2_workspace() {
  current_dir="${SCRIPT_DIRECTORY}"

  while [ "$current_dir" != "/" ]; do
    if [ -d "$current_dir/build" ] && \
       [ -d "$current_dir/install" ] && \
       [ -d "$current_dir/log" ] && \
       [ -d "$current_dir/src" ]; then
      echo "$current_dir"
      return 0
    fi

    current_dir=$(dirname "$current_dir")
  done

  echo "Error: ROS 2 workspace not found." >&2
  return 1
}

ros2_package=$(grep -oP '<name>\K[^<]*' "${SCRIPT_DIRECTORY}/package.xml" | tr -d '[:space:]')
dependencies=$(grep -oP '<depend>\K[^<]*' package.xml | tr '\n' ' ' | tr -s ' ' | sed 's/ $//')
ros2_workspace=$(find_ros2_workspace)

(
echo "Building: ${ros2_package}"
echo "  Using ros2_workspace: ${ros2_workspace}"
cd ${ros2_workspace}
for dependency in $dependencies; do
    colcon build --packages-select ${dependency}
done
set -x
cd ${ros2_workspace}
colcon build --packages-select ${ros2_package}
)