/*********************************************************************
 *
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2015,
 *  TU Dortmund - Institute of Control Theory and Systems Engineering.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of the institute nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 * Author: Christoph Rösmann
 *********************************************************************/

#ifndef TEB_CONFIG_H_
#define TEB_CONFIG_H_

#include <ros/console.h>
#include <ros/ros.h>
#include <Eigen/Core>
#include <Eigen/StdVector>

#include <teb_local_planner/TebLocalPlannerReconfigureConfig.h>


// Definitions
#define USE_ANALYTIC_JACOBI // if available for a specific edge, use analytic jacobi 


namespace teb_local_planner
{

/**
 * @class TebConfig
 * @brief Config class for the teb_local_planner and its components.
 */  
class TebConfig
{
public:
  
  std::string odom_topic; //!< Topic name of the odometry message, provided by the robot driver or simulator
  std::string map_frame; //!< Global planning frame
  
  //! Trajectory related parameters
  struct Trajectory
  {
    double teb_autosize; //!< Enable automatic resizing of the trajectory w.r.t to the temporal resolution (recommended)
    double dt_ref; //!< Desired temporal resolution of the trajectory (should be in the magniture of the underlying control rate)
    double dt_hysteresis; //!< Hysteresis for automatic resizing depending on the current temporal resolution (dt): usually 10% of dt_ref
    int min_samples; //!< Minimum number of samples (should be always greater than 2)
    bool global_plan_overwrite_orientation; //!< Overwrite orientation of local subgoals provided by the global planner
    double force_reinit_new_goal_dist; //!< Reinitialize the trajectory if a previous goal is updated with a seperation of more than the specified value in meters (skip hot-starting)
  } trajectory; //!< Trajectory related parameters
    
  //! Robot related parameters
  struct Robot
  {
    double max_vel_x; //!< Maximum translational velocity of the robot
    double max_vel_x_backwards; //!< Maximum translational velocity of the robot for driving backwards
    double max_vel_theta; //!< Maximum angular velocity of the robot
    double acc_lim_x; //!< Maximum translational acceleration of the robot
    double acc_lim_theta; //!< Maximum angular acceleration of the robot
  } robot; //!< Robot related parameters
  
  //! Goal tolerance related parameters
  struct GoalTolerance
  {
    double yaw_goal_tolerance; //!< Allowed final orientation error
    double xy_goal_tolerance; //!< Allowed final euclidean distance to the goal position
    double free_goal_vel; //!< "Allow the robot's velocity to be nonzero (usally max_vel) for planning purposes
  } goal_tolerance; //!< Goal tolerance related parameters

  //! Obstacle related parameters
  struct Obstacles
  {
    double min_obstacle_dist; //!< Minimum desired separation from obstacles
    double costmap_emergency_stop_dist; //!< Force stop moving if the distance to costmap obstacles is below the given threshold
    bool include_costmap_obstacles; //!< Specify whether the obstacles in the costmap should be taken into account directly
    bool costmap_obstacles_front_only; //!< Limit the considered costmap obstacles to the front of the robot (much more efficient)
    int obstacle_poses_affected; //!< The obstacle position is attached to the closest pose on the trajectory to reduce computational effort, but take a number of neighbors into account as well
    int polygon_obstacle_poses_affected; //!< See obstacle_poses_affected, but here for obstacles of polygonial shape
  } obstacles; //!< Obstacle related parameters

  
  //! Optimization related parameters
  struct Optimization
  {
    int no_inner_iterations; //!< Number of solver iterations called in each outerloop iteration
    int no_outer_iterations; //!< Each outerloop iteration automatically resizes the trajectory and invokes the internal optimizer with no_inner_iterations
    
    bool optimization_activate; //!< Activate the optimization
    bool optimization_verbose; //!< Print verbose information
    
    double penalty_scale; //!< Lower values increase the penalty cost for hard-constraint approximations (do not choose too low: no satisfaction of constraints, or too high: bad matrix conditions)
    double penalty_epsilon; //!< Add a small safety margin to penalty functions for hard-constraint approximations
    
