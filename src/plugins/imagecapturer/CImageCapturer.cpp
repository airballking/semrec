/*********************************************************************
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2013, Institute for Artificial Intelligence,
 *  Universität Bremen.
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
 *   * Neither the name of the Institute for Artificial Intelligence,
 *     Universität Bremen, nor the names of its contributors may be
 *     used to endorse or promote products derived from this software
 *     without specific prior written permission.
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
 *********************************************************************/

/** \author Jan Winkler */


#include <plugins/imagecapturer/CImageCapturer.h>


namespace beliefstate {
  CImageCapturer::CImageCapturer() {
    m_strImagesTopic = "";
  }

  CImageCapturer::~CImageCapturer() {
  }

  bool CImageCapturer::fileExists(string strFileName) {
    ifstream ifile(strFileName.c_str());
  
    if(ifile) {
      return true;
    }
  
    return false;
  }

  void CImageCapturer::freeFilename(string& strFileName, string strWorkingDirectory) {
    int nIndex = 0;
    string strBase = strWorkingDirectory + strFileName;
    string strFile = strFileName;
    
    while(this->fileExists(strBase)) {
      stringstream sts;
      sts << nIndex;
      sts << "_";
      sts << strFileName;
    
      nIndex++;
      
      strFile = sts.str();
      strBase = strWorkingDirectory + sts.str();
    }
    
    strFileName = strFile;
  }
  
  bool CImageCapturer::captureFromTopic(string strTopicName, string& strFileName, string strWorkingDirectory, bool bUseFreeName) {
    int nTimeout = 1000;
    bool bReturnvalue = false;
    bool bGoon = true;
    m_bReceived = false;
  
    ros::NodeHandle nh;
    m_subImage = nh.subscribe(strTopicName, 1, &CImageCapturer::imageCallback, this);
  
    while(bGoon) {
      nTimeout--;
    
      ros::Duration(0.01).sleep();
      ros::spinOnce();
    
      if(nTimeout <= 0 || m_bReceived) {
	bGoon = false;
      }
    }
  
    m_subImage.shutdown();
  
    if(m_bReceived) {
      cv_bridge::CvImagePtr cv_ptr;
    
      try {
	cv_ptr = cv_bridge::toCvCopy(m_imgReceived, sensor_msgs::image_encodings::BGR8);
      
	cv::Mat imgMat = cv_ptr->image;
	
	string strUseFilename = "";
	if(bUseFreeName) {
	  this->freeFilename(strFileName, strWorkingDirectory);
	  strUseFilename = strWorkingDirectory + strFileName;
	} else {
	  strUseFilename = strWorkingDirectory + strFileName;
	}
	
	cv::imwrite(strUseFilename, imgMat);
	
	if(m_strImagesTopic != "") {
	  m_pubImages.publish(m_imgReceived);
	}
	
	bReturnvalue = true;
      } catch (cv_bridge::Exception& e) {
      }
    }
  
    return bReturnvalue;
  }

  void CImageCapturer::imageCallback(const sensor_msgs::Image &imgData) {
    if(!m_bReceived) {
      m_bReceived = true;
      m_imgReceived = imgData;
    }
  }

  void CImageCapturer::publishImages(string strImagesTopic) {
    m_strImagesTopic = strImagesTopic;
    ros::NodeHandle nh;
    
    m_pubImages = nh.advertise<sensor_msgs::Image>(strImagesTopic, 1);
  }
}
