
import cv2
import numpy as np
from IPython.display import display, Image
from copy import deepcopy

if __name__ == '__main__':

    vidcap = cv2.VideoCapture('game2.mov')
    success, image = vidcap.read()
    frame_rate = int(vidcap.get(cv2.CAP_PROP_FPS))
    w = int(vidcap.get(cv2.CAP_PROP_FRAME_WIDTH))
    h = int(vidcap.get(cv2.CAP_PROP_FRAME_HEIGHT))

    field = cv2.imread('field.png')
    h_field = field.shape[0]
    w_field = field.shape[1]

    # fourcc = cv2.VideoWriter_fourcc(* 'XVID')  # ファイル形式(ここではmp4)
    fourcc = cv2.VideoWriter_fourcc('m', 'p', '4', 'v')
    writer = cv2.VideoWriter('output_player_tracking.mp4', fourcc,
                             frame_rate, (h, w))  # ライター作成

    field_writer = cv2.VideoWriter('output_field.mp4', fourcc,
                                   frame_rate, (w_field, h_field))  # ライター作成

    src_pts = np.array([[30, 175], [1080, 90], [1790, 322],
                        [1600, 525]], dtype=np.float32)
    dst_pts = np.array([[400, 4], [710, 4], [710, 370],
                        [600, 370]], dtype=np.float32)
    mat = cv2.getPerspectiveTransform(src_pts, dst_pts)

    def transform_plot(mat, point):
        x, y = point
        A = mat[0][0]
        B = mat[0][1]
        C = mat[0][2]
        D = mat[1][0]
        E = mat[1][1]
        F = mat[1][2]
        G = mat[2][0]
        H = mat[2][1]

        x_trans = (A * x + B*y + C) / (G * x + H*y + 1)
        y_trans = (D * x + E*y + F) / (G * x + H*y + 1)

        return int(x_trans), int(y_trans)

    while success:
        red_players_points = []
        blue_players_points = []
        image = cv2.rotate(image, cv2.ROTATE_90_CLOCKWISE)
        hsv = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)
        # green range
        lower_green = np.array([30, 40, 40])
        upper_green = np.array([100, 255, 255])
        lower_red = np.array([0, 180, 150])
        upper_red = np.array([180, 255, 255])
        lower_blue = np.array([70, 60, 200])
        upper_blue = np.array([110, 255, 255])
        lower_white = np.array([0, 0, 0])
        upper_white = np.array([0, 0, 255])

        mask = cv2.inRange(hsv, lower_green, upper_green)
        res = cv2.bitwise_and(image, image, mask=mask)

        res_bgr = cv2.cvtColor(res, cv2.COLOR_HSV2BGR)
        res_gray = cv2.cvtColor(res, cv2.COLOR_BGR2GRAY)

        kernel = np.ones((13, 13), np.uint8)
        thresh = cv2.threshold(
            res_gray, 127, 255, cv2.THRESH_BINARY_INV | cv2.THRESH_OTSU)[1]
        thresh = cv2.morphologyEx(thresh, cv2.MORPH_CLOSE, kernel)

        im2, contours, hierarchy = cv2.findContours(
            thresh, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
        idx = 0
        font = cv2.FONT_HERSHEY_SIMPLEX

        for c in contours:
            x, y, w, h = cv2.boundingRect(c)
    #         if w:
            if (w > 10 and w < 100 and h > 20 and h < 100):
                #             print(w, h)
                #             cv2.rectangle(image,(x,y),(x+w,y+h),(0,0,255),3)

                idx += 1
                player_img = image[y:y+h, x:x+w]

                player_hsv = cv2.cvtColor(player_img, cv2.COLOR_BGR2HSV)
                mask1 = cv2.inRange(player_hsv, lower_blue, upper_blue)
                res1 = cv2.bitwise_and(player_img, player_img, mask=mask1)
                res1 = cv2.cvtColor(res1, cv2.COLOR_HSV2BGR)
                res1 = cv2.cvtColor(res1, cv2.COLOR_BGR2GRAY)
                nzCount = cv2.countNonZero(res1)

                mask2 = cv2.inRange(player_hsv, lower_red, upper_red)
                res2 = cv2.bitwise_and(player_img, player_img, mask=mask2)
                res2 = cv2.cvtColor(res2, cv2.COLOR_HSV2BGR)
                res2 = cv2.cvtColor(res2, cv2.COLOR_BGR2GRAY)
                nzCountred = cv2.countNonZero(res2)

                if(nzCount >= 10):
                    #                 cv2.putText(image, 'team1', (x-2, y-2), font, 0.8, (255,0,0), 2, cv2.LINE_AA)
                    cv2.rectangle(image, (x, y), (x + w, y + h),
                                  (255, 0, 0), 3)
                    blue_players_points.append([x+w//2, y+h//2])
                elif(nzCountred >= 10):

                    # Mark red jersy players as belgium
                    #                 cv2.putText(image, 'team2', (x-2, y-2), font, 0.8, (0,0,255), 2, cv2.LINE_AA)
                    cv2.rectangle(image, (x, y), (x + w, y + h),
                                  (0, 0, 255), 3)
                    red_players_points.append([x+w//2, y+h//2])

        tmpfield = field.copy()
        for points in red_players_points:
            print(points)
            x, y = transform_plot(mat, points)
            print(x, y)
            cv2.circle(tmpfield, (x, y), 1, (0, 0, 255), thickness=10)

        for points in blue_players_points:
            print(points)
            x, y = transform_plot(mat, points)
            print(x, y)
            cv2.circle(tmpfield, (x, y), 1, (255, 0, 0), thickness=10)

        cv2.imshow("output", tmpfield)
        cv2.imshow('Match Detection', image)
        writer.write(image)
        field_writer.write(tmpfield)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
        success, image = vidcap.read()

    vidcap.release()
    writer.release()
    field_writer.release()
    cv2.destroyAllWindows()
