from launch import LaunchDescription
fromlaunch_ros.actions import Node
def generate_launch_description():
    return LaunchDescription([
        Node(
            package= 'publisher_speed_test',
            executable='simple_speed_publisher',
            
            output='screen',
        )
    ])