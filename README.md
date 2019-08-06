# thor [![Travis Status](https://travis-ci.org/davisking/dlib.svg?branch=master)](https://travis-ci.org/davisking/dlib)

<p align="center">
<img src="https://s2.ax1x.com/2019/06/06/VdV0i9.png" />
</p>

**thor** is a C++ helper library which provide huge utilities, algorithms, and visualization functions for deep learning. We recommend install thor from github source since we update thor new APIs a lot. But *thor* will always compatible with older versions, it safe and reliable integrate into your projects and providing useful utilities.

**note**: thor does not support Windows for now, we will upgrade to support Windows as quickly as possiable.



## Install

a). If you only need thor independent modules without OpenCV or Eigen or Protobuf or Curl, you can simply run:

```
./build_simple.sh
```

*this will build a standalone `thor.so` without link any other libs, but if you call any function that needs opencv or protobuf will leads to an error.*

b). If you need full capacity which thor does, including `vis`, `geometry`, `datum` modules, you can run:

```
./build_full.sh
```

*this will build a full version of thor with link to opencv, protobuf, curl, freetype. so if you call any related function, you gonna need link to that lib first in your cmake file.*



**note**: Our `logging` module using same micro define as glog, so it will conflicts if you importing them both. If your project doesn't need glog and you are using thor, you can simply deprecate glog and using thor only.



## Updates

- **2050.01.01**: to be continue..

- **2019.08.07**: O(∩_∩)O~~!!! A lightweighted logging lib has been integrated into thor!!!!! Now, you can using `LOG(INFO)` without glog:

    ```c++
    #include "thor/logging.h"
    
    using namespace thor;
    
    int main() {
    	LOG(INFO) << "this is a log.";
    }
    ```

    We'll see:

    ```
    I 8/7 11:24:46.484 ...thor_simple.cpp main:28]  this is a log
    ```

    

- **2019.08.06**: We add precompile define to turn on or off dependencies, such as opencv or protobuf, those dependencies were turned off by default. If you need full version of thor, simple build with `build_full.sh` ;

- **2019.06.16**: Update CMakeLists.txt fix eigen include issue, now users install will not got eigen include not found problem, simply remember to install `libeigen3-dev` first.

- **2019.06.15**: Adding HumanPose drawing in thor!!! Here is what it looks like:

    <p align="center">
        <img src="https://upload-images.jianshu.io/upload_images/617746-f083cb7d20c91f2d.gif"/>
    </p>

    this can be down by simply apply this:

    ```c++
    #include "thor/vis.h"
    using namespace thor::vis;
    int main() {
        // suppose you have detected poses
        std::vector<thor::HumanPose> poses = ..;
        renderHumanPose(poses, image);
    }
    ```


- **2019.05.23**: We just add a `datum` module into thor and providing a series of `str_utils` which can be very useful for normal C++ development. **Do you want judge a point is on a line segment or not? Do you want describe a rotate box and calculate it's area easily?**, thor `datum` is here to help:

    ```c++
    // datum
    #include "thor/datum.h"
    
    using namespace thor::datum;
    using namespace thor::generic;
    
    Vector2d p1(36, 36);
    Vector2d p2(78, 78);
    
    Vector2d p(23, 23);
    LineSegment2d l(p1, p2);
    
    //call LineSegment2d.IsPointIn() check a point is one segment or not
    cout << l.IsPointIn(p) << endl;
    // out: false, since 23 is out of line
    p = Vector2d(46, 46);
    cout << l.IsPointIn(p) << endl;
    // out: true, since 46 is on line.
    
    AABox2d abox1(Vector2d(2, 4), 224, 345);
    cout << abox1.DebugString() << endl;
    cout << "area: " << abox1.area() << endl;
    ```

    that's it! Using `thor::datum` you can do many math and geometry much more easily!!  **More excitedly, `thor::datum` data structure is compatible with apollo**!!

- **2019.05.19**:  There would be a huge updates on **thor** in recent days. We are trying to integrate all math library in Apollo into thor. So that it can do lots of math calculations.  If you got any undefined inference error just open an issue about that. Currently if you want using thor, you gonna need **link protobuf** in your c++ project.   

- **2019.05.01**: Add some geometry utils such as conversion between quaternion and euler angels for robots and autocar:
    ```c++
    Eigen::Matrix3d Quaternion2RotationMatrix(const double x, const double y, const double z, const double w);
    Eigen::Quaterniond EulerAngle2Quaternion(const double yaw, const double pitch, const double roll);                             
    ```
    **note**: this is only available when you have Eigen installed.

- **2019.01.15**: Tracking projects using **thor**:
    <p align="center">
    <img src="https://s2.ax1x.com/2019/05/16/EbUj2t.gif" />
    </p>

- **2018.09.15**: Object detection projects using **thor**:
    <p align="center">
    <img src="https://s1.ax1x.com/2018/12/07/F1xG7D.gif" />
    </p>






## Usage

**thor** has a very clean and easy-to-use API, it consist of many parts. All parts can be listed as follow:

- *vis*: For computer vision, draw boxes or segmentation results on image, should worked with opencv;
- *files*;
- *random*;
- *color*;
- *logging*;
- *slam*;

You only need add this line to CMakeLists.txt when you have thor installed in your system path.

```cmake
target_link_libraries(main -lthor)
```

to build with Makefile, using:

```shell
g++ -o a a.cpp `pkg-config thor --cflags --libs`
```
In case you want integrate but with trouble, find me via wechat: `jintianiloveu`


## Features
We list module **thor** currently finished here:

- [x] os
- [x] color
- [x] random
- [x] log
- [x] string utils
- [x] geometry (quaternion and euler angle conversion)


## Copyright

**thor** was build by *Lucas Jin* with ❤️. Under MIT license.
