import customtkinter #App built using customtkinter GUI
import sqlite3 #User info stored using sqlite3 database
from PIL import Image #Icons generated using Image from PIL
import datetime #Generating dates and timestamps for logging
import CTkTable #Display logged in users
import pandas #Generates Excel files from SQLite data
import openpyxl #Needed by Pandas
import webbrowser #Opens License URL

#Customtkinter initializations
customtkinter.set_appearance_mode("Dark")  # Modes: "System" (standard), "Dark", "Light"
customtkinter.set_default_color_theme("blue")  # Themes: "blue" (standard), "green", "dark-blue"

#Icon Definitions
close_btn_img = customtkinter.CTkImage(light_image=Image.open("Icons/close_btn_light.png"), dark_image=Image.open("Icons/close_btn_dark.png"), size=(20,20))
left_arrow_img = customtkinter.CTkImage(dark_image=Image.open("Icons/left_arrow.png"), light_image=None, size=(60, 80))

#Global Definitions
activeFrameLst = []
activeUsers = []

class SignUpWindow(customtkinter.CTkFrame):
    """
    This class represents the Sign Up GUI popup and associated functionality.
    
    Inherits: customtkinter.CTkFrame

    Attributes:
        container (customtkinter.CTkFrame): AttendanceLog() instance - the parent container of self
        closeBtn (customtkinter.CTkButton): Unloads this frame (self)
        firstNameEntry (customtkinter.CTkEntry): Textbox entry for user's first name
        lastNameEntry (customtkinter.CTkEntry): Textbox entry for user's last name
        gradeEntry (customtkinter.CTkEntry): Textbox entry for user's numeric grade (Likely 8-12)
        phoneNumberEntry (customtkinter.CTkEntry): Textbox entry for user's phone number
        teamEntry (customtkinter.CTkEntry): Textbox entry for user's team
        signUpBtn (customtkinter.CTkButton): Calls the sign up function
    """

    def __init__(self, container):
        """
        The constructor for the SignUpWindow class that creates and places GUI elements of the popup.

        Parameters: 
            self (customtkinter.CTkFrame): the CTkFrame instance of SignUpWindow
            container (customtkinter.CTkFrame): AttendanceLog() instance - the parent container of self

        Returns: void
        """

        #Initializations and Placement
        super().__init__(container)
        self.container = container
        self.place(relwidth=0.5, relheight=0.8, relx=0.25, rely=0.1)
        activeFrameLst.append(self)

        #Popup GUI
        self.closeBtn = customtkinter.CTkButton(self, image = close_btn_img, fg_color="transparent", text=None, command=lambda:[forgetFrame(self)])
        self.closeBtn.place(relwidth=0.1, relheight=0.1, relx=0.9, rely=0.02)

        self.firstNameEntry = customtkinter.CTkEntry(self, placeholder_text="First Name", font=generateFont('Roboto', 20), justify='center')
        self.firstNameEntry.setvar("name0", "First Name")
        self.firstNameEntry.place(relwidth=0.25, relheight=0.1, relx=0.163, rely=0.1)

        self.lastNameEntry = customtkinter.CTkEntry(self, placeholder_text="Last Name",font=generateFont('Roboto', 20), justify='center')
        self.lastNameEntry.setvar("name1", "Last Name")        
        self.lastNameEntry.place(relwidth=0.25, relheight=0.1, relx=0.576, rely=0.1)

        self.gradeEntry = customtkinter.CTkEntry(self, placeholder_text="Grade",font=generateFont('Roboto', 20), justify='center')
        self.gradeEntry.setvar("name2", "Grade")        
        self.gradeEntry.place(relwidth=0.25, relheight=0.1, relx=0.163, rely=0.25)

        self.phoneNumberEntry = customtkinter.CTkEntry(self, placeholder_text="Phone Number",font=generateFont('Roboto', 20), justify='center')
        self.phoneNumberEntry.setvar("name3", "Phone Number")        
        self.phoneNumberEntry.place(relwidth=0.25, relheight=0.1, relx=0.576, rely=0.25)

        self.teamEntry = customtkinter.CTkEntry(self, placeholder_text="Team",font=generateFont('Roboto', 20), justify='center')
        self.teamEntry.setvar("name4", "Team")        
        self.teamEntry.place(relwidth=0.25, relheight=0.1, relx=0.375, rely=0.4)

        self.signUpBtn = customtkinter.CTkButton(self, border_width=2, text="Sign Up", font=generateFont('Roboto', 20), text_color=("gray10", "#DCE4EE"), command=lambda:self.signUp())
        self.signUpBtn.place(relwidth=0.2, relheight=0.1, relx=0.4, rely=0.6)
    
    def signUp(self):
        """
        The registration handler function called by pressing the self.signUpBtn of the SignUpWindow instance. 

        Parameters: 
            self (customtkinter.CTkFrame): the CTkFrame instance of SignUpWindow

        Returns: void
        """

        #Opens database and retrieves all user last names
        conn = sqlite3.connect('main.sqlite')
        cursor = conn.cursor()
        res = cursor.execute("SELECT last_name from users")
        allNames = res.fetchall()

        #Creates a new row in the users table with inputted data if the inputted last name is unique, launches the sign in window with predefined data if succeeds
        lastNameIsUnique = True
        self.form_options = [self.firstNameEntry.get(), self.lastNameEntry.get(), int(self.gradeEntry.get()), self.phoneNumberEntry.get(), self.teamEntry.get()]
        for lst in allNames:
            if(lst[0] == self.lastNameEntry.get()):
                lastNameIsUnique = False

        if(lastNameIsUnique):
            cursor.execute("INSERT INTO users (first_name, last_name, grade, phone_number, team) VALUES (?, ?, ?, ?, ?)", self.form_options)
            conn.commit()
            conn.close()
            forgetFrame(self)
            loadNotification(self.container, "lime green", "black", "Welcome New User - " + self.firstNameEntry.get(), 2000)
            SignInWindow(self.container, self.form_options)
        else:
            forgetFrame(self)
            loadNotification(self.container, "red", "black", "Error - Sign Up Failed", 2000)

