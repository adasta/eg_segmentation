/*
 * eg_segmentation.h
 *
 *  Created on: Aug 26, 2012
 *      Author: Adam Stambler
 */

#ifndef EG_SEGMENTATION_H_
#define EG_SEGMENTATION_H_

#include <opencv2/opencv.hpp>
#include <map>

namespace cv {

  /* * Segment an image
 *
 * outputs an index image, uint16_t, representing the segments.

 * input: image to segment.
 * sigma: to smooth the image.
 * c: constant for treshold function.
 * min_size: minimum component size (enforced by post-processing stage).
 *
 * returns number of segments created
 */
  int segment_image(const cv::Mat& input,  cv::Mat& output, float sigma =0.5, int k=500, int min_size=20);

  typedef std::map<uint16_t, cv::Vec3b> ColorMap;
  void colorize_index_image(const cv::Mat& input, cv::Mat& output,   ColorMap& map );

}


#endif /* EG_SEGMENTATION_H_ */
