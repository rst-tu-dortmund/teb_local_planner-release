Name:           ros-lunar-teb-local-planner
Version:        0.7.1
Release:        0%{?dist}
Summary:        ROS teb_local_planner package

Group:          Development/Libraries
License:        BSD
URL:            http://wiki.ros.org/teb_local_planner
Source0:        %{name}-%{version}.tar.gz

Requires:       ros-lunar-base-local-planner
Requires:       ros-lunar-costmap-2d
Requires:       ros-lunar-costmap-converter
Requires:       ros-lunar-dynamic-reconfigure
Requires:       ros-lunar-geometry-msgs
Requires:       ros-lunar-interactive-markers
Requires:       ros-lunar-libg2o
Requires:       ros-lunar-message-runtime
Requires:       ros-lunar-nav-core
Requires:       ros-lunar-nav-msgs
Requires:       ros-lunar-pluginlib
Requires:       ros-lunar-roscpp
Requires:       ros-lunar-std-msgs
Requires:       ros-lunar-tf
Requires:       ros-lunar-tf-conversions
Requires:       ros-lunar-visualization-msgs
BuildRequires:  ros-lunar-base-local-planner
BuildRequires:  ros-lunar-catkin
BuildRequires:  ros-lunar-cmake-modules
BuildRequires:  ros-lunar-costmap-2d
BuildRequires:  ros-lunar-costmap-converter
BuildRequires:  ros-lunar-dynamic-reconfigure
BuildRequires:  ros-lunar-geometry-msgs
BuildRequires:  ros-lunar-interactive-markers
BuildRequires:  ros-lunar-libg2o
BuildRequires:  ros-lunar-message-generation
BuildRequires:  ros-lunar-nav-core
BuildRequires:  ros-lunar-nav-msgs
BuildRequires:  ros-lunar-pluginlib
BuildRequires:  ros-lunar-roscpp
BuildRequires:  ros-lunar-std-msgs
BuildRequires:  ros-lunar-tf
BuildRequires:  ros-lunar-tf-conversions
BuildRequires:  ros-lunar-visualization-msgs

%description
The teb_local_planner package implements a plugin to the base_local_planner of
the 2D navigation stack. The underlying method called Timed Elastic Band locally
optimizes the robot's trajectory with respect to trajectory execution time,
separation from obstacles and compliance with kinodynamic constraints at
runtime.

%prep
%setup -q

%build
# In case we're installing to a non-standard location, look for a setup.sh
# in the install tree that was dropped by catkin, and source it.  It will
# set things like CMAKE_PREFIX_PATH, PKG_CONFIG_PATH, and PYTHONPATH.
if [ -f "/opt/ros/lunar/setup.sh" ]; then . "/opt/ros/lunar/setup.sh"; fi
mkdir -p obj-%{_target_platform} && cd obj-%{_target_platform}
%cmake .. \
        -UINCLUDE_INSTALL_DIR \
        -ULIB_INSTALL_DIR \
        -USYSCONF_INSTALL_DIR \
        -USHARE_INSTALL_PREFIX \
        -ULIB_SUFFIX \
        -DCMAKE_INSTALL_LIBDIR="lib" \
        -DCMAKE_INSTALL_PREFIX="/opt/ros/lunar" \
        -DCMAKE_PREFIX_PATH="/opt/ros/lunar" \
        -DSETUPTOOLS_DEB_LAYOUT=OFF \
        -DCATKIN_BUILD_BINARY_PACKAGE="1" \

make %{?_smp_mflags}

%install
# In case we're installing to a non-standard location, look for a setup.sh
# in the install tree that was dropped by catkin, and source it.  It will
# set things like CMAKE_PREFIX_PATH, PKG_CONFIG_PATH, and PYTHONPATH.
if [ -f "/opt/ros/lunar/setup.sh" ]; then . "/opt/ros/lunar/setup.sh"; fi
cd obj-%{_target_platform}
make %{?_smp_mflags} install DESTDIR=%{buildroot}

%files
/opt/ros/lunar

%changelog
* Tue Jun 05 2018 Christoph Rösmann <christoph.roesmann@tu-dortmund.de> - 0.7.1-0
- Autogenerated by Bloom

* Sat Sep 23 2017 Christoph Rösmann <christoph.roesmann@tu-dortmund.de> - 0.7.0-0
- Autogenerated by Bloom