class SignOutWindow(customtkinter.CTkFrame):
    """
    This class represents the Sign Out GUI popup and associated functionality. 
    
    Inherits: customtkinter.CTkFrame

    Attributes:
        container (customtkinter.CTkFrame):  AttendanceLog() instance - the parent container of self
        closeBtn (customtkinter.CTkButton): Unloads this frame (self)
        firstNameEntry (customtkinter.CTkEntry): Textbox entry for user's first name
        lastNameEntry (customtkinter.CTkEntry): Textbox entry for user's last name
        logoutBtn (customtkinter.CTkButton): Calls the logout function
    """

    def __init__(self, container):
        """
        The constructor for the SignUpWindow class that creates and places GUI elements of the popup.

        Parameters: 
            self (customtkinter.CTkFrame): the CTkFrame instance of SignOutWindow
            container (customtkinter.CTkFrame): AttendanceLog() instance - the parent container of self

        Returns: void
        """

        #Initializations and Placement
        super().__init__(container)
        self.container = container
        self.place(relwidth=0.5, relheight=0.5, relx=0.25, rely=0.25)
        activeFrameLst.append(self)

        #Popup GUI
        self.closeBtn=customtkinter.CTkButton(self, image = close_btn_img, fg_color="transparent", text=None, command=lambda:[forgetFrame(self)])
        self.closeBtn.place(relwidth=0.1, relheight=0.1, relx=0.9, rely=0.02)

        self.firstNameEntry = customtkinter.CTkEntry(self, placeholder_text="First Name", font=generateFont('Roboto', 20), justify='center')
        self.firstNameEntry.setvar("name0", "First Name")
        self.firstNameEntry.place(relwidth=0.25, relheight=0.1, relx=0.163, rely=0.1)

        self.lastNameEntry = customtkinter.CTkEntry(self, placeholder_text="Last Name",font=generateFont('Roboto', 20), justify='center')
        self.lastNameEntry.setvar("name1", "Last Name")        
        self.lastNameEntry.place(relwidth=0.25, relheight=0.1, relx=0.576, rely=0.1)

        self.logOutBtn = customtkinter.CTkButton(self, border_width=2, text="Log Out", font=generateFont('Roboto', 20), text_color=("gray10", "#DCE4EE"), command=lambda:self.logout())
        self.logOutBtn.place(relwidth=0.2, relheight=0.1, relx=0.4, rely=0.6)

    def logout(self):
        """
        The logout handler function called by pressing the self.logOutBtn of the SignOutWindow instance. 

        Parameters: 
            self (customtkinter.CTkFrame): the CTkFrame instance of SignOutWindow

        Returns: void
        """

        for user in activeUsers:
            if(user.first_name == self.firstNameEntry.get() and user.last_name == self.lastNameEntry.get()):
                activeUsers.remove(user)
                loadNotification(self.container, "lime green", "black", "Goodbye - " + user.first_name, 2000)
                self.container.signOut(user)
            else:
                loadNotification(self.container, "red", "black", "Error - Logout Failed", 2000)
        forgetFrame(self) 

