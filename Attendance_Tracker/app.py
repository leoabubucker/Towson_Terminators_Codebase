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
right_arrow_img = customtkinter.CTkImage(dark_image=Image.open("Icons/right_arrow.png"), light_image=None, size=(60, 80))

#Global Definitions
activeFrameLst = []
activeUsers = []
activeUserFrames = []

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

        #Creates a new row in the users table with inputted data, launches the sign in window with predefined data if succeeds
        self.form_options = [self.firstNameEntry.get(), self.lastNameEntry.get(), int(self.gradeEntry.get()), self.phoneNumberEntry.get(), self.teamEntry.get()]

        cursor.execute("INSERT INTO users (first_name, last_name, grade, phone_number, team) VALUES (?, ?, ?, ?, ?)", self.form_options)
        conn.commit()
        conn.close()
        forgetFrame(self)
        loadNotification(self.container, "lime green", "black", "Welcome New User - " + self.firstNameEntry.get(), 2000)
        SignInWindow(self.container, self.form_options)
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
        self.newAccnBtn.place(relwidth=0.25, relheight=0.1, relx=0.375, rely=0.75)

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

class UserProfile(customtkinter.CTkFrame):
    """
    This class represents the User Profile frame and associated functionality. 
    
    Inherits: customtkinter.CTkFrame

    Attributes:
        container (customtkinter.CTkFrame):  UserDashboard() instance - the parent container of self
        userID (int): ID of the given user
        user (User): User() instance of the given user
        attendanceInfo (list): list that starts with the header row for attendance, to be populated with data correlating to the headers
        absenceInfo (list): list that starts with the header row for absences, to be populated with data correlating to the headers
        backBtn (customtkinter.CTkButton): button that returns user to the home page of the application

    """

    def __init__(self, container, userID):
        """
        The constructor for the UserProfile class that creates and places GUI elements.

        Parameters: 
            self (customtkinter.CTkFrame): the CTkFrame instance of UserProfile
            container (customtkinter.CTkFrame): UserDashboard() instance - the parent container of self
            userID (int): ID of the given user

        Returns: void
        """

        super().__init__(container)
        self.container = container
        self.userID = userID
        self.place(relwidth=1, relheight=1)
        activeFrameLst.append(self)
        self.user = None
        self.attendanceInfo = [("Day", "Time In", "Time Out")]
        self.absenceInfo = [("Day", "Is Excused")]
        self.backBtn=customtkinter.CTkButton(self, image = left_arrow_img, fg_color="transparent", text=None, command=lambda:[forgetAllFrames()])
        self.backBtn.place(relwidth=0.05, relheight=0.1, relx=0.01, rely=0.02)

        #Calls fetchUserDetails() to populate lists with respective data
        self.fetchUserDetails()

    def fetchUserDetails(self):
        """
        Retrieves necessary data from tables and populates it into predefined lists

        Parameters: 
            self (customtkinter.CTkFrame): the CTkFrame instance of UserProfile

        Returns: void
        """

        conn = sqlite3.connect("main.sqlite")
        cursor = conn.cursor()

        userInfoRes = cursor.execute("SELECT * FROM users WHERE id=?", [self.userID])
        self.user = User(*userInfoRes.fetchall()[0])

        attendanceInfoRes = cursor.execute("SELECT day, time_in, time_out FROM attendance WHERE user_id=? ORDER BY id DESC", [self.userID])
        self.attendanceInfo.extend(attendanceInfoRes.fetchall())

        absenceInfoRes = cursor.execute("SELECT day, isExcused FROM absences WHERE user_id=? ORDER BY id DESC", [self.userID])
        self.absenceInfo.extend(absenceInfoRes.fetchall())
        self.absenceInfo = self.absenceInfo

        #Converts binary boolean values to text
        for iter, val in enumerate(self.absenceInfo):
            self.absenceInfo[iter] = list(val)
            if(val[1] == 0):
                self.absenceInfo[iter][1] = "No"
            elif(val[1] == 1):
                self.absenceInfo[iter][1] = "Yes"

        conn.close()

        #Calls loadProfile() to display stored data
        self.loadProfile()

    def loadProfile(self):
        """
        Creates and displays tables and other GUI elements to display user info

        Parameters: 
            self (customtkinter.CTkFrame): the CTkFrame instance of UserProfile

        Returns: void
        """

        nameTxt = customtkinter.CTkLabel(self, text=self.user.first_name + " " + self.user.last_name, font=generateFont('Roboto', 20, weight='bold'))
        nameTxt.place(relwidth=1, relheight=0.05, rely=0.02)
        nameTxt.lower(belowThis=self.backBtn)

        userInfoHeaderLabel = customtkinter.CTkLabel(self, text="User Info", font=generateFont('Roboto', 15, slant="italic"))
        userInfoHeaderLabel.place(rely=0.12, relwidth=0.3, relheight=0.03)
        userInfoTableFrame = customtkinter.CTkFrame(self)
        userInfoTableFrame.place(rely=0.15, relwidth=0.3, relheight=1)
        userInfoTable = CTkTable.CTkTable(userInfoTableFrame, row=6, column=2, orientation="vertical", header_color="maroon", values=[["ID", "First Name", "Last Name", "Grade", "Phone Number", "Team"], [self.user.id, self.user.first_name, self.user.last_name, self.user.grade, self.user.phone_number, self.user.team]])
        userInfoTable.pack(expand=False, fill="x", padx=20, pady=20, side="top")

        attendanceHeaderLabel = customtkinter.CTkLabel(self, text="Attendance Log", font=generateFont('Roboto', 15, slant="italic"))
        attendanceHeaderLabel.place(rely=0.12, relwidth=0.3, relheight=0.03, relx=0.3)
        attendanceTableFrame = customtkinter.CTkFrame(self)
        attendanceTableFrame.place(relx = 0.3, rely=0.15, relwidth=0.3, relheight=1)
        attendanceTable = CTkTable.CTkTable(attendanceTableFrame, row=len(self.attendanceInfo), column=3, values=self.attendanceInfo, header_color="maroon")
        attendanceTable.pack(expand=False, fill="x", padx=20, pady=20, side="top")

        absenceHeaderLabel = customtkinter.CTkLabel(self, text="Absences Log", font=generateFont('Roboto', 15, slant="italic"))
        absenceHeaderLabel.place(rely=0.12, relwidth=0.3, relheight=0.03, relx=0.6)
        absenceTableFrame = customtkinter.CTkFrame(self)
        absenceTableFrame.place(relx = 0.6, rely=0.15, relwidth=0.3, relheight=1)
        absenceTable = CTkTable.CTkTable(absenceTableFrame, row=len(self.absenceInfo), column=2, values=self.absenceInfo, header_color="maroon")
        absenceTable.pack(expand=False, fill="x", padx=20, pady=20, side="top")

