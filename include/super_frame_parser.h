#ifndef SUPER_FRAME_PARSER_H
#define SUPER_FRAME_PARSER_H

#ifndef NOT_COMPILING_WITH_MDK
#define NOT_COMPILING_WITH_MDK
#endif
// Tick Rate for Peanut, SF version 0x100
#define PEANUT_TICKS_PER_MICROSECOND 180.

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <superframe_parser/superframe_v2.h>

#include <rosbag/bag.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/Imu.h>
#include <sensor_msgs/PointCloud2.h>

#include <pcl_ros/point_cloud.h>
#include <pcl/point_types.h>

struct CameraIntrinsics
{
    size_t width, height; // Size of the image
    float focal_length[2]; // Focal length in unit of horizontal and vertical pixels
    float principal_point[2]; // Principal point location in pixels where (0,0) denotes the center of the upper left pixel
    float omega; // Field of view parameters in radians
    float max_angle; // Maximum angle corresponding to the maximum fov of the sensor in radians

};

/** Parses a given super frame file with intrinsic parameters for all cameras to ros messages.

    Currently extracted ros messages:
    sensor_msgs::Image for the small fisheye camera
    sensor_msgs::Image for the big narrow camera
    sensor_msgs::PointCloud2 for the depth camera (converted to a pointcloud) */
class SuperFrameParser
{
public:
    /** Opens the super frame file and already converts it to ros messages.
        Get the messages with getter functions.
        \param[in] super_frame_file string containing the path to the super frame file
        \param[in] depth_intrinsics name of the file for the depth instrinsic parameters
        \param[in] fisheye_intrinsics name of the file for the fisheye instrinsic parameters
        \param[in] narrow_intrinsics name of the file for the narrow instrinsic parameters */
    SuperFrameParser (const std::string &super_frame_file,
                      const std::string &depth_intrinsics = "depth_intrinsics.txt",
                      const std::string &fisheye_intrinsics = "fisheye_intrinsics.txt",
                      const std::string &narrow_intrinsics = "narrow_intrinsics.txt");

    /** Frees allocated memory. */
    ~SuperFrameParser ();

    /** Returs a pointer to the internal super frame structure */
    sf2_t* getSuperFrame () const { return super_frame_; }

    /** Returns a pointer to the small image message */
    sensor_msgs::ImagePtr getSmallImage () const { return small_img_msgs_; }

    /** Returns a pointer to the big image message */
    sensor_msgs::ImagePtr getBigImage () const { return big_img_msgs_; }
    /** Returns a pointer to the pointcoud message */
    sensor_msgs::PointCloud2Ptr getPointCloud () const { return point_cloud_msgs_; }
    /** Returns a pointer to the Imu message */
    sensor_msgs::ImuPtr getImu () const { return imu_msgs_; }

private:
    sf2_t *super_frame_;

    ros::Time time_now_;

    /////////         MESSAGES         //////////
    sensor_msgs::ImagePtr small_img_msgs_;
    sensor_msgs::ImagePtr big_img_msgs_;
    sensor_msgs::PointCloud2Ptr point_cloud_msgs_;
    sensor_msgs::ImuPtr imu_msgs_;

    ////////     CAMERA INTRINSICS    //////////
    CameraIntrinsics depth_intrinsics_;
    CameraIntrinsics fisheye_intrinsics_;
    CameraIntrinsics narrow_intrinsics_;


    /** parses the data from the superframe file to internal superframe structure */
    void parseSfFile (const std::string &file);

    /** parses the intrinsic parameters from the file to internal structure */

    void parseIntrinsicParams (const std::string &intrinsic_params, CameraIntrinsics &intrinsics);
    /** converts ticks got from the superframe header to seconds */
    double convertTicksToSeconds (const uint32_t super_frame_version, const TimeStamp& raw_timestamp);
    /** converts a sensor_msgs::Image to a pcl::PointCloud */
    void convertImageToPointCloud(const sensor_msgs::ImagePtr &depth_msg, const pcl::PointCloud<pcl::PointXYZ>::Ptr &cloud);

    ///////   FILL FUNCTIONS FOR THE MESSAGES   ////////
    void fillSmallImgMsg ();
    void fillBigImgMsg ();
    void fillPointCloudMsg ();
    void fillImuMsg ();

};

#endif // SUPER_FRAME_PARSER_H
