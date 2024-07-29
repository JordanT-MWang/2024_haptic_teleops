from launch import LaunchDescription
from launch_ros.actions import Node
def generate_launch_description():
    return LaunchDescription([
        Node(
            package= 'comport',
            executable='serial_COM_node',
            output='screen'
        ),
    ])
