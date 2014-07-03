/**
* 
* \mainpage BreezySLAM: Simple, efficient SLAM in C++
*
* breezyslam.hpp - C++ header for BreezySLAM classes
*
* Copyright (C) 2014 Simon D. Levy

* This code is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as 
* published by the Free Software Foundation, either version 3 of the 
* License, or (at your option) any later version.
* 
* This code is distributed in the hope that it will be useful,     
* but WITHOUT ANY WARRANTY without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU Lesser General Public License 
* along with this code.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <math.h>

#include <iostream>
#include <vector>
using namespace std; 

/**
*Supports essential CoreSLAM operations. 
*
*Based on
<pre>
* @inproceedings{coreslam-2010,  
*   author    = {Bruno Steux and Oussama El Hamzaoui}, 
*   title     = {CoreSLAM: a SLAM Algorithm in less than 200 lines of C code},  
*   booktitle = {11th International Conference on Control, Automation,   
*                Robotics and Vision, ICARCV 2010, Singapore, 7-10  
*                December 2010, Proceedings},  
*   pages     = {1975-1979},  
*   publisher = {IEEE},  
*   year      = {2010}
* }
* </pre>
*/
class CoreSLAM 
{
    
public:
    
/**
* Creates a CoreSLAM object.
* @param laser a Laser object containing parameters for your Lidar equipment
* @param map_size_pixels the size of the desired map (map is square)
* @param map_scale_pixels_per_meter the scale of the desired map,
* @param random_seed a positive seed for the pseudorandom number generator used in the particle filter
* @return a new CoreSLAM object
*/
CoreSLAM(class Laser & laser, int map_size_pixels, double map_scale_pixels_per_meter, 
    int random_seed);

    /**
    * Creates a CoreSLAM object.  Current time in seconds is used as the seed for
    * pseudorandom number generator.
    */
    CoreSLAM(class Laser & laser, int map_size_pixels, double map_scale_pixels_per_meter);
    
    ~CoreSLAM(void);
    
    /**
    * Retrieves the current map.
    * @param mapbytes a byte array big enough to hold the map (map_size_pixels * map_size_pixels)
    */
    void getmap(unsigned char * mapbytes);
    
    /**
    * Retrieves the current position cloud generated by the particle filter.
    * @param cloud a vector of Position objects representing the current cloud
    */
    void getcloud(vector<class Position>  & cloud);
    
    /**
    * Sets the current map.
    * @param mapbytes a byte array of size map_size_pixels * map_size_pixels containing the map in
    * row-major order
    */
    void setmap(unsigned char * mapbytes);
    
    /**
    * Updates the CoreSLAM object using the current Lidar scan, with velocities for odometry.
    * @param scanvals Lidar scan values, whose count is specified in the <tt>scan_size</tt> 
    * attribute of the Laser object passed to the CoreSlam constructor
    * @param dxyMeters forward distance change of robot in meters
    * @param dthetaDegrees angular change of robot in degrees
    * @param dtSeconds time difference since previous odometry, in seconds
    */
    class Position update(int * scanvals, double dxyMeters, double dthetaDegrees, double dtSeconds);
    
    /**
    * Updates the CoreSLAM object using the current Lidar scan, without odometry.
    */
    class Position update(int * scanvals);
    
    /**
    * The quality of the map (0 through 255); default = 50
    */
    int map_quality;
    
    /**
    * The width in meters of each "hole" in the map (essentially, wall width); 
    * default = 0.6
    */
    double hole_width_meters;
    
    /**
    * The standard deviation in meters of the Gaussian distribution of 
    * the (X,Y) component of position in the particle filter; default = 0.1
    */
    double sigma_xy_meters;
    
    /**
    * The standard deviation in degrees of the Gaussian distribution of 
    * the angular rotation component of position in the particle filter; default = 20
    */
    double sigma_theta_degrees;   
    
private:
    
    // State of the entire system
    struct state_t * state;
    
};

