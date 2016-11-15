Name:           ros-kinetic-teb-local-planner
Version:        0.6.5
Release:        0%{?dist}
Summary:        ROS teb_local_planner package

Group:          Development/Libraries
License:        BSD
URL:            http://wiki.ros.org/teb_local_planner
Source0:        %{name}-%{version}.tar.gz

Requires:       ros-kinetic-base-local-planner
Requires:       ros-kinetic-costmap-2d
Requires:       ros-kinetic-costmap-converter
Requires:       ros-kinetic-dynamic-reconfigure
Requires:       ros-kinetic-geometry-msgs
Requires:       ros-kinetic-interactive-markers
Requires:       ros-kinetic-libg2o
Requires:       ros-kinetic-message-runtime
Requires:       ros-kinetic-nav-core
Requires:       ros-kinetic-nav-msgs
Requires:       ros-kinetic-pluginlib
Requires:       ros-kinetic-roscpp
Requires:       ros-kinetic-std-msgs
Requires:       ros-kinetic-tf
Requires:       ros-kinetic-tf-conversions
Requires:       ros-kinetic-visualization-msgs
BuildRequires:  ros-kinetic-base-local-planner
BuildRequires:  ros-kinetic-catkin
BuildRequires:  ros-kinetic-cmake-modules
BuildRequires:  ros-kinetic-costmap-2d
BuildRequires:  ros-kinetic-costmap-converter
BuildRequires:  ros-kinetic-dynamic-reconfigure
BuildRequires:  ros-kinetic-geometry-msgs
BuildRequires:  ros-kinetic-interactive-markers
BuildRequires:  ros-kinetic-libg2o
BuildRequires:  ros-kinetic-message-generation
BuildRequires:  ros-kinetic-nav-core
BuildRequires:  ros-kinetic-nav-msgs
BuildRequires:  ros-kinetic-pluginlib
BuildRequires:  ros-kinetic-roscpp
BuildRequires:  ros-kinetic-std-msgs
BuildRequires:  ros-kinetic-tf
BuildRequires:  ros-kinetic-tf-conversions
BuildRequires:  ros-kinetic-visualization-msgs

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
if [ -f "/opt/ros/kinetic/setup.sh" ]; then . "/opt/ros/kinetic/setup.sh"; fi
mkdir -p obj-%{_target_platform} && cd obj-%{_target_platform}
%cmake .. \
        -UINCLUDE_INSTALL_DIR \
        -ULIB_INSTALL_DIR \
        -USYSCONF_INSTALL_DIR \
        -USHARE_INSTALL_PREFIX \
        -ULIB_SUFFIX \
        -DCMAKE_INSTALL_LIBDIR="lib" \
        -DCMAKE_INSTALL_PREFIX="/opt/ros/kinetic" \
        -DCMAKE_PREFIX_PATH="/opt/ros/kinetic" \
        -DSETUPTOOLS_DEB_LAYOUT=OFF \
        -DCATKIN_BUILD_BINARY_PACKAGE="1" \

make %{?_smp_mflags}

%install
# In case we're installing to a non-standard location, look for a setup.sh
# in the install tree that was dropped by catkin, and source it.  It will
# set things like CMAKE_PREFIX_PATH, PKG_CONFIG_PATH, and PYTHONPATH.
if [ -f "/opt/ros/kinetic/setup.sh" ]; then . "/opt/ros/kinetic/setup.sh"; fi
cd obj-%{_target_platform}
make %{?_smp_mflags} install DESTDIR=%{buildroot}

%files
/opt/ros/kinetic

%changelog
* Tue Nov 15 2016 Christoph Rösmann <christoph.roesmann@tu-dortmund.de> - 0.6.5-0
- Autogenerated by Bloom

* Sun Oct 23 2016 Christoph Rösmann <christoph.roesmann@tu-dortmund.de> - 0.6.4-0
- Autogenerated by Bloom

* Wed Aug 17 2016 Christoph Rösmann <christoph.roesmann@tu-dortmund.de> - 0.6.3-0
- Autogenerated by Bloom

* Wed Jun 15 2016 Christoph Rösmann <christoph.roesmann@tu-dortmund.de> - 0.6.2-0
- Autogenerated by Bloom

* Mon May 23 2016 Christoph Rösmann <christoph.roesmann@tu-dortmund.de> - 0.6.1-0
- Autogenerated by Bloom

* Mon May 23 2016 Christoph Rösmann <christoph.roesmann@tu-dortmund.de> - 0.6.0-0
- Autogenerated by Bloom