class SignInWindow(customtkinter.CTkFrame):
    """
    This class represents the Sign In GUI popup and associated functionality.
    
    Inherits: customtkinter.CTkFrame

    Attributes:
    container (customtkinter.CTk): Main() instance - the parent container of self
    prefilledUserInfo (List): User's first and last name that is prefilled (i.e., when SignUpWindow internally calls self, it passes in the user data from SignUpWindow's entries)
    closeBtn (customtkinter.CTkButton): Unloads this frame (self)
    firstNameEntry (customtkinter.CTkEntry): Textbox entry for user's first name
    lastNameEntry (customtkinter.CTkEntry): Textbox entry for user's last name
    loginBtn (customtkinter.CTkButton): Calls the login function
    newAccnBtn (customtkinter.CTkButton): Unloads this frame (self) and loads an instance of SignUpWindow
    """

    def __init__(self, container, prefilledUserInfo=None):
        """
        The constructor for the SignInWindow class that creates and places GUI elements of the popup.

        Parameters: 
            self (customtkinter.CTkFrame): the CTkFrame instance of SignInWindow
            container (customtkinter.CTkFrame): AttendanceLog() instance - the parent container of self
            prefilledUserInfo (List): first and last name of user if this was called from the SignUpWindow instance (default is None)

        Returns: void
        """

        #Intializations and Placement
        super().__init__(container)
        self.container = container
        self.place(relwidth=0.5, relheight=0.5, relx=0.25, rely=0.25)
        activeFrameLst.append(self)
        self.prefilledUserInfo = prefilledUserInfo

        #Skips GUI placement if login info was prefilled from internal call by SignUpWindow instance
        if(prefilledUserInfo != None):
            self.login()

        #Popup GUI
        self.closeBtn=customtkinter.CTkButton(self, image = close_btn_img, fg_color="transparent", text=None, command=lambda:[forgetFrame(self)])
        self.closeBtn.place(relwidth=0.1, relheight=0.1, relx=0.9, rely=0.02)

        self.firstNameEntry = customtkinter.CTkEntry(self, placeholder_text="First Name", font=generateFont('Roboto', 20), justify='center')
        self.firstNameEntry.setvar("name0", "First Name")
        self.firstNameEntry.place(relwidth=0.25, relheight=0.1, relx=0.163, rely=0.1)

        self.lastNameEntry = customtkinter.CTkEntry(self, placeholder_text="Last Name",font=generateFont('Roboto', 20), justify='center')
        self.lastNameEntry.setvar("name1", "Last Name")        
        self.lastNameEntry.place(relwidth=0.25, relheight=0.1, relx=0.576, rely=0.1)

        self.loginBtn = customtkinter.CTkButton(self, border_width=2, text="Log In", font=generateFont('Roboto', 20), text_color=("gray10", "#DCE4EE"), command=lambda:self.login())
        self.loginBtn.place(relwidth=0.2, relheight=0.1, relx=0.4, rely=0.6)

        self.newAccnBtn = customtkinter.CTkButton(self, fg_color="transparent", text="New? Sign Up Here", font=generateFont('Roboto', 20), text_color="cyan", command=lambda:[forgetFrame(self), loadFrame(SignUpWindow, self.container)])
        self.newAccnBtn.place(relwidth=0.2, relheight=0.1, relx=0.4, rely=0.75)

    def login(self):
        """
        The login handler function called by pressing the self.logInBtn of the SignOutWindow instance. 

        Parameters: 
            self (customtkinter.CTkFrame): the CTkFrame instance of SignInWindow

        Returns: void
        """   

        #Matches provided user info to existing database entries, creates User instance if succeeds and calls sign in function, provides error notification if fails.     
        conn = sqlite3.connect('main.sqlite')
        cursor = conn.cursor()
        if(self.prefilledUserInfo != None):
            res = cursor.execute("SELECT * from users WHERE first_name=? AND last_name=?", [self.prefilledUserInfo[0], self.prefilledUserInfo[1]])
        else:
            res = cursor.execute("SELECT * from users WHERE first_name=? AND last_name=?", [self.firstNameEntry.get(), self.lastNameEntry.get()])
        activeUserInfo = res.fetchall()
        conn.close()

        if(len(activeUserInfo) > 0):
            activeUser = User(*activeUserInfo[0])
            activeUsers.append(activeUser)
            loadNotification(self.container, "lime green", "black", "Welcome - " + activeUser.first_name, 2000)
            forgetFrame(self)
            self.container.signIn(activeUser)
        else:   
            forgetFrame(self)
            loadNotification(self.container, "red", "black", "Error - Login Failed", 2000)

