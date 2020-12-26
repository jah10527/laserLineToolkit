# Calibration tool for 3D scanner based on line laser and  camera

----

*This tool is available for free now!*

It is designed to calibrate the line structured light sensor automatically as well as the camera parameters, and the 3D scanner system usually is something like the figure shown below, where a camera and a laser are firmly fixed together.

![scanner](https://github.com/jah10527/laserLineToolkit/raw/master/res/lineLaser.jpg)

### 
---

Just clicking the *laserScanner.exe* file in this directory to open the mainwindow, there is a calibration menu in the menu bar, and activating it the calibration window will show you on the screen if no problem occurs and then you can have a try. By the way, there are some example images in the *exampleImage* subdirectory.

Tested on win10-64bit, win7-64bit and ubuntu1604-64bit, however this version is not for any Linux system.

If there is anything puzzling around you about this software, please send an email to me, and I will try my best.

Email:jah10527@126.com

Antonio Fan

March 30th 2017

Beijing China

---

### update

A new test program released at April 22nd 2018, named LineLaserCalibration-2018.4.exe

A new test program released at June 3rd 2019, named LineLaserCalibration-2019.6.exe

- more robust
- add visualization

There is a *reconstruct.cpp* file along with the tool, which you can exploit to reconstruct anything as your like. 

A new test program released at Dec. 26, 2020, named LineLaserCalibration-2020.12.exe

- add log file, which can be helpful to check whether your images is good enough when finding chessboard via cv::findChessboardCorners(gray, patternsize, corners, cv::CALIB_CB_ADAPTIVE_THRESH + cv::CALIB_CB_NORMALIZE_IMAGE);

### guidance

The main window will show like below when you double click LineLaserCalibration-2020.12.exe. and welcome to donate!

![mainwin](https://github.com/jah10527/laserLineToolkit/raw/master/res/mainwin.jpg)

You are supposed to activate this tool at first before have a try to calibrate for every time you open this tool. Clicking the calibrate button, the calibrate window will show as below.

![calibrate](https://github.com/jah10527/laserLineToolkit/raw/master/res/calib.jpg)

The load images button are used to select the images for calibrate. Once a directory is selected all the images in the fold are listed in the text browser below the button. Then clicking the calibration button,  the camera parameter and the light plain parameter are estimated at the same time. (Some images are shown in the process, just pressing any key to continue.)

![image list](https://github.com/jah10527/laserLineToolkit/raw/master/res/imageList.jpg)

The results are written in the same directory as the exe, which are called like camera_result....txt for camera parameter and stripe_result....txt for laser parameter.

If you want to get a direct view of the result in 3D space, you can click the visualize button and the figure maybe like as below.

![visualize](https://github.com/jah10527/laserLineToolkit/raw/master/res/visualize.jpg)



### Tips

One should avoid the bad situation that those lines containing control points are co-linear and close to each other as below.

![visualize](https://github.com/jah10527/laserLineToolkit/raw/master/res/visualize_badcase.jpg)



cv::findChessboardCorners may be time consuming, just be patient.



June 5th 2019.

---

[more infomation](https://jah10527.github.io/articles/lineLaser.html "线结构光视觉传感器/线激光深度传感器标定工具(续)") (in Chinese)