/**
* A class for scanning laser rangefinder (Lidar) parameters.
*/
class Laser  
{
    friend class CoreSLAM;
    
public:
    
/**
* Builds a Laser object. Parameters should be based on the specifications for
* your Lidar unit.
* @param scanSize                   number of rays per scan
* @param scanRateHz                 laser scan rate in Hertz
* @param angleMinDegrees            minimum laser angle in degrees
* @param angleMaxDegrees            maximum laser angle in degrees
* @param distanceNoDetectionMeters  scan distances above this are treated as infinfity
* @param detectionMargin            number of rays at edges of scan to ignore
* @param offsetMeters               forward/backward offset of laser motor from robot center
* @return a new Laser object
* 
*/
Laser(
    int scanSize,
    int scanRateHz,
    int angleMinDegrees,
    int angleMaxDegrees,
    float distanceNoDetectionMeters,
    int detectionMargin,
    float offsetMeters
    )
{
    this->scan_size = scanSize;
    this->scan_rate_hz = scanRateHz;
    this->angle_min_degrees = angleMinDegrees;
    this->angle_max_degrees = angleMaxDegrees;
    this->distance_no_detection_meters = distanceNoDetectionMeters;
    this->detection_margin = detectionMargin;
    this->offset_meters = offsetMeters;
}


private:
    
    float offset_meters;
    int scan_rate_hz;
    int angle_min_degrees;
    int angle_max_degrees;
    int detection_margin;
    float distance_no_detection_meters;
    int scan_size;
    
public:
    
    friend ostream& operator<< (ostream & out, Laser & laser)
    {
        char str[512];
        
        sprintf(str, "<offset=%3.0f m | scan_rate=%d hz | scan_size=%d " 
            "angle_min=%d deg | angle_max=%d deg" 
            "detection_margin=%d | distance_no_detection=%4.0f mm>",
            laser.offset_meters, laser.scan_rate_hz, laser.scan_size,
            laser.angle_min_degrees, laser.angle_max_degrees,
            laser.detection_margin, laser.distance_no_detection_meters);
        
        out << str;
        
        return out;
    }
};

/**
* A class representing the position of a robot.
*/class Position 
{
    friend class CoreSLAM;
    
public:
    
    friend ostream& operator<< (ostream & out, Position & position)
    {
        char str[100];
        
        sprintf(str, "<x_meters=%5.0f mm  y_meters=%5.0f mm theta_degrees=%3.0f radians",
            position.x_meters, position.y_meters, position.theta_degrees);
        
        out << str;
        
        return out;
    }
    
    /**
    * Distance of robot from left edge of map, in meters.
    */
    double x_meters;
    
    
    /**
    * Distance of robot from top edge of map, in meters.
    */
    double y_meters;
    
    /**
    * Clockwise rotation of robot with respect to three o'clock (east), in degrees.
    */
    double theta_degrees;
    
    /**
    * Likelihood of this position with respect to other positions in a point cloud.
    */
    double likelihood;
    
private:
    
    Position(double x_meters, double y_meters, double theta_degrees)
    {
        this->x_meters = x_meters;
        this->y_meters = y_meters;
        this->theta_degrees = theta_degrees;
        
        this->likelihood = 1.0;
    }
    
    Position(double x_meters, double y_meters, double theta_degrees, double likelihood)
    {
        this->x_meters = x_meters;
        this->y_meters = y_meters;
        this->theta_degrees = theta_degrees;
        
        this->likelihood = likelihood;
    }
    
    
};