class AttendanceLog(customtkinter.CTkFrame):
    """
    This class represents the Attendance Log GUI Screen and associated functionality.
    
    Inherits: customtkinter.CTkFrame

    Attributes:
        container (customtkinter.CTk): Main() instance - the parent container of self
        backBtn (customtkinter.CTkButton): Button that removes all loaded frames, only leaving the Main() instance screen loaded and visible
        genExcelBtn (customtkinter.CTkButton): Button that calls the genExcelFiles() function
        signInBtn (customtkinter.CTkButton): Button that loads an instance of the SignInWindow()
        signOutBtn (customtkinter.CTkButton): Button that loads an instance of the SignOutWindow()
        tableFrame (customtkinter.CTkFrame): Sub-frame to properly place the logTable in
        logTable (CTkTable.CTkTable): Table that shows the attendance logs of the program session
        lastRowIndex (int): Integer that saves the row index of the last row placed so subsequent rows can be placed below existing ones
    """

    def __init__(self, container):
        """
        The constructor for the AttendanceLog class that creates and places GUI elements of the screen.

        Parameters: 
            self (customtkinter.CTkFrame): the CTkFrame instance of AttendanceLog
            container (customtkinter.CTk): Main() instance - the parent container of self

        Returns: void
        """

        #Intializations and Placement
        super().__init__(container)
        self.container = container
        self.place(relwidth=1, relheight=1)
        activeFrameLst.append(self)

        #Screen GUI
        self.backBtn=customtkinter.CTkButton(self, image = left_arrow_img, fg_color="transparent", text=None, command=lambda:[forgetAllFrames()])
        self.backBtn.place(relwidth=0.05, relheight=0.1, relx=0.01, rely=0.02)

        self.genExcelBtn = customtkinter.CTkButton(self, text='Update Excel', font=generateFont('Roboto', 20), command=lambda:[genExcelFiles(self.container)])
        self.genExcelBtn.place(relx=0.85, rely=0.04, relwidth=0.1, relheight=0.05)

        self.signInBtn = customtkinter.CTkButton(self, text='Sign In', font=generateFont('Roboto', 20), command=lambda:[loadFrame(SignInWindow, self)])
        self.signInBtn.place(relx=0.85, rely=0.14, relwidth=0.1, relheight=0.05)

        self.signOutBtn = customtkinter.CTkButton(self, text='Sign Out', font=generateFont('Roboto', 20), command=lambda:[loadFrame(SignOutWindow, self)])
        self.signOutBtn.place(relx=0.85, rely=0.24, relwidth=0.1, relheight=0.05)

        self.tableFrame = customtkinter.CTkFrame(self)
        self.tableFrame.place(rely=0.3, relwidth=1, relheight=0.65)

        self.logTable = CTkTable.CTkTable(master=self.tableFrame, row=1, column=4, header_color="maroon" , values=[["Date", "Time In", "Time Out", "Name"]])
        self.logTable.pack(expand=False, fill="x", padx=20, pady=20, side="top")

        self.lastRowIndex = 1

    def signIn(self, user): 
        """
        The sign in handler function called through an internal call by a SignInWindow Instance that creates a database entry for an attendance login and updates GUI accordingly. 

        Parameters: 
            self (customtkinter.CTkFrame): the CTkFrame instance of AttendanceLog
            user (User): the User instance that is being logged in

        Returns: void
        """   

        #Inserts an attendance log entry into database that includes everything except time_out
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

        #Inserts a row to the GUI table with the day, time_in, N/A for time_out, and a concatenation of the User's first and last name w/ a space between
        self.logTable.add_row(index=self.lastRowIndex, values=[str(currentDay), str(currentDateTime), "N/A", user.first_name + " " + user.last_name])
        user.row = self.lastRowIndex
        self.lastRowIndex += 1

        #Loads success notification
        loadNotification(self.container, "lime green", "black", "Login Info Added For " + user.first_name, 2000, rely=0.1)

    def signOut(self, user):
        """
        The sign out handler function called through an internal call by a SignOutWindow Instance that edits a database entry for an attendance logout and updates GUI accordingly. 

        Parameters: 
            self (customtkinter.CTkFrame): the CTkFrame instance of AttendanceLog
            user (User): the User instance that is being logged out

        Returns: void
        """    

        #Edits any attendance database entry to add a time_out where the user is the passed in user, the day is the current day, and the time_out is NULL (i.e., this entry hasn't been previously signed out)    
        conn = sqlite3.connect('main.sqlite')
        cursor = conn.cursor()
        currentDateTime = datetime.datetime.today().time()
        currentDay = datetime.date.today()
        cursor.execute('''
          UPDATE attendance SET time_out=? WHERE user_id=? AND day=? AND time_out IS NULL            
        ''', [str(currentDateTime), user.id, currentDay])
        conn.commit()
        conn.close()

        #Inserts the time_out to the appropriate row in the GUI table
        self.logTable.insert(user.row, 2, str(currentDateTime))

        #Loads success notification
        loadNotification(self.container, "lime green", "black", "Time Out Added For " + user.first_name, 2000, rely=0.1)