    double weight_max_vel_x; //!< Optimization weight for satisfying the maximum allowed translational velocity
    double weight_max_vel_theta; //!< Optimization weight for satisfying the maximum allowed angular velocity
    double weight_acc_lim_x; //!< Optimization weight for satisfying the maximum allowed translational acceleration
    double weight_acc_lim_theta; //!< Optimization weight for satisfying the maximum allowed angular acceleration
    double weight_kinematics_nh; //!< Optimization weight for satisfying the non-holonomic kinematics
    double weight_kinematics_forward_drive; //!< Optimization weight for forcing the robot to choose only forward directions (positive transl. velocities)
    double weight_optimaltime; //!< Optimization weight for contracting the trajectory w.r.t transition time
    double weight_point_obstacle; //!< Optimization weight for satisfying a minimum separation from point obstacles
    double weight_poly_obstacle; //!< Optimization weight for satisfying a minimum separation from polygon obstacles
    double weight_dynamic_obstacle; //!< Optimization weight for satisfying a minimum separation from dynamic obstacles
    bool alternative_time_cost; //!< Not in use yet...
    
  } optim; //!< Optimization related parameters
  
  
  struct HomotopyClasses
  {
    bool enable_homotopy_class_planning; //!< Activate homotopy class planning (Requires much more resources that simple planning, since multiple trajectories are optimized at once).
    bool enable_multithreading; //!< Activate multiple threading for planning multiple trajectories in parallel.
    bool simple_exploration; //!< If true, distinctive trajectories are explored using a simple left-right approach (pass each obstacle on the left or right side) for path generation, otherwise sample possible roadmaps randomly in a specified region between start and goal.
    int max_number_classes; //!< Specify the maximum number of allowed alternative homotopy classes (limits computational effort)
    
    int roadmap_graph_no_samples; //! < Specify the number of samples generated for creating the roadmap graph, if simple_exploration is turend off.
    double roadmap_graph_area_width; //!< Random keypoints/waypoints are sampled in a rectangular region between start and goal. Specify the width of that region in meters.
    double h_signature_prescaler; //!< Scale number of obstacle value in order to allow huge number of obstacles. Do not choose it extremly low, otherwise obstacles cannot be distinguished from each other (0.2<H<=1).
    double h_signature_threshold; //!< Two h-signatures are assumed to be equal, if both the difference of real parts and complex parts are below the specified threshold.
    
    double obstacle_keypoint_offset; //!< If simple_exploration is turned on, this parameter determines the distance on the left and right side of the obstacle at which a new keypoint will be cretead (in addition to min_obstacle_dist).
    double obstacle_heading_threshold; //!< Specify the value of the normalized scalar product between obstacle heading and goal heading in order to take them (obstacles) into account for exploration [0,1]
    
    bool visualize_hc_graph; //!< Visualize the graph that is created for exploring new homotopy classes.
  } hcp;
  