class UserDashboard(customtkinter.CTkFrame):
    """
    This class represents the User Dashboard frame and associated functionality. 
    
    Inherits: customtkinter.CTkFrame

    Attributes:
        container (customtkinter.CTkFrame):  UserDashboard() instance - the parent container of self
        userNames (list): List that holds the full names of all users, ordered by their ID
        ids (list): List that holds the IDs of all users, ordered by their ID

    """

    def __init__(self, container):
        """
        The constructor for the UserDashboard class that creates and places GUI elements.

        Parameters: 
            self (customtkinter.CTkFrame): the CTkFrame instance of UserCard
            container (customtkinter.CTkFrame): Main() instance - the parent container of self

        Returns: void
        """

        super().__init__(container)
        self.container = container
        self.place(relwidth=1, relheight=1)
        activeFrameLst.append(self)

        backBtn=customtkinter.CTkButton(self, image = left_arrow_img, fg_color="transparent", text=None, command=lambda:[forgetAllFrames()])
        backBtn.place(relwidth=0.05, relheight=0.1, relx=0.01, rely=0.02)

        #Retrieves and stores names and IDs
        conn = sqlite3.connect("main.sqlite")
        cursor = conn.cursor()
        userRes = cursor.execute("SELECT first_name, last_name from users")
        self.userNames = [name[0] + " " + name[1] for name in userRes.fetchall()]
        IDRes = cursor.execute("SELECT id from users ORDER BY id ASC")
        self.ids = [id[0] for id in IDRes.fetchall()]
        conn.close()

        #Loads a UserCard() instance for each user
        self.loadUserCards()

    def loadUserCards(self, startPoint = 0):
        """
        Recursive function that loads UserCard() instances for each User and provides buttons to navigate different pages of UserCard() instances

        Parameters: 
            self (customtkinter.CTkFrame): the CTkFrame instance of UserCard
            startPoint (int): index in self.userNames of where to start generating cards from (default = 0)

        Returns: void
        """

        relx = 0.1
        rely = 0.15

        for i, name in enumerate(self.userNames):
            prevBtn=customtkinter.CTkButton(self, image= left_arrow_img, fg_color="transparent", text=None, command=lambda:[card.forgetItems(), self.loadUserCards(i-12)])

            if(startPoint > 0):
                prevBtn.place(relwidth = 0.05, relheight=0.1, relx=0.01, rely=0.45)
            else:
                prevBtn.place_forget()
            if(i >= startPoint):
                card = UserCard(self, relx, rely, name, self.ids[i])
                card.placeItems()
                relx += 0.2
                if(relx > 0.7):
                    relx = 0.1
                    rely += 0.3 
                if(rely > 0.75):
                    #Creates a new page
                    nextBtn=customtkinter.CTkButton(self, image= right_arrow_img, fg_color="transparent", text=None, command=lambda:[card.forgetItems(), self.loadUserCards(i)])
                    nextBtn.place(relwidth = 0.05, relheight=0.1, relx=0.94, rely=0.45)

