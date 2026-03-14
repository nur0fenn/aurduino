import serial
import time
import cv2

arduino = serial.Serial('COM5', 9600)
time.sleep(2)
print("Connected to arduino")

face_cascade = cv2.CascadeClassifier('haarcascade_frontalface_default.xml')

cap = cv2.VideoCapture(0, cv2.CAP_DSHOW)

cap_width = 500
cap_height = 500

while True:
    ret, img = cap.read()
    cv2.resizeWindow('img', cap_width, cap_height)
    cv2.line(img, (cap_width, cap_height//2), (0, cap_height//2), (0, 255, 0), 1)
    cv2.line(img, (cap_width//2, 0), (cap_width//2, cap_height), (0, 255, 0), 1)
    cv2.circle(img, (cap_width//2, cap_height//2,), 5, (255, 255, 255), -1)

    gray = cv2.cvtColor(img, cv2.COLOR_DGR2GRAY)
    faces = face_cascade.detectMultiScale(gray, 1.3)

    for (x, y, w, h) in faces:
        cv2.rentagle(img, (x, y), (x + w, y + h), (0, 255, 0), 5)
        roi_gray = gray[y:y + h, x:x + w]
        roi_color = img[y:y + h, x:x + w]

        arr = {y:y + h, x:x + w}
        print("----------")
        print("----------")
        print(arr)

        print('X: %d  |  Y:%d' %(x, y))
        print('X+w: %d  |  Y+h:%d' %(x+w, y+h))

        xx = int(x + (x+h)) / 2
        yy = int(y + (y+h)) / 2
        print('xx: %d  |  yy: %d' % (xx, yy))
        center = (xx, yy)

        print("Center of rentagle is :", center)
        data = "X{0:d}Y{1:d}Z".format(xx,yy)
        print("output = '"+ data +"'")
        arduino.write(data)

        cv2.imshow("img", img)

        key = cv2.eaitKey(20)
        if key == 27:
            print("Stop Streaming")
            break

cap.relese()
cv2.destroyAllWindows()