 /**
  * @brief Construct the TebConfig using default values.
  * @warning If the \b rosparam server or/and \b dynamic_reconfigure (rqt_reconfigure) node are used,
  *	     the default variables will be overwritten: \n
  *	     E.g. if \e base_local_planner is utilized as plugin for the navigation stack, the initialize() method will register a
  * 	     dynamic_reconfigure server. A subset (not all but most) of the parameters are considered for dynamic modifications.
  * 	     All parameters considered by the dynamic_reconfigure server (and their \b default values) are 
  * 	     set in \e PROJECT_SRC/cfg/TebLocalPlannerReconfigure.cfg. \n
  * 	     In addition the rosparam server can be queried to get parameters e.g. defiend in a launch file.
  * 	     The plugin source (or a possible binary source) can call loadRosParamFromNodeHandle() to update the parameters.
  * 	     In \e summary, default parameters are loaded in the following order (the right one overrides the left ones): \n
  * 		<b>TebConfig Constructor defaults << dynamic_reconfigure defaults << rosparam server defaults</b>
  */
  TebConfig()
  {
    
    odom_topic = "odom";
    map_frame = "odom"; 
    
    // Trajectory
    
    trajectory.teb_autosize = true;
    trajectory.dt_ref = 0.3;
    trajectory.dt_hysteresis = 0.1;
	trajectory.min_samples = 3;
    trajectory.global_plan_overwrite_orientation = true;
    trajectory.force_reinit_new_goal_dist = 1;
    
    // Robot
         
    robot.max_vel_x = 0.4;
    robot.max_vel_x_backwards = 0.2;
    robot.max_vel_theta = 0.3;
    robot.acc_lim_x = 0.5;
    robot.acc_lim_theta = 0.5;
    
    // GoalTolerance
    
    goal_tolerance.xy_goal_tolerance = 0.2;
    goal_tolerance.yaw_goal_tolerance = 0.1;
    goal_tolerance.free_goal_vel = false;
    
    // Obstacles
    
    obstacles.min_obstacle_dist = 0.5;
    obstacles.costmap_emergency_stop_dist = 0.3;
    obstacles.include_costmap_obstacles = true;
    obstacles.costmap_obstacles_front_only = true;
    obstacles.obstacle_poses_affected = 10;
    obstacles.polygon_obstacle_poses_affected = 25;

   
    // Optimization
    
    optim.no_inner_iterations = 5;
    optim.no_outer_iterations = 4;
    optim.optimization_activate = true;
    optim.optimization_verbose = false;
    optim.penalty_scale = 0.1;
    optim.penalty_epsilon = 0.1;
    optim.weight_max_vel_x = 2; //1
    optim.weight_max_vel_theta = 1;
    optim.weight_acc_lim_x = 1;
    optim.weight_acc_lim_theta = 1;
    optim.weight_kinematics_nh = 1000;
    optim.weight_kinematics_forward_drive = 1;
    optim.weight_optimaltime = 1;
    optim.weight_point_obstacle = 50;
    optim.weight_poly_obstacle = 10;
    optim.weight_dynamic_obstacle = 10;
    optim.alternative_time_cost = false;
    
    // Homotopy Class Planner
   
    hcp.enable_homotopy_class_planning = true;
    hcp.enable_multithreading = true;
    hcp.simple_exploration = false;
    hcp.max_number_classes = 5; 
    
    hcp.obstacle_keypoint_offset = 0.1;
    hcp.obstacle_heading_threshold = 0.45; 
    hcp.roadmap_graph_no_samples = 15;
    hcp.roadmap_graph_area_width = 6; // [m]
    hcp.h_signature_prescaler = 1;
    hcp.h_signature_threshold = 0.01;
    
    hcp.visualize_hc_graph = false;


  }
  
  /**
   * @brief Load parmeters from the ros param server.
   * @param nh const reference to the local ros::NodeHandle
   */
  void loadRosParamFromNodeHandle(const ros::NodeHandle& nh);
  
  /**
   * @brief Reconfigure parameters from the dynamic_reconfigure config.
   * Change parameters dynamically (e.g. with <c>rosrun rqt_reconfigure rqt_reconfigure</c>).
   * A reconfigure server needs to be instantiated that calls this method in it's callback.
   * In case of the plugin \e teb_local_planner default values are defined
   * in \e PROJECT_SRC/cfg/TebLocalPlannerReconfigure.cfg.
   * @param cfg Config class autogenerated by dynamic_reconfigure according to the cfg-file mentioned above.
   */
  void reconfigure(TebLocalPlannerReconfigureConfig& cfg);
  
  /**
   * @brief Return the internal config mutex
   */
  boost::mutex& configMutex() {return config_mutex_;}
  
private:
  boost::mutex config_mutex_; //!< Mutex for config accesses and changes
  
};


} // namespace teb_local_planner

#endif
