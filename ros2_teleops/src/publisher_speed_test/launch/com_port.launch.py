from launch import LaunchDescription
from launch_ros.actions import Node
def generate_launch_description():
    return LaunchDescription([
        Node(
            package= 'publisher_speed_test',
            executable='simple_com_publisher',
            output='screen'
        ),
    ])