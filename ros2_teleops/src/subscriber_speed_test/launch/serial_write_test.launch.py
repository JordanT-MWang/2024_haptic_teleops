from launch import LaunchDescription
from launch_ros.actions import Node
def generate_launch_description():
    return LaunchDescription([
        Node(
            package= 'subscriber_speed_test',
            executable='simple_write_subscriber',
            output='screen'
        ),
    ])
