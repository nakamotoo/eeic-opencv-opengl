
import cv2
import numpy as np
from IPython.display import display, Image


vidcap = cv2.VideoCapture('input2.mp4')
success, image = vidcap.read()
count = 0
success = True
idx = 0


while success:
    hsv = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)
    # green range
    lower_green = np.array([0, 40, 40])
    upper_green = np.array([70, 255, 255])
    lower_red = np.array([0, 31, 255])
    upper_red = np.array([30, 255, 255])
    lower_blue = np.array([110, 50, 50])
    upper_blue = np.array([130, 255, 255])
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
        if((w > 20) and (h >= 20) and (h >= (1.1)*w)):
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
                cv2.putText(image, 'Japan', (x-2, y-2), font,
                            0.8, (255, 0, 0), 2, cv2.LINE_AA)
                cv2.rectangle(image, (x, y), (x+w, y+h), (255, 0, 0), 3)
            elif(nzCountred >= 10):
                # Mark red jersy players as belgium
                cv2.putText(image, 'Belgium', (x-2, y-2), font,
                            0.8, (0, 0, 255), 2, cv2.LINE_AA)
                cv2.rectangle(image, (x, y), (x + w, y + h), (0, 0, 255), 3)

    cv2.imshow('Match Detection', image)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break
    success, image = vidcap.read()

vidcap.release()
cv2.destroyAllWindows()
