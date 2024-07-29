from launch import LaunchDescription
from launch_ros.actions import Node
def generate_launch_description():
    return LaunchDescription([
        Node(
            package= 'PD_pkg',
            executable='pd_controller_node',
            output='screen'
        ),
    ])