# `ava_f3e_ev_range` package
ROS 2 C++ package for electric vehicle driving range estimation.  [![Static Badge](https://img.shields.io/badge/ROS_2-Humble-34aec5)](https://docs.ros.org/en/humble/)
## Packages and build

It is assumed that the workspace is `~/ros2_ws/`.

### Clone the packages
``` r
cd ~/ros2_ws/src
```
``` r
git clone https://github.com/ABalintt/ava_f3e_ev_range
```

### Build ROS 2 packages
``` r
cd ~/ros2_ws
```
``` r
colcon build --packages-select ava_f3e_ev_range --symlink-install
```

<details>
<summary> Don't forget to source before ROS commands.</summary>

``` bash
source ~/ros2_ws/install/setup.bash
```
</details>

``` r
ros2 launch ava_f3e_ev_range ev_range.launch.py
```

## Node Graph

```mermaid
graph LR
sensor([ /ev_sensor_node]):::red --> energy[ /battery_energy<br/>std_msgs/Float32]:::light
sensor --> cons[ /avg_consumption<br/>std_msgs/Float32]:::light
energy --> calc([ /range_calc_node]):::red
cons --> calc
calc --> range[ /estimated_range<br/>std_msgs/Float32]:::light

classDef light fill:#34aec5,stroke:#152742,stroke-width:2px,color:#152742
classDef dark fill:#152742,stroke:#34aec5,stroke-width:2px,color:#34aec5
classDef white fill:#ffffff,stroke:#152742,stroke-width:2px,color:#152742
classDef red fill:#ef4638,stroke:#152742,stroke-width:2px,color:#fff
```

## Screenshot

![Output](screenshot.png)