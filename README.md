Tango output to rosbag files 
=================
This package contains scripts and nodes to convert output files created by Tango Mapper to rosbag files storing most of the messages. Different scripts and programs extract different information from the mapper log files. The python script 'combined_parse_tango_output.py' executes all the scripts sequentially and combines the results into one single bagfile. The /tmp folder is used to store the intermediate results. 

Usage:
        
        combined_parse_tango_output.py /location/to/files outputbag.bag

'/location/to/files' must be of the kind produced by Tango mapper. The superframes subfolder should be there, as well. The file outputbag.bag will have all the messages. 

Please note that:

 - The images from the narrow field camera are black and white only
 - The messages in the bag file is not ordered. Sorting can be done afterward.
 - This code requires the file superframe_v2.h from the Google Tango SDK.  Due to licensing issues we cannot redistribute this file. It should be copied in the include/superframe_parser folder.
 - The ROS package is named "superframe_parser", so best clone into a directory with that name.

A video showing a recorded dataset is available at http://youtu.be/c2sGjgMlnEU
