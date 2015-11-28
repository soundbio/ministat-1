#!/usr/bin/env python
# license removed for brevity
import rospy
from std_msgs.msg import Float32, Int32

sumdata = 0.0
count = 0

def callback(data):
    global count, sumdata
    sumdata += data.data
    count += 1
    
def listener():
    global count, sumdata
    pub = rospy.Publisher('avg', Float32, queue_size=10)
 
    # In ROS, nodes are uniquely named. If two nodes with the same
    # node are launched, the previous one is kicked off. The
    # anonymous=True flag means that rospy will choose a unique
    # name for our 'listener' node so that multiple listeners can
    # run simultaneously.
    rospy.init_node('avg')
    rate = rospy.Rate(10) # 10hz

    rospy.Subscriber("thermistor", Int32, callback)
    while not rospy.is_shutdown():
        if count >= 20:
           pub.publish(Float32(sumdata/count))
           count = 0
           sumdata = 0
        rate.sleep()

    # spin() simply keeps python from exiting until this node is stopped
    rospy.spin()

if __name__ == '__main__':
    listener()
