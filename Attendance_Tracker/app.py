import customtkinter #App built using customtkinter GUI
import sqlite3 #User info stored using sqlite3 database
from PIL import Image #Icons generated using Image from PIL
import datetime #Generating dates and timestamps for logging
import CTkTable #Display logged in users
import pandas #Generates Excel files from SQLite data
import openpyxl #Needed by Pandas

#Customtkinter initializations
customtkinter.set_appearance_mode("Dark")  # Modes: "System" (standard), "Dark", "Light"
customtkinter.set_default_color_theme("blue")  # Themes: "blue" (standard), "green", "dark-blue"

#Icon Definitions
close_btn_img = customtkinter.CTkImage(light_image=Image.open("Icons/close_btn_light.png"), dark_image=Image.open("Icons/close_btn_dark.png"), size=(20,20))
left_arrow_img = customtkinter.CTkImage(dark_image=Image.open("Icons/left_arrow.png"), light_image=None, size=(60, 80))

#Global Definitions
activeFrameLst = []
activeUsers = []

#Sign Up Popup
class SignUpWindow(customtkinter.CTkFrame):
    def __init__(self, container):
        #Initializations and Placement
        super().__init__(container)
        self.container = container
        self.place(relwidth=0.5, relheight=0.8, relx=0.25, rely=0.1)
        activeFrameLst.append(self)

        #Popup GUI
        self.close_button=customtkinter.CTkButton(self, image = close_btn_img, fg_color="transparent", text=None, command=lambda:[forgetFrame(self)])
        self.close_button.place(relwidth=0.1, relheight=0.1, relx=0.9, rely=0.02)

        self.first_name = customtkinter.CTkEntry(self, placeholder_text="First Name", font=generateFont('Roboto', 20), justify='center')
        self.first_name.setvar("name0", "First Name")
        self.first_name.place(relwidth=0.25, relheight=0.1, relx=0.163, rely=0.1)

        self.last_name = customtkinter.CTkEntry(self, placeholder_text="Last Name",font=generateFont('Roboto', 20), justify='center')
        self.last_name.setvar("name1", "Last Name")        
        self.last_name.place(relwidth=0.25, relheight=0.1, relx=0.576, rely=0.1)

        self.grade = customtkinter.CTkEntry(self, placeholder_text="Grade",font=generateFont('Roboto', 20), justify='center')
        self.grade.setvar("name2", "Grade")        
        self.grade.place(relwidth=0.25, relheight=0.1, relx=0.163, rely=0.25)

        self.phone_number = customtkinter.CTkEntry(self, placeholder_text="Phone Number",font=generateFont('Roboto', 20), justify='center')
        self.phone_number.setvar("name3", "Phone Number")        
        self.phone_number.place(relwidth=0.25, relheight=0.1, relx=0.576, rely=0.25)

        self.team = customtkinter.CTkEntry(self, placeholder_text="Team",font=generateFont('Roboto', 20), justify='center')
        self.team.setvar("name4", "Team")        
        self.team.place(relwidth=0.25, relheight=0.1, relx=0.375, rely=0.4)

        self.signUpBtn = customtkinter.CTkButton(self, border_width=2, text="Sign Up", font=generateFont('Roboto', 20), text_color=("gray10", "#DCE4EE"), command=lambda:self.signUp())
        self.signUpBtn.place(relwidth=0.2, relheight=0.1, relx=0.4, rely=0.6)

    #Sign Up Function - Logistic Handler
    def signUp(self):
        #Opens database and retrieves all user last names
        conn = sqlite3.connect('main.sqlite')
        cursor = conn.cursor()
        res = cursor.execute("SELECT last_name from users")
        allNames = res.fetchall()

        #Creates a new row in the users table with inputted data if the inputted last name is unique, launches the sign in window with predefined data if succeeds
        lastNameIsUnique = True
        self.form_options = [self.first_name.get(), self.last_name.get(), int(self.grade.get()), self.phone_number.get(), self.team.get()]
        for lst in allNames:
            if(lst[0] == self.last_name.get()):
                lastNameIsUnique = False

        if(lastNameIsUnique):
            cursor.execute("INSERT INTO users (first_name, last_name, grade, phone_number, team) VALUES (?, ?, ?, ?, ?)", self.form_options)
            conn.commit()
            conn.close()
            forgetFrame(self)
            loadNotification(self.container, "lime green", "black", "Welcome New User - " + self.first_name.get(), 2000)
            SignInWindow(self.container, self.form_options)
        else:
            forgetFrame(self)
            loadNotification(self.container, "red", "black", "Error - Sign Up Failed", 2000)

