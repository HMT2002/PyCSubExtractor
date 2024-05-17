import cv2
import os,argparse
import pytesseract
import pathlib
from PIL import Image
import easyocr
import math

def format_a_line(text,timeFrom,timeTo,index):
  milisecTimeFrom=timeFrom%1000
  secTimeFrom=timeFrom//1000
  hourTimeFrom=int(round(secTimeFrom//(60*60)))
  secTimeFrom=secTimeFrom-(hourTimeFrom*(60*60))
  minuteTimeFrom=int(round(secTimeFrom//(60)))
  secTimeFrom = secTimeFrom- (minuteTimeFrom *(60))
  milisecTimeTo=timeTo%1000
  secTimeTo=timeTo//1000
  hourTimeTo=int(round(secTimeTo//(60*60)))
  secTimeTo=secTimeTo-(hourTimeTo*(60*60))
  minuteTimeTo=int(round(secTimeTo//(60)))
  secTimeTo = secTimeTo- (minuteTimeTo *(60))
  if hourTimeFrom<10:
     hourTimeFrom="0"+str(hourTimeFrom)
  if hourTimeTo<10:
     hourTimeTo="0"+str(hourTimeTo)
  if minuteTimeFrom<10:
     minuteTimeFrom="0"+str(minuteTimeFrom)
  if minuteTimeTo<10:
     minuteTimeTo="0"+str(minuteTimeTo)
  if secTimeFrom<10:
     secTimeFrom="0"+str(secTimeFrom)
  if secTimeTo<10:
     secTimeTo="0"+str(secTimeTo)
  if milisecTimeFrom<100:
     if milisecTimeFrom<10:
       milisecTimeFrom="00"+str(milisecTimeFrom)
     else:
       milisecTimeFrom="0"+str(milisecTimeFrom)

  if milisecTimeTo<100:
     if milisecTimeTo<10:
       milisecTimeTo="00"+str(milisecTimeTo)
     else:
       milisecTimeTo="0"+str(milisecTimeTo)


  writeText=str(index)+"\n"+str(hourTimeFrom)+":"+str(minuteTimeFrom)+":"+str(secTimeFrom)+","+str(milisecTimeFrom)+" --> "+str(hourTimeTo)+":"+str(minuteTimeTo)+":"+str(secTimeTo)+","+str(milisecTimeTo)+"\n "+text+"\n"+"\n"
  print(writeText)
  return writeText

#We then Construct an Argument Parser
ap=argparse.ArgumentParser()
ap.add_argument("-i","--image",
                required=True,
                help="Path to the image folder")
ap.add_argument("-f","--folder",
                required=True,
                help="Path to the folder")
ap.add_argument("-p","--pre_processor",
                default="thresh", 
                help="the preprocessor usage")
args=vars(ap.parse_args())
single_image=args["image"]
folder_image=args["folder"]
# init easyocr
# reader = easyocr.Reader(['vi']) # this needs to run only once to load the model into memory
reader = easyocr.Reader(['vi','en'])

step=0
index=1
#read files in folder
path = str(pathlib.Path().resolve())+'\\'+folder_image
print(path)
dir_list = os.listdir(path)
print("Files and directories in '", path, "' :")
# prints all files
for file in dir_list:
  #We then read the image with text
  images=cv2.imread(path+'\\'+file)
  #convert to grayscale image
  gray=cv2.cvtColor(images, cv2.COLOR_BGR2GRAY)
  #checking whether thresh or blur
  if args["pre_processor"]=="thresh":
      cv2.threshold(gray, 0,255,cv2.THRESH_BINARY| cv2.THRESH_OTSU)[1]
  if args["pre_processor"]=="blur":
      cv2.medianBlur(gray, 3) 
  #memory usage with image i.e. adding image to memory
  filename = "{}.jpg".format(os.getpid())
#   print(file)
  cv2.imwrite(filename, gray)
  # text = pytesseract.image_to_string(Image.open(filename),lang='vie')
  # print(text)
#   print("EasyOCR")
#   print("output"+file)
  result = reader.readtext(folder_image+"/"+file)
  os.remove(filename)
#   print(result)
  if len(result)>0:
    if len(result[0])>1:
      sub=format_a_line(result[0][1],step,step+500,index)
      with open("sub.srt", 'a+', encoding="utf-8") as f:
        f.write(sub)
        index=index+1

  step=step+500

# show the output images
# cv2.imshow("Image Input", images)
# cv2.imshow("Output In Grayscale", gray)
# cv2.waitKey(0)
input("Press Enter to continue...")
