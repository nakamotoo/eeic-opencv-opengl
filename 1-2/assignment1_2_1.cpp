#include <opencv2/opencv.hpp>
#include <stdio.h>

cv::Mat inpaint_mask, s_roi, d_roi, dst_img;
cv::Mat original_image, whiteLined_image, inpainted;
cv::Point start_pt;
cv::Point end_pt;

void myMouseEventHandler(int event, int x , int y , int flags, void *){
  if(whiteLined_image.empty()){
    return;
  }
  static bool isBrushDown = false;
  static bool isStart = false;
  static bool isEnd = false;
  static cv::Point prevPt;
  cv::Point pt(x, y);

  bool isLButtonPressedBeforeEvent = (bool)(flags & CV_EVENT_FLAG_LBUTTON);
  
  if(isLButtonPressedBeforeEvent && !isStart){
    // cv::line(inpaint_mask, prevPt, pt, cv::Scalar(255), 5, 8, 0);
    // cv::line(whiteLined_image, prevPt, pt, cv::Scalar::all(255), 5, 8, 0);
    // cv::imshow("image", whiteLined_image);
    start_pt = pt;
    isStart = true;
    printf("start");
  }

  // The XOR below means, isLButtonPressedAfterEvent
  // is usualy equal to isLButtonPressedBeforeEvent,
  // but not equal if the event is mouse down or up.
  bool isLButtonPressedAfterEvent = isLButtonPressedBeforeEvent ^ (event == CV_EVENT_LBUTTONUP) ^ isStart;
  if(isLButtonPressedAfterEvent && !isEnd){
      end_pt = pt;
      cv::Rect roi(start_pt, end_pt);
      cv::rectangle(whiteLined_image, roi, cv::Scalar(255, 255, 255), 3);
      cv::imshow("image", whiteLined_image);


    //   cv::Size s = original_image.size();
    //   inpaint_mask = cv::Mat::ones(s, CV_8U);
    //   cv::rectangle(inpaint_mask, roi, cv::Scalar(255, 255, 255), -1);
    //   cv::imshow("mask", inpaint_mask);


      s_roi = original_image(roi);   
      dst_img = original_image.clone();
      d_roi = dst_img(roi);
      

      isEnd = true;
      printf("end");
  }


//   bool isBrushUpAfterStarted = isStart ^ (event == CV_EVENT_LBUTTONUP) ^ (!isEnd) ^ isBrushDown;

//   if(isBrushUpAfterStarted){
//     end_pt = pt;
//     cv::Rect roi(start_pt, end_pt);
//     cv::rectangle(whiteLined_image, roi, cv::Scalar(255));
//     cv::imshow("image", whiteLined_image);
//     isEnd = true;
//   }else{
//     isBrushDown = false;
//   }
}

int main(int argc, char *argv[]){

  // 1. read image file
  const char *filename = (argc >= 2) ? argv[1] : (char *)"fruits.jpg";
  original_image = cv::imread(filename);
  if(original_image.empty()){
    printf("ERROR: image not found!\n");
    return 0;
  }

  //print hot keys
  printf( "Hot keys: \n"
      "\tESC - quit the program\n"
      "\ti or ENTER - run inpainting algorithm\n"
      "\t\t(before running it, paint something on the image)\n");

  // 2. prepare window
  cv::namedWindow("image",1);

  // 3. prepare Mat objects for processing-mask and processed-image
  /* hatena */
  whiteLined_image = original_image.clone();
  /* hatena */
  inpainted = original_image.clone();
  /* hatena */
  inpaint_mask.create(original_image.size(), CV_8UC1);

  inpaint_mask = cv::Scalar(0);
  inpainted = cv::Scalar(0);

  // 4. show image to window for generating mask
  cv::imshow("image", whiteLined_image);

  // 5. set callback function for mouse operations
  /* hatena */
  cv::setMouseCallback("image", myMouseEventHandler, 0);

  bool loop_flag = true;
  while(loop_flag){

    // 6. wait for key input
    int c = cv::waitKey(0);

    // 7. process according to input
    switch(c){
      case 27://ESC
      case 'q':
        loop_flag = false;
        break;

      case 'r':
        inpaint_mask = cv::Scalar(0);
        /* hatena */
        original_image.copyTo(whiteLined_image);
        cv::imshow("image", whiteLined_image);
        break;

      case 'i':
      case 10://ENTER (LF)
      case 13://ENTER (CR)
        cv::namedWindow("bitwise_not image", 1);
        /* hatena */
        cv::bitwise_not(s_roi, d_roi);
        cv::imshow("bitwise not image", dst_img);
        break;
    }
  }
  return 0;
}
// https://qiita.com/wakaba130/items/91ddc8c9d1cbf9ea8eda