class Main(customtkinter.CTk):
    """  
    This class represents the home GUI Screen (first screen launched upon execution) and associated functionality.
   
    Inherits: customtkinter.CTk

    Attributes:
        titleUI (customtkinter.CTkLabel): Title text of the screen
        attendanceLogBtn (customtkinter.CTkButton): Button that loads an instance of the AttendanceLog frame
        generateExcelBtn (customtkinter.CTkButton): Button that calls the genExcelFiles() function
        userDashboardBtn (customtkinter.CTkButton): Button that loads an instance of the UserDashboard frame
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
        self.title("Attendance Tracker v1.1")
        self.geometry("%dx%d+0+0" % (self.winfo_screenwidth(), self.winfo_screenheight()))
        self.protocol("WM_DELETE_WINDOW", lambda:[quitApp(self)])
        self.initDb()

        #Home UI
        self.titleUI = customtkinter.CTkLabel(self, text="Attendance Tracker v1.0 - DEV", font=generateFont('Roboto', 64))
        self.titleUI.place(relx=0, rely=0.1, relwidth=1)

        self.attendanceLogBtn = customtkinter.CTkButton(self, text='Launch Attendance Log', font=generateFont('Roboto', 20), command=lambda:[loadFrame(AttendanceLog, self, True)])
        self.attendanceLogBtn.place(relx=0.4, rely=0.3, relwidth=0.2, relheight=0.05)

        self.generateExcelBtn = customtkinter.CTkButton(self, text='Generate Excel Files', font=generateFont('Roboto', 20), command=lambda:[genExcelFiles(self)])
        self.generateExcelBtn.place(relx=0.4, rely=0.4, relwidth=0.2, relheight=0.05)

        self.userDashboardBtn = customtkinter.CTkButton(self, text='View User Dashboard', font=generateFont('Roboto', 20), command=lambda:[forgetAllFrames(), UserDashboard(container=self)])
        self.userDashboardBtn.place(relx=0.4, rely=0.5, relwidth=0.2, relheight=0.05)

        self.copyrightNotice = customtkinter.CTkLabel(self, text="Copyright © 2024 Leo Abubucker", font=generateFont('Roboto', 20))
        self.copyrightNotice.place(relx=0.42, rely=0.8)

        self.licenseNotice = customtkinter.CTkLabel(self, text="Software licensed under the", font=generateFont('Roboto', 20))
        self.licenseNotice.place(relx=0.4, rely=0.85)

        self.licenseLink = customtkinter.CTkLabel(self, text="MIT License", text_color="cyan", font=generateFont('Roboto', 20, underline=True), cursor="hand2")     
        self.licenseLink.bind("<Button-1>", lambda e:webbrowser.open_new("https://choosealicense.com/licenses/mit/")) 
        self.licenseLink.place(relx=0.547, rely=0.85)

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
                last_name TEXT NOT NULL,
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

        #Create the absences table if not exists
        cursor.execute('''
            CREATE TABLE IF NOT EXISTS absences (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                day TIMESTAMP NOT NULL,
                user_id INTEGER NOT NULL,
                user_first_name TEXT NOT NULL,
                user_last_name TEXT NOT NULL,
                isExcused BOOLEAN NOT NULL,
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

class UserCard(customtkinter.CTkFrame):
    """
    This class represents a User Card shown in the UserDashboard frame and associated functionality. 
    
    Inherits: customtkinter.CTkFrame

    Attributes:
        container (customtkinter.CTkFrame):  UserDashboard() instance - the parent container of self
        relx (double): relx position of the frame 
        rely (double): rely position of the frame
        id (int): user's ID
        nameTxt (customtkinter.CTkLabel): label item to display name       
    """

    def __init__(self, container, relx, rely, name, id):
        """
        The constructor for the UserCard class that creates and places GUI elements of the UserCard in the UserDashboard.

        Parameters: 
            self (customtkinter.CTkFrame): the CTkFrame instance of UserCard
            container (customtkinter.CTkFrame): UserDashboard() instance - the parent container of self
            relx (double): relx position of the frame
            rely (double): rely position of the frame
            name (str): user's name to display
            id (int): user's ID

        Returns: void
        """

        super().__init__(container)
        self.container = container
        self.relx = relx
        self.rely = rely
        self.id = id
        self.nameTxt = customtkinter.CTkLabel(self, text=name, font=generateFont('Roboto', 15))
        self.bind("<Button-1>", lambda e:self.onClickEvent(e))
        self.nameTxt.bind("<Button-1>", lambda e:self.onClickEvent(e))

    def placeItems(self):
        """
        Places this and its text on the UserDashboard.

        Parameters: 
            self (customtkinter.CTkFrame): the CTkFrame instance of UserCard

        Returns: void
        """

        activeUserFrames.append(self)
        self.place(relwidth=0.15, relheight=0.15, relx=self.relx, rely=self.rely)
        self.nameTxt.place(relwidth=1, relheight=0.1, rely=0.45)

    def forgetItems(self):
        """
        Removes all frames from the Application.

        Parameters: 
            self (customtkinter.CTkFrame): the CTkFrame instance of UserCard

        Returns: void
        """

        for frame in activeUserFrames:
            frame.place_forget()

    def onClickEvent(self, event):
        """
        Launches a User Profile instance of the given user, of which the User Card which was clickec belongs.

        Parameters: 
            self (customtkinter.CTkFrame): the CTkFrame instance of UserCard
            event (tkinter event): Not used, but necessary for calling this function on mouse click

        Returns: void
        """

        UserProfile(self.container, self.id)

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

        #Uses pandas to read specific queried info from the absences database and write it to "Absences.xlsx"
        absencesSqlQuery = "SELECT day, user_first_name, user_last_name from absences;"
        absencesDf_read = pandas.read_sql(absencesSqlQuery, conn)
        absencesDf_read.to_excel("Absences.xlsx")

        #Loads success notification 
        loadNotification(container, "lime green", "black", "Success - Members.xlsx, Attendance.xlsx, & Absences.xlsx Created", 2000)

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

def quitApp(container):
    """
    Runs when the close button is pressed, providing the user with a popup confirming their action.

    Parameters:
        container (customtkinter.CTkFrame or customtkinter.CTk): parent container of the created frame

    Returns: void
    """

    popup = customtkinter.CTkFrame(master=container)
    activeFrameLst.append(popup)
    popup.place(relwidth=0.5, relheight=0.4, relx=0.25, rely=0.25)

    closeBtn=customtkinter.CTkButton(master=popup, image = close_btn_img, fg_color="transparent", text=None, command=lambda:[forgetFrame(popup)])
    closeBtn.place(relwidth=0.1, relheight=0.1, relx=0.9, rely=0.02)

    confirmationTxt = customtkinter.CTkLabel(master=popup, text="Are you sure you want to quit the app?\nIf there is at least one logged user, all unlogged users will be marked absent.", font=generateFont('Roboto', 20))
    confirmationTxt.place(rely=0.2, relwidth=1)

    #Runs the logAbsences function
    yesBtn = customtkinter.CTkButton(master=popup, text="Yes", command=lambda:logAbsences(container))
    yesBtn.place(relx=0.25, rely=0.5)

    #Skips the logAbsences function, immediately exits the app
    noBtn = customtkinter.CTkButton(master=popup, text="No", command=lambda:forgetFrame(popup))
    noBtn.place(relx=0.5, rely=0.5)

def logAbsences(container):
    """
    Inserts current day and user info of all users not logged in into the Absences table if at least one user is logged in. Then, quits the app.

    Parameters:
        container (customtkinter.CTkFrame or customtkinter.CTk): parent container of the created frame

    Returns: void
    """

    #Retrieves and stores pertinent info
    conn = sqlite3.connect('main.sqlite')
    cursor = conn.cursor()
    userIDRes = cursor.execute("SELECT id FROM users")
    userIDs = [id[0] for id in userIDRes.fetchall()]
    currentDay = datetime.date.today()
    attendedUserIDRes = cursor.execute("SELECT user_id FROM attendance WHERE day=? AND time_out IS NOT NULL", [currentDay])
    attendedUserIDs = [id[0] for id in attendedUserIDRes.fetchall()] #Logged in Users' IDs

    #Inserts pertinent info into Absences table for users not logged in if at least one user is logged in
    if(len(attendedUserIDs) > 0):
        for id in userIDs:
            if(not id in attendedUserIDs):
                absentUserInfoRes = cursor.execute("SELECT first_name, last_name FROM users WHERE id=?", [id])
                absentUserInfo = absentUserInfoRes.fetchone()
                cursor.execute("INSERT INTO absences (day, user_id, user_first_name, user_last_name, isExcused) VALUES (?, ?, ?, ?, ?)", [currentDay, id, absentUserInfo[0], absentUserInfo[1], False])
                conn.commit()

    #Closes Database
    conn.close()

    #Closes Application
    container.destroy()

#Launches Application
if __name__ == "__main__":
    app = Main()
    app.mainloop()