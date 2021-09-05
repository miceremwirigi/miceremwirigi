import serial                                #import serial library
import matplotlib.pyplot as plt              #import matplotlib
import numpy as np                           #import numpy
from drawnow import *                        #import drawnow
import mysql.connector                       #import mysql connector
import datetime as dt                        #import datetime
import calendar                              #import calender

tempA =[]               
hmdtA =[]
modeA =[] 
arduinoData = serial.Serial('/dev/ttyACM0', 115200) #set serial port to read from
plt.ion() #tel matplotlib we want to plot live data in interactive mode
count = 0
count2 = 0
curr_mode = ''

def makeChart(): #creates a function that plots our data
    plt.ylim(23,27)                                       #set y limit of graph
    plt.xlim(0,25)
    plt.suptitle('Live Feed')
    plt.title('Temp and Humidity Live Date')              #set title of grap
    plt.grid(True)                                        #show graph
    plt.ylabel('Temp C')                                  #set label
    plt.plot(tempA,'r-', )
    plt.plot(tempA,'bo', label = 'Degrees Celcius')     #plot red line blue dots
    plt.legend(loc = 'upper left')                      #plot legend on upper left
    plt.tight_layout()
    
        

    plt2 = plt.twinx()                                  #create twin x axis

    plt.ylim(60,100)                                     #set y limit of second plot
    plt.ylabel('Humidity %')                             #set y label for second plot
    plt2.plot(hmdtA,'b-', )                              #plot hmdt values
    plt2.plot(hmdtA,'g^', label = 'Percent')              #plot red line blue dots
    if (int(modeA[0]) == 0):
        plt2.text(0,65,'Light Mode', style = 'oblique', color =  'blue', fontsize = 25,
              bbox={'facecolor': 'red','alpha': 0.4})
        
    if (int(modeA[0]) == 1):
        plt2.text(0,65,'Motion Mode', style = 'italic', color =  'purple', fontsize = 25,
              bbox={'facecolor': 'red','alpha': 0.4})
        
    if (int(modeA[0]) == 2):
        plt2.text(0,65,'Dark Mode', style = 'normal', color =  'green', fontsize = 25,
              bbox={'facecolor': 'red','alpha': 0.4})
        
    plt2.legend(loc = 'upper right')                     #plot legend on upper right

while True:
    while (arduinoData.inWaiting() == 0):   #check if there us data from arduino
        pass
    arduinoString = arduinoData.readline().decode('utf-8').rstrip()  #read and decode string read from arduino

    
    dataArray = arduinoString.split(',')   #split string from arduino into an array
    temp = float(dataArray[2])             #set first dataArray value as a float and store in temp
    hmdt = float (dataArray[3])            #set second dataArray value as a float and store in hmdt
    mode = int (dataArray[1])
    tempA.append(temp)                     #store temp data into tempF array
    hmdtA.append(hmdt)                     #store hmdt data into hmdtF array
    modeA.append(mode)
    
    
    print (modeA)
    print (tempA)
    print (hmdtA)
    drawnow(makeChart)                     #call drawnow to update our live data
    plt.pause(0.00001)
    
                     #Syncing data to Mysql Database SmartHouse Table LiveFeed

    
    if (count2%100 == 0):
        print(modeA[0], "\n\n")
        if (int(modeA[0]) == 0):
            curr_mode = 'Light Mode'                              #Declare string variable to store mode
        if (int(modeA[0]) == 1):
            curr_mode = 'Motion Mode'
        if (int(modeA[0]) == 2):
            curr_mode = 'Dark Mode'
        
        #Getting current time,day and date
        curr_time = dt.datetime.now().time()
        curr_date = dt.datetime.now().date()
        curr_day = calendar.day_name[curr_date.weekday()]

        #establishing mysql connection
        conn = mysql.connector.connect(
           user='mwirigi', password='qwerty123456', host='127.0.0.1', database='SmartHouse')

        #Creating a cursor object using the cursor() method
        cursor = conn.cursor()

        # Preparing SQL query to INSERT a record into the database.
        sql = """INSERT INTO LiveFeed (
           HUMIDITY, TEMPERATURE, MODE, TIME, DAY, DATE)
           VALUES ({},{}, '{}', '{}', '{}', '{}')""".format(hmdtA[0],tempA[0],curr_mode,curr_time, curr_day, curr_date)

        try:
           # Executing the SQL command
           cursor.execute(sql)

           # Commit your changes in the database
           conn.commit()

        except:
           # Rolling back in case of error
           conn.rollback()

        #Show table
        print('Table is: ')
        cursor.execute('select * from LiveFeed')
        print(cursor.fetchall())

        # Closing the connection
        conn.close()
    
    count = count + 1
    if (count > 0):
        modeA.pop(0)
    if (count > 25):
        tempA.pop(0)
        hmdtA.pop(0)
    
    count2 = count2 + 1
             

    
    
    
