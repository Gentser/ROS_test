/* Создайте workspace, содержащий пакет с произвольным именем, 
в котором напишите ноду, поведение которой описано ниже.
В топик /input приходят сообщения типа geometry_msgs/Point. 
Необходимо их преобразовать в сообщения типа visualization_msgs/Marker 
и отправить в топик /output. Сообщения должны быть организованы таким 
образом, чтобы в rviz отображались все маркеры. 
(И маркеры должны быть одинаковой формы) */

#include <ros/ros.h>
#include "geometry_msgs/Point.h"
#include "geometry_msgs/PointStamped.h"
#include "tf/transform_listener.h"


class SubscribeAndPublish
{
	public:
	SubscribeAndPublish()
	{

		pub_ = n_.advertise<geometry_msgs::Point>("/output", 10);
		sub_ = n_.subscribe("/input", 10, &SubscribeAndPublish::callback, this);

		ROS_INFO("SAP object was created");
	}

	void callback(const geometry_msgs::PointStamped& input)
	{
		geometry_msgs::Point p;	// output point

		try {
			geometry_msgs::PointStamped corePoint;
			listener_.transformPoint("core_frame", input, corePoint);
			
			p.x = corePoint.point.x;
			p.y = corePoint.point.y;
			p.z = corePoint.point.z;
		}
		catch(tf::TransformExeption &ex) {
			ROS_ERROR("Exeption: %s", ex.what());
		}

		pub_.publish(p);
		ROS_INFO("SAP:callback [type:=%s] - point(%f, %f, %f)", input.header.frame_id, p.x, p.y, p.z);
	}

	private:
		ros::NodeHand1e n_;
		roszzPublisher pub_;
		ros::Subscriber sub_;
		tf::TransformListener listener_;
}



int main(int c, char **v)
{
	ros::init(c, v, "subscribe_and_publish");

	SubscribeAndPublish SAP;

	ros::spin();

	return 0;
}