class Main(customtkinter.CTk):
    """  
    This class represents the home GUI Screen (first screen launched upon execution) and associated functionality.
   
    Inherits: customtkinter.CTk

    Attributes:
        titleUI (customtkinter.CTkLabel): Title text of the screen
        attendanceLogBtn (customtkinter.CTkButton): Button that loads an instance of the AttendanceLog frame
        generateExcelBtn (customtkinter.CTkButton): Button that calls the genExcelFiles() function
        copyrightNotice (customtkinter.CTkLabel): Copyright text of the screen
        licenseNotice (customtkinter.CTkLabel): License text (not link) of the screen
        licenseLink (customtkinter.CTkLabel): MIT License link of the screen
    """

    def __init__(self):
        """
        The constructor for the Main class that creates and places GUI elements of the screen.

        Parameters: 
            self (customtkinter.CTk): the CTk instance of Main

        Returns: void
        """

        #Initializations and Placement
        super().__init__()
        self.title("Attendance Tracker v1.0")
        self.geometry("%dx%d+0+0" % (self.winfo_screenwidth(), self.winfo_screenheight()))
        self.initDb()

        #Home UI
        self.titleUI = customtkinter.CTkLabel(self, text="Attendance Tracker v1.0", font=generateFont('Roboto', 64))
        self.titleUI.place(relx=0, rely=0.1, relwidth=1)

        self.attendanceLogBtn = customtkinter.CTkButton(self, text='Launch Attendance Log', font=generateFont('Roboto', 20), command=lambda:[loadFrame(AttendanceLog, self, True)])
        self.attendanceLogBtn.place(relx=0.4, rely=0.3, relwidth=0.2, relheight=0.05)

        self.generateExcelBtn = customtkinter.CTkButton(self, text='Generate Excel Files', font=generateFont('Roboto', 20), command=lambda:[genExcelFiles(self)])
        self.generateExcelBtn.place(relx=0.4, rely=0.4, relwidth=0.2, relheight=0.05)

        self.copyrightNotice = customtkinter.CTkLabel(self, text="Copyright © 2024 Leo Abubucker", font=generateFont('Roboto', 20))
        self.copyrightNotice.place(relx=0.42, rely=0.8)

        self.licenseNotice = customtkinter.CTkLabel(self, text="Software licensed under the", font=generateFont('Roboto', 20))
        self.licenseNotice.place(relx=0.4, rely=0.85)

        self.licenseLink = customtkinter.CTkLabel(self, text="MIT License", text_color="cyan", font=generateFont('Roboto', 20, underline=True), cursor="hand2")     
        self.licenseLink.bind("<Button-1>", lambda e:webbrowser.open_new("https://choosealicense.com/licenses/mit/")) 
        self.licenseLink.place(relx=0.534, rely=0.85)

    def initDb(self):
        """
        The database intializer handler function called at the beginning of execution to create the database and its tables if they don't already exist. 

        Parameters: 
            self (customtkinter.CTk): the CTk instance of Main

        Returns: Void
        """    

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
    """
    This class represents a User that is instantiated from data stored in the users table
    
    Inherits: None

    Attributes:
            id (int): the user's id
            first_name (str): the user's first name
            last_name (str): the user's last name
            grade (int): the user's numerical grade number (1-12)
            phone_number (str): the user's phone number
            team (str): the user's team identification
            row (int): the row that the user's log is placed in the GUI table
    """

    def __init__(self, id:int, first_name:str, last_name:str, grade:int, phone_number:str, team:str):
        """
        The constructor for the User class that initializes parameters and attributes.

        Parameters: 
            self (User): the User instance of User
            id (int): the user's id
            first_name (str): the user's first name
            last_name (str): the user's last name
            grade (int): the user's numerical grade number (1-12)
            phone_number (str): the user's phone number
            team (str): the user's team identification

        Returns: void
        """

        self.id = id
        self.first_name = first_name
        self.last_name = last_name
        self.grade = grade
        self.phone_number = phone_number
        self.team = team
        self.row = -1

