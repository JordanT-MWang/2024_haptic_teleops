from launch import LaunchDescription
from launch_ros.actions import Node
def generate_launch_description():
    return LaunchDescription([
        Node(
            package= 'comport',
            executable='serial_read_COM_node',
            output='screen'
        ),
        Node(
            package= 'comport',
            executable='serial_write_COM_node',
            output='screen'
        ),
    ])
