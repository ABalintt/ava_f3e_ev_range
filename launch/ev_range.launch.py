from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    """Launch both the EV sensor simulator node and the range calculator node."""
    return LaunchDescription([
        Node(
            package='ava_f3e_ev_range',
            executable='ev_sensor_node',
            name='ev_sensor_node',
            output='screen'
        ),
        Node(
            package='ava_f3e_ev_range',
            executable='range_calc_node',
            name='range_calc_node',
            output='screen'
        )
    ])