class SignOutWindow(customtkinter.CTkFrame):
    def __init__(self, container):
        super().__init__(container)
        self.container = container
        self.place(relwidth=0.5, relheight=0.5, relx=0.25, rely=0.25)
        activeFrameLst.append(self)

        self.close_button=customtkinter.CTkButton(self, image = close_btn_img, fg_color="transparent", text=None, command=lambda:[forgetFrame(self)])
        self.close_button.place(relwidth=0.1, relheight=0.1, relx=0.9, rely=0.02)

        self.first_name = customtkinter.CTkEntry(self, placeholder_text="First Name", font=generateFont('Roboto', 20), justify='center')
        self.first_name.setvar("name0", "First Name")
        self.first_name.place(relwidth=0.25, relheight=0.1, relx=0.163, rely=0.1)

        self.last_name = customtkinter.CTkEntry(self, placeholder_text="Last Name",font=generateFont('Roboto', 20), justify='center')
        self.last_name.setvar("name1", "Last Name")        
        self.last_name.place(relwidth=0.25, relheight=0.1, relx=0.576, rely=0.1)

        self.form_options = [self.first_name, self.last_name]

        self.logOutBtn = customtkinter.CTkButton(self, border_width=2, text="Log Out", font=generateFont('Roboto', 20), text_color=("gray10", "#DCE4EE"), command=lambda:self.logout())
        self.logOutBtn.place(relwidth=0.2, relheight=0.1, relx=0.4, rely=0.6)

    def logout(self):
        for user in activeUsers:
            print(user.first_name, user.last_name, self.first_name.get(), self.last_name.get())
            if(user.first_name == self.first_name.get() and user.last_name == self.last_name.get()):
                activeUsers.remove(user)
                loadNotification(self.container, "lime green", "black", "Goodbye - " + user.first_name, 2000)
                self.container.signOut(user)
            else:
                loadNotification(self.container, "red", "black", "Error - Logout Failed", 2000)
        forgetFrame(self)
       

class SignInWindow(customtkinter.CTkFrame):
    def __init__(self, container, prefilledUserInfo=None):
        super().__init__(container)
        self.container = container
        self.place(relwidth=0.5, relheight=0.5, relx=0.25, rely=0.25)
        activeFrameLst.append(self)
        self.prefilledUserInfo = prefilledUserInfo
        if(prefilledUserInfo != None):
            self.login()

        self.close_button=customtkinter.CTkButton(self, image = close_btn_img, fg_color="transparent", text=None, command=lambda:[forgetFrame(self)])
        self.close_button.place(relwidth=0.1, relheight=0.1, relx=0.9, rely=0.02)

        self.first_name = customtkinter.CTkEntry(self, placeholder_text="First Name", font=generateFont('Roboto', 20), justify='center')
        self.first_name.setvar("name0", "First Name")
        self.first_name.place(relwidth=0.25, relheight=0.1, relx=0.163, rely=0.1)

        self.last_name = customtkinter.CTkEntry(self, placeholder_text="Last Name",font=generateFont('Roboto', 20), justify='center')
        self.last_name.setvar("name1", "Last Name")        
        self.last_name.place(relwidth=0.25, relheight=0.1, relx=0.576, rely=0.1)

        self.form_options = [self.first_name, self.last_name]

        self.logInBtn = customtkinter.CTkButton(self, border_width=2, text="Log In", font=generateFont('Roboto', 20), text_color=("gray10", "#DCE4EE"), command=lambda:self.login())
        self.logInBtn.place(relwidth=0.2, relheight=0.1, relx=0.4, rely=0.6)

        self.newAccnBtn = customtkinter.CTkButton(self, fg_color="transparent", text="New? Sign Up Here", font=generateFont('Roboto', 20), text_color="cyan", command=lambda:[forgetFrame(self), SignUpWindow(self.container)])
        self.newAccnBtn.place(relwidth=0.2, relheight=0.1, relx=0.4, rely=0.75)

    def login(self):
        conn = sqlite3.connect('main.sqlite')
        cursor = conn.cursor()
        if(self.prefilledUserInfo != None):
            res = cursor.execute("SELECT * from users WHERE first_name=? AND last_name=?", [self.prefilledUserInfo[0], self.prefilledUserInfo[1]])
        else:
            res = cursor.execute("SELECT * from users WHERE first_name=? AND last_name=?", [self.first_name.get(), self.last_name.get()])
        activeUserInfo = res.fetchall()
        print(activeUserInfo)
        if(len(activeUserInfo) > 0):
            activeUser = User(*activeUserInfo[0])
            activeUsers.append(activeUser)
            loadNotification(self.container, "lime green", "black", "Welcome - " + activeUser.first_name, 2000)
            forgetFrame(self)
            self.container.signIn(activeUser)
        else:   
            forgetFrame(self)
            loadNotification(self.container, "lime red", "black", "Error - Login Failed", 2000)

