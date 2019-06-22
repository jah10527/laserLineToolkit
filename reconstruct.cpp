unsigned char fan_maxLoc(unsigned char *pArr, unsigned int nLength, unsigned int *max_loc)
{
  unsigned int i; 
  unsigned char nMax;
  if (pArr[0] > pArr[1])  
  {  
    nMax = pArr[0];  
    *max_loc = 0;
  }  
  else  
  {  
    nMax = pArr[1];  
    *max_loc = 1;
  }
  for (i=2; i<nLength; i+=2)  
  {  
    if (pArr[i] < pArr[i+1])
    {
      if (nMax < pArr[i+1])
      {  
        nMax = pArr[i+1]; 
        *max_loc = i+1;                    
      }
    }
    else
    {
      if (nMax < pArr[i])
      {  
        nMax = pArr[i]; 
        *max_loc = i;
      }
    }
  }
  return nMax;
}

/*
find laser stripe center, supposing that the x is 0:cols and y the location of maximum value for each column saved in the loc array. You can have a different vertion of it.
*/
void laserCenter(cv::Mat &im, float *loc)
{
  uchar thr = 220;
  uchar *pArr = im.data;
  unsigned char val;
  unsigned int maxLoc;
  unsigned int count, intensity;
  for (int i=0; i<im.rows; i++)
  {
    val = fan_maxLoc(pArr+i*im.cols, im.cols, &maxLoc);
    if (maxLoc<5 || maxLoc>im.cols-25 || val<thr)
    {
      loc[i] = 0;
      continue;
    }
    count = 0;
    intensity = 0;
    for  (int j=maxLoc-5; j<maxLoc+25; j++)
    {
      val = *(pArr+i*im.cols+j);
      if (val<thr)
      {
        if (j<maxLoc) continue;
        if (j>maxLoc) break;
      }
      count += val*j;
      intensity += val;
    }
    loc[i] = count/intensity;
  }
  return ;
}

/*calculate the 3D coordinates of the stripe.*/
void calc3Dpts(cv::Mat &A, cv::Mat &results, float *loc, int size)
{
    double s;
    results = cv::Mat::zeros(size,3,CV_32FC1);
    for (int i=0; i<size; i++)
    {
        if (loc[i]<1e-6) continue;
        s = 1/(A.at<double>(3,0)*i+A.at<double>(3,1)*loc[i]+A.at<double>(3,2));
        results.at<float>(i,0) = s*(A.at<double>(0,0)*i+A.at<double>(0,1)*loc[i]+A.at<double>(0,2));
        results.at<float>(i,1) = s*(A.at<double>(1,0)*i+A.at<double>(1,1)*loc[i]+A.at<double>(1,2));
        results.at<float>(i,2) = s*(A.at<double>(2,0)*i+A.at<double>(2,1)*loc[i]+A.at<double>(2,2));

    }
}


/*Load the camera parameters*/
void snapwindow::on_pushButtonLoadCam_clicked()
{
    QString filename = QFileDialog::getOpenFileName(
       this,
       "Load Cam Parameters",
       QDir::currentPath(),
       "Document files (*.txt);;All files(*.*)");
    if (!filename.isNull()) {
        cv::FileStorage fs;
        fs.open(filename.toStdString(), cv::FileStorage::READ);
        try
        {
            fs["camera_matrix"] >> kk;
            fs["distortion_coefficients"] >> dist;
            A.at<double>(0,0) = kk.at<double>(0,0);
            A.at<double>(0,2) = kk.at<double>(0,2);
            A.at<double>(1,1) = kk.at<double>(1,1);
            A.at<double>(1,2) = kk.at<double>(1,2);
            A.at<double>(2,2) = kk.at<double>(2,2);
            QMessageBox::information(this, "Document", "OK", QMessageBox::Ok | QMessageBox::Cancel);
            paramFlag = 1;
        }
        catch (...)
        {
            QMessageBox::information(this, "Document", "Failed", QMessageBox::Ok | QMessageBox::Cancel);
        }
    }
}

/*Load the stripe parameters*/
void snapwindow::on_pushButton_clicked()
{
    QString filename = QFileDialog::getOpenFileName(
       this,
       "Load Stripe Parameters",
       QDir::currentPath(),
       "Document files (*.txt);;All files(*.*)");
    if (!filename.isNull() && paramFlag==1) {
        cv::FileStorage fs;
        fs.open(filename.toStdString(), cv::FileStorage::READ);
        try
        {
            fs["params"] >> lightParam;
            A.at<double>(3,0) = lightParam.at<double>(0);
            A.at<double>(3,1) = lightParam.at<double>(1);
            A.at<double>(3,2) = lightParam.at<double>(2);
            A.at<double>(3,3) = -lightParam.at<double>(3);
            A=A.inv();
            QMessageBox::information(this, "Document", "OK", QMessageBox::Ok | QMessageBox::Cancel);
            paramFlag++;
        }
        catch (...)
        {
            QMessageBox::information(this, "Document", "Failed", QMessageBox::Ok | QMessageBox::Cancel);
        }
    }
}

void snapwindow::cameraTimerTimeout()
{
    if(capture.isOpened())
    {
        // get the current frame from the video stream
        capture >> imrgb;
        if (imrgb.empty())
        {
            capture.release();
            cameraTimer.stop();
            ui->pushButtonStartPreview->setEnabled(true);
            ui->pushButtonClose->setEnabled(false);
            ui->pushButtonSnap->setEnabled(false);
            ui->pushButtonVideoIn->setEnabled(true);
            ui->pushButtonPause->setEnabled(false);
            return;
        }
        if (paramFlag==2)
            cv::undistort(imrgb,im,kk,dist);
        else
        {
            im = imrgb;
        }

        if (ui->radioButtonLaser->isChecked())
        {
            cv::Mat results;
            float loc[640];
            laserCenter(im, loc); //find center 
            if (paramFlag==2)
            {
            calc3Dpts(A, results, loc, sizeof(loc)/sizeof(float));
            int rr = 20;
            for (int i=0; i<im.cols; i++)
            {
                if (loc[i]<1e-6) continue;
                im.at<cv::Vec3b>(round(loc[i]), i) = cv::Vec3b(0, 255, 0);
                if (i|0xF==0)
                {
                    rr += 20;
                    cv::putText(im, std::to_string(results.at<float>(i,2)),cv::Point2d(20, rr),CV_FONT_HERSHEY_COMPLEX, 1, cv::Scalar(255, 0, 0));
                }
            }
            memcpy(ui->openGLWidget->data, results.data, sizeof(ui->openGLWidget->data));
            ui->openGLWidget->update();
            }
        }

        // ... change color channel ordering (from BGR in our Mat to RGB in QImage)
        cv::cvtColor(im, imrgb, CV_BGR2RGB);

        frameToShow = QImage((const unsigned char*)(imrgb.data), imrgb.cols, imrgb.rows, imrgb.step, QImage::Format_RGB888);

        ui->labelPreview->setPixmap(QPixmap::fromImage(frameToShow));
    }
}