class WheeledRobot 
{
    
protected:
    
/**
* Builds a WheeledRobot object. Parameters should be based on the specifications for
* your robot.
* @param wheelRadiusMeters radius of each odometry wheel, in meters        
* @param halfAxleLengthMeters half the length of the axle between the odometry wheels, in meters  
* @return a new WheeledRobot object
*/
WheeledRobot(double wheelRadiusMeters, double halfAxleLengthMeters)
{
    this->wheelRadiusMeters = wheelRadiusMeters;     
    this->halfAxleLengthMeters = halfAxleLengthMeters;
    
    this->timestampSecondsPrev = 0;
    this->leftWheelDegreesPrev = 0;
    this->rightWheelDegreesPrev = 0;
}

/**
* Computes forward and angular velocities based on odometry.
* @param timestamp time stamp, in whatever units your robot uses       
* @param leftWheelOdometry odometry for left wheel, in whatever units your robot uses       
* @param rightWheelOdometry odometry for right wheel, in whatever units your robot uses
* @param dxyMeters gets forward distance traveled, in meters
* @param dthetaDegrees gets change in angular position, in degrees
* @param dtSeconds gets elapsed time since previous odometry, in seconds
*/

void computeVelocities(
    double timestamp, double leftWheelOdometry, double rightWheelOdometry,
    double & dxyMeters, double & dthetaDegrees, double & dtSeconds)
{                
    dxyMeters = 0;             
    dthetaDegrees = 0;              
    dtSeconds = 0;               
    
    double timestampSecondsCurr, leftWheelDegreesCurr, rightWheelDegreesCurr;
    
    this->extractOdometry(timestamp, leftWheelOdometry, rightWheelOdometry, 
        timestampSecondsCurr, leftWheelDegreesCurr, rightWheelDegreesCurr);
    
    if (this->timestampSecondsPrev > 0)
    {             
        double leftDiffDegrees = leftWheelDegreesCurr - this->leftWheelDegreesPrev;
        double rightDiffDegrees = rightWheelDegreesCurr - this->rightWheelDegreesPrev;
        
        dxyMeters =  this->wheelRadiusMeters *
        (radians(leftDiffDegrees) + radians(rightDiffDegrees));
        
        dthetaDegrees = this->wheelRadiusMeters / this->halfAxleLengthMeters *
        (rightDiffDegrees - leftDiffDegrees);
        
        dtSeconds = timestampSecondsCurr - timestampSecondsPrev;
    }
    
    // Store current odometry for next time
    this->timestampSecondsPrev = timestampSecondsCurr;
    this->leftWheelDegreesPrev = leftWheelDegreesCurr;
    this->rightWheelDegreesPrev = rightWheelDegreesCurr;        
}

/**
* Extracts usable odometry values from your robot's odometry.
* @param timestamp time stamp, in whatever units your robot uses       
* @param leftWheelOdometry odometry for left wheel, in whatever units your robot uses       
* @param rightWheelOdometry odometry for right wheel, in whatever units your robot uses
* @param timestampSeconds gets time stamp in seconds
* @param leftWheelDegrees gets left wheel rotation in degrees
* @param rightWheelDegrees gets right wheel rotation in degrees
*/
virtual void extractOdometry(
    double timestamp, 
    double leftWheelOdometry, 
    double rightWheelOdometry, 
    double & timestampSeconds, 
    double & leftWheelDegrees, 
    double & rightWheelDegrees) = 0;

friend ostream& operator<< (ostream & out, WheeledRobot & robot)
{
    
    char subclassStr[100];
    robot.descriptorString(subclassStr);
    
    char str[200];
    sprintf(str, "<Wheel radius=%f m Half axle Length=%f m | %s>",
        robot.wheelRadiusMeters, robot.halfAxleLengthMeters, subclassStr);
    
    out << str;
    
    return out;
}


/**
* Gets a descriptor string for your robot.
* @param str gets the descriptor string    
*/
virtual void descriptorString(char * str) = 0;

private:
    
    double wheelRadiusMeters;
    double halfAxleLengthMeters;
    
    double timestampSecondsPrev;
    double leftWheelDegreesPrev;
    double rightWheelDegreesPrev;    
    
    static double radians(double degrees)
    {
        return degrees * M_PI / 180;
    }
    
    
};        

