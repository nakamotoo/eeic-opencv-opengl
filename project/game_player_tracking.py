
import cv2
import numpy as np
from IPython.display import display, Image


vidcap = cv2.VideoCapture('game2.mov')
success, image = vidcap.read()


frame_rate = int(vidcap.get(cv2.CAP_PROP_FPS))
w = int(vidcap.get(cv2.CAP_PROP_FRAME_WIDTH))
h = int(vidcap.get(cv2.CAP_PROP_FRAME_HEIGHT))


# fourcc = cv2.VideoWriter_fourcc(* 'XVID')  # ファイル形式(ここではmp4)
fourcc = cv2.VideoWriter_fourcc('m', 'p', '4', 'v')
writer = cv2.VideoWriter('output.mp4', fourcc,
                         frame_rate, (h, w))  # ライター作成


while success:
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
                cv2.rectangle(image, (x, y), (x + w, y + h), (255, 0, 0), 3)
            elif(nzCountred >= 10):

                # Mark red jersy players as belgium
                #                 cv2.putText(image, 'team2', (x-2, y-2), font, 0.8, (0,0,255), 2, cv2.LINE_AA)
                cv2.rectangle(image, (x, y), (x+w, y+h), (0, 0, 255), 3)

    cv2.imshow('Match Detection', image)
    writer.write(image)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break
    success, image = vidcap.read()

vidcap.release()
writer.release()
cv2.destroyAllWindows()