def generateFont(fontName:str, fontSize:int, weight:str = 'normal', slant:str = 'roman', underline:bool = False, overstrike:bool = False):
    """
    Generates a customtkinter.CTkFont instance customized based on the given parameters. 

    Parameters:
        fontName (str): name of font style
        fontSize (int): pixel size of the font
        weight (str): "normal" or "bold" font weight
        slant (str): "roman" for unslanted or "italic" for slanted text
        underline (bool): true (underlined) or false (no underline)
        overstrike (bool): true (strikethrough) or false (no strikethrough)

    Returns: customtkinter.CTkFont instance with given parameters
    """

    return customtkinter.CTkFont(family = fontName, size = fontSize, weight=weight, slant=slant, underline=underline, overstrike=overstrike)

def forgetAllFrames():
    """
    Removes all customtkinter.CTkFrame instances in activeFrameLst from the screen and the list
    
    Parameters: None

    Returns: void
    """

    for frame in activeFrameLst:
        frame.place_forget()
        activeFrameLst.remove(frame)

def forgetFrame(frame:customtkinter.CTkFrame):
    """
    Removes a specific customtkinter.CTkFrame instance that is passed in from the screen and activeFrameLst

    Parameters:
        frame (customtkinter.CTkFrame): frame to be removed from screen and activeFrameLst

    Returns: void
    """

    frame.place_forget()
    activeFrameLst.remove(frame)

