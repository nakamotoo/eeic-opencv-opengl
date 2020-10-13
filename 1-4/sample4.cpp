#include <opencv2/opencv.hpp>
#include <stdio.h>

int main(int argc, char *argv[]) {
  int INIT_TIME = 50;
  double B_PARAM = 1.0 / 50.0;
  double T_PARAM = 1.0 / 200.0;
  double Zeta = 10.0;

  cv::VideoCapture cap;
  cv::Mat frame;
  cv::Mat avg_img, sgm_img;
  cv::Mat lower_img, upper_img, tmp_img;
  cv::Mat dst_img, msk_img;

  // 1. initialize VideoCapture
  if (argc > 1) {
    char *in_name = argv[1];
    if ('0' <= in_name[0] && in_name[0] <= '9' && in_name[1] == '\0') {
      cap.open(in_name[0] - '0'); // open with device id
    } else {
      cap.open(in_name); // open with filename
    }
  } else {
    cap.open(0);
  }
  if (!cap.isOpened()) {
    printf("Cannot open the video.\n");
    exit(0);
  }

  // 2. prepare window for showing images
  cv::namedWindow("Input", 1);
  cv::namedWindow("FG", 1);
  cv::namedWindow("mask", 1);

  // 3. calculate initial value of background
  cap >> frame;

  cv::Size s = frame.size();

  avg_img.create(s, CV_32FC3);
  sgm_img.create(s, CV_32FC3);
  lower_img.create(s, CV_32FC3);
  upper_img.create(s, CV_32FC3);
  tmp_img.create(s, CV_32FC3);

  dst_img.create(s, CV_8UC3);
  msk_img.create(s, CV_8UC1);

  printf("Background statistics initialization start\n");

  avg_img = cv::Scalar(0, 0, 0);

  for (int i = 0; i < INIT_TIME; i++) {
    cap >> frame;
    cv::Mat tmp;
    frame.convertTo(tmp, avg_img.type());
    /* hatena */
    cv::accumulate(tmp, avg_img);
  }

  /* hatena */
  avg_img.convertTo(avg_img, -1, 1.0 / INIT_TIME);
  sgm_img = cv::Scalar(0, 0, 0);

  for (int i = 0; i < INIT_TIME; i++) {
    cap >> frame;
    frame.convertTo(tmp_img, avg_img.type());
    /* hatena */
    cv::subtract(tmp_img, avg_img, tmp_img);
    /* hatena */
    cv::pow(tmp_img, 2.0, tmp_img);
    /* hatena */
    tmp_img.convertTo(tmp_img, -1, 2.0);
    /* hatena */
    cv::sqrt(tmp_img, tmp_img);
    /* hatena */
    cv::accumulate(tmp_img, sgm_img);
  }

  /* hatena */
  sgm_img.convertTo(sgm_img, -1, 1.0 / INIT_TIME);

  printf("Background statistics initialization finish\n");

  bool loop_flag = true;
  while (loop_flag) {
    cap >> frame;
    frame.convertTo(tmp_img, tmp_img.type());

    // 4. check whether pixels are background or not
    /* hatena */
    cv::subtract(avg_img, sgm_img, lower_img);
    /* hatena */
    cv::subtract(lower_img, Zeta, lower_img);
    /* hatena */
    cv::add(avg_img, sgm_img, upper_img);
    /* hatena */
    cv::add(upper_img, Zeta, upper_img);
    /* hatena */
    cv::inRange(tmp_img, lower_img, upper_img, msk_img);

    // 5. recalculate
    /* hatena */
    cv::subtract(tmp_img, avg_img, tmp_img);
    /* hatena */
    cv::pow(tmp_img, 2.0, tmp_img);
    /* hatena */
    tmp_img.convertTo(tmp_img, -1, 2.0);
    /* hatena */
    cv::sqrt(tmp_img, tmp_img);

    // 6. renew avg_img and sgm_img
    /* hatena */
    cv::accumulateWeighted(frame, avg_img, B_PARAM, msk_img);
    /* hatena */
    cv::accumulateWeighted(tmp_img, sgm_img, B_PARAM, msk_img);

    /* hatena */
    cv::bitwise_not(msk_img, msk_img);
    /* hatena */
    cv::accumulateWeighted(tmp_img, sgm_img, T_PARAM, msk_img);

    dst_img = cv::Scalar(0);
    /* hatena */
    frame.copyTo(dst_img, msk_img);

    cv::imshow("Input", frame);
    cv::imshow("FG", dst_img);
    cv::imshow("mask", msk_img);

    char key = cv::waitKey(10);
    if (key == 27) {
      loop_flag = false;
    }
  }
  return 0;
}