class AttendanceLog(customtkinter.CTkFrame):

    def __init__(self, container):
        
        super().__init__(container)
        self.container = container
        self.place(relwidth=1, relheight=1)
        activeFrameLst.append(self)

        self.backBtn=customtkinter.CTkButton(self, image = left_arrow_img, fg_color="transparent", text=None, command=lambda:[forgetAllFrames()])
        self.backBtn.place(relwidth=0.05, relheight=0.1, relx=0.01, rely=0.02)

        self.genExcelBtn = customtkinter.CTkButton(self, text='Update Excel', font=generateFont('Roboto', 20), command=lambda:[genExcelFiles(self.container)])
        self.genExcelBtn.place(relx=0.85, rely=0.04, relwidth=0.1, relheight=0.05)
        self.signInBtn = customtkinter.CTkButton(self, text='Sign In', font=generateFont('Roboto', 20), command=lambda:[SignInWindow(self)])
        self.signInBtn.place(relx=0.85, rely=0.14, relwidth=0.1, relheight=0.05)
        self.signOutBtn = customtkinter.CTkButton(self, text='Sign Out', font=generateFont('Roboto', 20), command=lambda:[SignOutWindow(self)])
        self.signOutBtn.place(relx=0.85, rely=0.24, relwidth=0.1, relheight=0.05)
        self.tableFrame = customtkinter.CTkFrame(self)
        self.tableFrame.place(rely=0.3, relwidth=1, relheight=0.65)
        self.logTable = CTkTable.CTkTable(master=self.tableFrame, row=1, column=4, header_color="maroon" , values=[["Date", "Time In", "Time Out", "Name"]])
        self.logTable.pack(expand=False, fill="x", padx=20, pady=20, side="top")
        self.lastRowIndex = 1

    def signIn(self, user): 
        conn = sqlite3.connect('main.sqlite')
        cursor = conn.cursor()
        currentDateTime = datetime.datetime.today().time()
        currentDay = datetime.date.today()
        print(currentDateTime, currentDay)
        cursor.execute('''
          INSERT INTO attendance (day, time_in, user_id, user_first_name, user_last_name) VALUES (?, ?, ?, ?, ?)             
        ''', [currentDay, str(currentDateTime), user.id, user.first_name, user.last_name])
        conn.commit()
        conn.close()
        self.logTable.add_row(index=self.lastRowIndex, values=[str(currentDay), str(currentDateTime), "N/A", user.first_name + " " + user.last_name])
        user.row = self.lastRowIndex
        self.lastRowIndex += 1
        loadNotification(self.container, "lime green", "black", "Login Info Added For " + user.first_name, 2000, rely=0.1)

    def signOut(self, user):
        conn = sqlite3.connect('main.sqlite')
        cursor = conn.cursor()
        currentDateTime = datetime.datetime.today().time()
        currentDay = datetime.date.today()
        cursor.execute('''
          UPDATE attendance SET time_out=? WHERE user_id=? AND day=?             
        ''', [str(currentDateTime), user.id, currentDay])
        conn.commit()
        conn.close()
        self.logTable.insert(user.row, 2, str(currentDateTime))
        loadNotification(self.container, "lime green", "black", "Time Out Added For " + user.first_name, 2000, rely=0.1)

