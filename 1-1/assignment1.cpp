#include <opencv2/opencv.hpp>
#include <stdio.h>

#define FLAG 1 // (0: direct access / 1: built-in function)

const char *preset_file = "fruits.jpg";

void convertColorToGray(cv::Mat &input, cv::Mat &processed);

int main(int argc, char *argv[])
{
  const char *input_file;
  // 1. prepare Mat objects for input-image and output-image
  /* hatena */
  cv::Mat input, processed;

  if(argc == 2){
    input_file = argv[1];
  }
  else{
    input_file = preset_file;
  }

  // 2. read an image from the specified file
  /* hatena */
  input = cv::imread(input_file, 1);
  if(input.empty()){
    fprintf(stderr, "cannot open %s\n", input_file);
    exit(0);
  }

  convertColorToGray(input, processed);

  // 5. create windows
  /* hatena */
  /* hatena */
  cv::namedWindow("original image", 1);
  cv::namedWindow("processed image", 1);

  // 6. show images
  /* hatena */
  /* hatena */
  cv::imshow("original image", input);
  cv::imshow("processed image", processed);

  // 7. wait key input
  /* hatena */
  cv::waitKey(0);

  // 8. save the processed result
  /* hatena */
  cv::imwrite("processed.jpg", processed);

  return 0;
}


void convertColorToGray(cv::Mat &input, cv::Mat &processed)
{
#if FLAG // use built-in function

  //4. convert color to gray
  cv::Mat temp;
  std::vector<cv::Mat> planes;
  /* gray scale */
  /* cv::cvtColor(input, temp, CV_BGR2YCrCb);
     cv::split(temp, planes);
     processed = planes[0]; */
  
  /* gaussianblur input, output, kernelsaize, stdx, stdy*/
  /* cv::GaussianBlur(input, temp, cv::Size(11, 11), 80, 3);
    processed = temp; */

  /*edge sobel*/
  cv::Sobel(input, temp, CV_32F, 1, 1, 1, 5);
  processed = temp;

#else

  // 3. create Mat for output-image
  /* hatena */
  /* hatena */
  cv::Size s = imput.size();
  processed.create(s, CV_8UC1);

  for(int j = 0; j < s.height; j++){
    uchar *ptr1, *ptr2;
    ptr1 = input.ptr<uchar>(j);
    ptr2 = processed.ptr<uchar>(j);

    //4. convert color to gray
    for(int i = 0; i < s.width; i++){
      /* hatena */
      double y = 0.114 * ((double)ptr1[0]) + 0.587 * (double)ptr1[1] + 0.299 * (double)ptr1[2];

      if(y > 255) y = 255;
      if(y < 0) y = 0;

      *ptr2 = (uchar)y;
      /* hatena */
      /* hatena */
      ptr1 += 3;
      ptr2++;
    }
  }
#endif
}