# thor [![Travis Status](https://travis-ci.org/davisking/dlib.svg?branch=master)](https://travis-ci.org/davisking/dlib)

<p align="center">
<img src="https://s2.ax1x.com/2019/06/06/VdV0i9.png" />
</p>
**thor** is a C++ helper library which provide huge utilities, algorithms, and visualization functions for deep learning. We recommend install thor from github source since we update thor new APIs a lot. But *thor* will always compatible with older versions, it safe and reliable integrate into your projects and providing useful utilities.

Glad to know this repo was recommended by 爱可可老师! The link is https://weibo.com/1402400261/I8p1gnIkK .

>  note: Now thor built against protobuf by default (due to we use proto to visualize boxes in C++ for cross-platform), so in your project, you gonna need add following in your CMakeLists.txt:

```cmake
find_package(Protobuf REQUIRED)
target_link_library(your_executable ${PROTOBUF_LIBRARY} thor)
```


## Roadmap

**thor** is still need progress and enlarge it's functionality. Current roadmap are:

- [x] support instance segmentation visualization;
- [ ] upload thor to ubuntu package manager;



## Quick Install

Simple version:

```
./build_simple.sh
```

You need install `libprotobuf-dev` and `protobuf-compiler` first on Ubuntu or linux system. If you got any problems about not found **libcurl** or **protobuf header files**, be sure installed libs inside `build_simple.sh`.

If you want to build thor on **JetsonNano**, you need manually uncomment CMakeLists.txt link path to your aarch64 system path (normal PC is x86).

![image-20201107222319190](https://i.loli.net/2020/11/07/o1LyGD8l3EHabkK.png)

![image-20201107222409982](https://i.loli.net/2020/11/07/UGmwID86ZaKlV5n.png)

If you setup correctly, thor can be successfully installed on **JetsonNano** as well.




## Install

**If you are a newbie of C++ or Ubuntu, recommend you using simple mode**.

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

- **2021.01.11**: New thor built with **C++14** by default. `det.pro` added `x, y, w, h` as attributes.

- **2020.08.17**: thor now supports visualize lane:

    ```c++
    auto resImg = thor::vis::VisualizeLanes(image, res, nullptr, 12, 1.0);
    ```

    ![](https://i.loli.net/2020/08/21/byrAOkCFNoWItP4.png)

    

- **2020.04.15**: **thor now build with curl and protobuf** by default. If you got any question about protobuf link problem, pls fire an issue, I will help u fix that, normally thor should built successfully without any errors, as long as you install default libs:

    ```
    sudo apt install libprotobuf-dev
    sudo apt install protobuf-compiler
    sudo apt install libcurl4-openssl-dev
    ```

    We add protobuf as default built for the introduce of protos which we will using for default data structures:

    ```c++
    Detection2D det1;
    Box box;
    box.set_x1(23);
    box.set_y1(89);
    box.set_x2(99);
    box.set_y2(156);
    det1.set_allocated_box(&box);
    det1.set_cls_id(9);
    det1.set_prob(0.9);
    
    InstanceSegmentation seg1;
    seg1.set_allocated_detection(&det1);
    // float32
    seg1.add_mask(2.3);
    seg1.add_mask(2.3);
    seg1.add_mask(2.3);
    seg1.add_mask(2.3);
    
    LOG(INFO) << seg1.DebugString();
    ```

    this will easy transport to other languages:

    ```
    detection {
      box {
        x1: 23
        y1: 89
        x2: 99
        y2: 156
      }
      cls_id: 9
      prob: 0.9
    }
    mask: 2.3
    mask: 2.3
    mask: 2.3
    mask: 2.3
    ```

- **2019.12.26**: We add a `functions` in thor to enable some tiny functions:

    ```c++
    #include "thor/functions.h"
    
    double values[] = {-0.9381,  0.8967};
    double probs[2];
    thor::functions::softmax_1d(values, probs, 2);
    LOG(INFO) << probs[0] << " " << probs[1];
    ```

    Also we update a bug in `HWC2CHW`, now it works well now.

- **2019.11.16**: We demonstrate how to using `thor::vis` to draw detections in your algorithm:

    ![](https://s2.ax1x.com/2019/11/15/Mdig8e.png)

    ```c++
    #include "thor/dl.h"
    #include "thor/os.h"
    #include "thor/structures.h"
    #include "thor/vis.h"
    
    using namespace std;
    using namespace cv;
    using namespace thor::vis;
    using namespace thor::dl;
    
    int main() {
        vector<thor::Box> all_detections;
      for (int i = 0; i < num_det; i++) {
        // Show results over confidence threshold
        if (scores[i] >= 0.33f) {
          float x1 = boxes[i * 4 + 0] * scale_x;
          float y1 = boxes[i * 4 + 1] * scale_y;
          float x2 = boxes[i * 4 + 2] * scale_x;
          float y2 = boxes[i * 4 + 3] * scale_y;
          thor::Box one_box{x1, y1, x2, y2, thor::BoxFormat::XYXY};
          one_box.score = scores[i];
          one_box.idx = classes[i] + 1;
          all_detections.emplace_back(one_box);
        }
      }
    
    // draw
    auto res_image = thor::vis::VisualizeDetectionStyleDetectron2(
              image, all_detections, COCO_CLASSES);
    }
    ```

    Above is a simple example using `thor::vis::VisualizeDetectionStyleDetectron2` draw bounding boxes using detectron2 style.  From our experiences, these steps time cost is about **0.0001**s, so it's doesn't matter if you generate your box format to `thor::Box` format first and then call our drawing method.

- **2019.09.24**: I just notice thor has been recommended by 爱可可老师 through weibo, here is link: https://weibo.com/1402400261/I8p1gnIkK , check it out!!! If you like this project, pls give a star!!

- **2019.08.29**: A new header-only args parser lib has been added into thor. Original implementation from [here](https://github.com/Taywee/args). We did some changes than original, roughly usage can be used like this:

    ````c++
    #include "thor/args.h"
    using namespace thor;
    int main() {
      args::ArgumentParser parser("YoloV3 TensorRT", "args parse from thor");
      args::HelpFlag help(parser, "HELP", "Show this help menu.", {"help"});
      args::ValueFlag<std::string> data(parser, "data", "data.", {'d'}, "");
      args::ValueFlag<std::string> proto_txt(parser, "proto_txt", "proto_txt.", {'p'}, "");
      args::ValueFlag<std::string> model(parser, "model", "caffe model.", {'m'}, "");
      args::ValueFlag<std::string> engine(parser, "engine", "trt engine file.", {"engine"}, "");
        
        try
        {
            parser.ParseCLI(argc, argv);
        }
        catch (const args::Completion& e)
        {
            std::cout << e.what();
            return 0;
        }
        catch (const args::Help&)
        {
            std::cout << parser;
            return 0;
        }
        catch (const args::ParseError& e)
        {
            std::cerr << e.what() << std::endl;
            std::cerr << parser;
            return 1;
        }
    }
    ````

    You will see:

    ```
    ./examples/trt_yolov3 {OPTIONS}
    
        YoloV3 TensorRT
    
      OPTIONS:
    
          --help                            Show this help menu.
          -d[data]                          data.
          -p[proto_txt]                     proto_txt.
          -m[model]                         caffe model.
          --engine=[engine]                 trt engine file.
        args parse from thor
    ```

    This is useful when you do not want gflags and glog lib, since those 2 has been integrated into thor!!

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