def loadFrame(frame:customtkinter.CTkFrame, container, forgetOtherFrames:bool=False):
        """
        Loads a given customtkinter.CTkFrame instance with a given parent container, and optionally removes all other frames from the screen

        Parameters:
            frame (customtkinter.CTkFrame): frame to be loaded
            container (customtkinter.CTkFrame or customtkinter.CTk): parent container of frame
            forgetOtherFrames (bool): true (all loaded frames are removed) or false (other loaded frames are not removed) - default false

        Returns: void
        """

        if(forgetOtherFrames):
            forgetAllFrames()
        frame(container)

def genExcelFiles(container):
    """
    Generates Excel files from the sql database tables and loads a success or fail notification accordingly.

    Parameters:
        container (customtkinter.CTkFrame or customtkinter.CTk): parent container where the notification is placed

    Returns: void
    """

    try:
        conn = sqlite3.connect('main.sqlite')

        #Uses pandas to read specific queried info from the users database and write it to "Members.xlsx"
        usersSqlQuery = "SELECT first_name, last_name, grade, phone_number, team from users;"
        usersDf_read = pandas.read_sql(usersSqlQuery, conn)
        usersDf_read.to_excel("Members.xlsx")

        #Uses pandas to read specific queried info from the attendance database and write it to "Attendance.xlsx"
        attendanceSqlQuery = "SELECT day, time_in, time_out, user_first_name, user_last_name from attendance;"
        attendanceDf_read = pandas.read_sql(attendanceSqlQuery, conn)
        attendanceDf_read.to_excel("Attendance.xlsx")

        #Loads success notification 
        loadNotification(container, "lime green", "black", "Success - Attendance.xlsx & Members.xlsx Created", 2000)

    except Exception as e:
        #Continues execution if there are any exceptions and prints a fail notification with the exception details
        loadNotification(container, "red", "black", "Fail - " + str(e), 2000)

    #Closes the database securely
    conn.close()

def loadNotification(container, bgColor:str, textColor:str, text:str, msec:int, rely:float=0.01):
    """
    Creates a customtkinter.CTkLabel with the passed in parameters and displays it for passed in miliseconds

    Parameters:
        container (customtkinter.CTkFrame or customtkinter.CTk): parent container of the created frame
        bgColor (str): background color
        textColor (str): text color
        text (str): text content
        msec (int): miliseconds to display the notification
        rely (float): the relative y position of the frame in its container (default 0.01)

    Returns: void
    """
    
    notification = customtkinter.CTkLabel(master=container, bg_color=bgColor, text_color=textColor, font=generateFont('Roboto', 12), text=text)
    notification.place(relwidth=0.2, relheight=0.05, relx=0.8, rely=rely)
    notification.after(ms=msec, func=lambda:[notification.place_forget()])

#Launches Application
if __name__ == "__main__":
    app = Main()
    app.mainloop()