class Main(customtkinter.CTk):
    """  
    
    class Main() - customTkinter Master Window serves as home page that opens upon program start

        def __init__() - Configures the Master Window as well as the UI for the home menu
    
        def initDb() - Ensures the main.db database and "students", "courses", and "assignments" tables have been created

    """

    def __init__(self):
        super().__init__()
        
        # configure window
        self.title("Attendance Tracker")
        self.geometry("%dx%d+0+0" % (self.winfo_screenwidth(), self.winfo_screenheight()))
        self.initDb()

        #Home UI
        self.titleUI = customtkinter.CTkLabel(self, text="Attendance Tracker vDEV", font=generateFont('Roboto', 64))
        self.titleUI.place(relx=0, rely=0.1, relwidth=1)

        self.attendanceLogBtn = customtkinter.CTkButton(self, text='Launch Attendance Log', font=generateFont('Roboto', 20), command=lambda:[loadFrame(AttendanceLog, self)])
        self.attendanceLogBtn.place(relx=0.4, rely=0.3, relwidth=0.2, relheight=0.05)

        self.generateExcelBtn = customtkinter.CTkButton(self, text='Generate Excel Files', font=generateFont('Roboto', 20), command=lambda:[genExcelFiles(self)])
        self.generateExcelBtn.place(relx=0.4, rely=0.4, relwidth=0.2, relheight=0.05)

    def initDb(self):
        # Connect to the database, creates if not exists
        conn = sqlite3.connect('main.sqlite')
        cursor = conn.cursor()

        # Create the users table if not exists
        cursor.execute('''
            CREATE TABLE IF NOT EXISTS users (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                first_name TEXT NOT NULL,
                last_name TEXT UNIQUE NOT NULL,
                grade INTEGER NOT NULL,
                phone_number TEXT UNIQUE,
                team TEXT
            )
        ''')

        #Create the attendance table if not exists
        cursor.execute('''
            CREATE TABLE IF NOT EXISTS attendance (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                day TIMESTAMP,
                time_in TEXT,
                time_out TEXT,
                user_id INTEGER NOT NULL,
                user_first_name TEXT NOT NULL,
                user_last_name TEXT NOT NULL,
                FOREIGN KEY (user_id) REFERENCES users (id)
                FOREIGN KEY (user_first_name) REFERENCES users (first_name)
                FOREIGN KEY (user_last_name) REFERENCES users (last_name)
            )
        ''')

        # Commit the changes and close the connection
        conn.commit()
        conn.close()

class User():
    def __init__(self, id:int, first_name:str, last_name:str, grade:int, phone_number:str, team:str):
        self.id = id
        self.first_name = first_name
        self.last_name = last_name
        self.grade = grade
        self.phone_number = phone_number
        self.team = team
        self.row = -1

def generateFont(fontName:str, fontSize:int, weight:str = 'normal', slant:str = 'roman', underline:bool = False, overstrike:bool = False):
    return customtkinter.CTkFont(family = fontName, size = fontSize, weight=weight, slant=slant, underline=underline, overstrike=overstrike)

def forgetAllFrames():
    for frame in activeFrameLst:
        frame.place_forget()
        activeFrameLst.remove(frame)

def forgetFrame(frame):
    frame.place_forget()
    activeFrameLst.remove(frame)

def loadFrame(frame, container):
        forgetAllFrames()
        frame(container)

def genExcelFiles(container):
    try:
        conn = sqlite3.connect('main.sqlite')
        usersSqlQuery = "SELECT first_name, last_name, grade, phone_number, team from users;"
        usersDf_read = pandas.read_sql(usersSqlQuery, conn)
        usersDf_read.to_excel("Members.xlsx")

        attendanceSqlQuery = "SELECT day, time_in, time_out, user_first_name, user_last_name from attendance;"
        attendanceDf_read = pandas.read_sql(attendanceSqlQuery, conn)
        attendanceDf_read.to_excel("Attendance.xlsx")
        loadNotification(container, "lime green", "black", "Success - Attendance.xlsx & Members.xlsx Created", 2000)

    except Exception as e:
        loadNotification(container, "red", "black", "Fail - " + str(e), 2000)

def loadNotification(container, bgColor, textColor, text, msec, rely=0.01):
    notification = customtkinter.CTkLabel(master=container, bg_color=bgColor, text_color=textColor, font=generateFont('Roboto', 12), text=text)
    notification.place(relwidth=0.2, relheight=0.05, relx=0.8, rely=rely)
    notification.after(ms=msec, func=lambda:[notification.place_forget()])

#Launches Application
if __name__ == "__main__":
    app = Main()
    # validateEntryCommand = app.register(validateEntry)
    app.mainloop()
