import customtkinter
import webscraper
import CTkTable
import threading
from PIL import Image
import json
import datetime
import note
activeFrameList = []
customtkinter.set_appearance_mode("System")  # Modes: "System" (standard), "Dark", "Light"
customtkinter.set_default_color_theme("blue")  # Themes: "blue" (standard), "green", "dark-blue"
close_btn_img = customtkinter.CTkImage(dark_image=Image.open("Icons/close_btn_dark.png"), size=(20,20))
left_arrow_img = customtkinter.CTkImage(dark_image=Image.open("Icons/left_arrow.png"), light_image=None, size=(60, 80))
activeFrameDict = {}

    

class DetailedNoteViewFrame(customtkinter.CTkFrame):
    def __init__(self, container, note=None):
        super().__init__(container, fg_color="transparent")
        self.place(relx=0.6, rely=0.15, relwidth=0.35, relheight=0.45)
        self.container = container   
        titleLabel = None
        dateLabel = None
        textBox = customtkinter.CTkTextbox(self, wrap='word')
        if(note is not None):
            titleLabel = customtkinter.CTkLabel(self, text=f"Open Note: {note.noteTitle.replace("\n", " ")}", fg_color="transparent", font=customtkinter.CTkFont("Roboto", 16, 'bold'))
            dateLabel = customtkinter.CTkLabel(self, text=f"{note.noteDate.strip()}     ", fg_color="transparent", font=customtkinter.CTkFont("Roboto", 14, slant='italic'))
            textBox.insert("0.0", note.noteContent)
        else:
            titleLabel = customtkinter.CTkLabel(self, text=f"Open Note: None", fg_color="transparent", font=customtkinter.CTkFont("Roboto", 16, 'bold')) 
            dateLabel = customtkinter.CTkLabel(self, text="", fg_color="transparent", font=customtkinter.CTkFont("Roboto", 14, slant='italic'))
            textBox.insert("0.0", "Click a note from the below view to open it here.\nNo notes below? Use the 'Create New Note' Button to create a new note for this team in this season.")
        titleLabel.place(relx=0, rely=0, relwidth=1)
        dateLabel.place(relx=0, rely=0.05, relwidth=1)
        activeFrameList.append(self)
        activeFrameDict.update({"DetailedNoteViewFrame" : self})
        # textBox.insert("0.0", note.noteTitle + "(" + note.noteDate.strip() + ")\n" + "----------\n")
        textBox.configure(state="disabled")
        textBox.place(relx=0, relwidth=1, rely=0.1, relheight=0.9) 

class NoteViewFrame(customtkinter.CTkFrame):
    def __init__(self, container, season):
        super().__init__(container, fg_color="transparent")
        self.place(relx=0.05, rely=0.68, relwidth=0.9, relheight=0.3)
        self.container = container
        self.season = season
        self.noteFrames = []
        activeFrameList.append(self)
        activeFrameDict.update({"NoteViewFrame" : self})
        self.showNotes()
    def showNotes(self):
        if(len(self.noteFrames) > 0):
            for frame in self.noteFrames:
                frame.place_forget()
                self.noteFrames.remove(frame)
        
        for n in note.Note.openNotes:
            print(f"ATTEMPTED: {n.noteTitle}")
            if(webscraper.chosenTeam.teamNumber == n.noteTeam and self.season == n.noteSeason):
                tempFrame = customtkinter.CTkTextbox(self, wrap='word')
                tempFrame.insert("0.0", n.noteTitle + "(" + n.noteDate.strip() + ")")
                tempFrame.bind("<Button-1>", lambda event, n=n: self.openNoteDetailedView(n))
                self.noteFrames.append(tempFrame)
                print(f"CONFIRMED: {n.noteTitle}")
        relx=0
        rely=0
        for frame in self.noteFrames:
            if(relx < 0.85):
                frame.place(relx=relx, rely=rely, relwidth=0.1, relheight=0.15)
                frame.configure(state="disabled")
            elif(rely < 0.8):
                relx = 0
                rely += 0.2
                frame.place(relx=relx, rely=rely, relwidth=0.1, relheight=0.15)
                frame.configure(state="disabled")
            relx += 0.12
        alertLabel = customtkinter.CTkLabel(self, text=f"Alert: {webscraper.chosenTeam.teamNumber} has no saved notes for the {self.season} season. Click the above 'Create a New Note' button.", text_color="orange", fg_color="transparent", font=customtkinter.CTkFont("Roboto", 16, slant='italic'))
        if(len(note.Note.openNotes) > 0):
            alertLabel.place_forget()
            self.openNoteDetailedView(note.Note.openNotes[0])
        else:
            alertLabel.place(relx=0, rely=0)
            self.openNoteDetailedView()
    def openNoteDetailedView(self, note=None):
        removeFrames(["DetailedNoteViewFrame"])
        DetailedNoteViewFrame(self.container, note)

class NoteCreationFrame(customtkinter.CTkFrame):
    def __init__(self, container, season):
        super().__init__(container)
        self.place(relx=0.5, rely=0.5, anchor = customtkinter.CENTER, relwidth=0.8, relheight=0.8)
        self.container = container
        self.season = season
        activeFrameList.append(self)
        activeFrameDict.update({"NoteCreationFrame" : self})
        self.close_button = customtkinter.CTkButton(self, image = close_btn_img, fg_color="transparent", text=None, command=lambda:[self.place_forget(), activeFrameList.remove(self)])
        self.close_button.place(anchor=customtkinter.NE, relwidth=0.05, relheight=0.07, relx=1, rely=0)
        titleTextBox = customtkinter.CTkTextbox(self, wrap='word')
        titleTextBox.place(relx=0.1, rely=0.1, relwidth=0.8, relheight=0.1)
        contentTextBox = customtkinter.CTkTextbox(self, wrap='word')
        contentTextBox.place(relx=0.1, rely=0.3, relwidth=0.8, relheight=0.5)
        saveButton = customtkinter.CTkButton(self, text="Save", command=lambda:self.saveNote(titleTextBox.get("0.0", customtkinter.END), contentTextBox.get("0.0", customtkinter.END), season))
        saveButton.place(anchor=customtkinter.CENTER, relx=0.5, rely=0.9)
    def saveNote(self, noteTitle, noteContent, season):
        print(f"Note Title: {noteTitle}\nNote Content: {noteContent}")
        day = datetime.date.isoformat(datetime.datetime.today())
        newNote = note.Note(noteTitle, noteContent, day, webscraper.chosenTeam.teamNumber, season)
        newNote.saveToTxtFile()
        note.txtFilesToNotes(season, webscraper.chosenTeam.teamNumber)
        self.place_forget()
        activeFrameList.remove(self)
        removeFrames(["NoteViewFrame", "DetailedNoteViewFrame"])
        NoteViewFrame(self.container, season)

class TeamInfoFrame(customtkinter.CTkFrame):
    V5RCSeasonsList = ["High Stakes (2024-2025)", "Over Under (2023-2024)", "Spin Up (2022-2023)", "Tipping Point (2021-2022)", "Change Up (2020-2021)", "Tower Takeover (2019-2020)", "Turning Point (2018-2019)", "In the Zone (2017-2018)", "Starstruck (2016-2017)",  "Nothing But Net (2015-2016)", "Skyrise (2014-2015)", "Toss Up (2013-2014)", "Sack Attack (2012-2013)", "Gateway (2011-2012)", "Round Up (2010-2011)", "Clean Sweep (2009-2010)", "Elevation (2008-2009)", "Bridge Battle (2007-2008)"]
    VIQRCSeasonsList = ["Rapid Relay (2024-2025)", "Full Volume (2023-2024)", "Slapshot (2022-2023)", "Pitching In (2021-2022)", "Rise Above (2020-2021)", "Squared Away (2019-2020)", "Next Level (2018-2019)", "Ringmaster (2017-2018)", "Crossover (2016-2017)", "Bank Shot (2015-2016)", "Highrise (2014-2015)", "Add It Up (2013-2014)", "Rings-n-Things (2012-2013)"]
    def __init__(self, container, division, team):
        super().__init__(container)
        self.place(relwidth=1, relheight=1)
        self.container = container
        activeFrameList.append(self)   
        activeFrameDict.update({"TeamInfoFrame" : self})
        self.previousPageBtn = customtkinter.CTkButton(self, image=close_btn_img, fg_color=findCorrectColorFromJSON("CTkFrame", "fg_color"), hover=False, text=None, command=lambda:[self.place_forget(), activeFrameList.remove(self)])
        self.previousPageBtn.place(relwidth=0.05, relheight=0.07, relx=0, rely=0)
        self.rankingTable = None    
        self.secondRankingTable = None 
        self.thirdRankingTable = None
        self.noDataFoundLabel = None
        self.noteFrames = []
        teamLabel = customtkinter.CTkLabel(self, text=f"{webscraper.chosenTeam.teamName} ({webscraper.chosenTeam.teamNumber} - {webscraper.chosenTeam.teamDivision})", fg_color="transparent", font=customtkinter.CTkFont("Roboto", 32, 'bold'))
        teamLabel.place(anchor=customtkinter.CENTER, relx=0.5, rely=0.1)      

        infoTableLabel = customtkinter.CTkLabel(self, text="General Information", fg_color="transparent", font=customtkinter.CTkFont("Roboto", 16, 'bold'))
        infoTableLabel.place(relx=0.05, rely=0.15)
        infoTableValues = [["Team Name", "Robot Name", "Currently Active", "Grade Level", "Location", "Event Region"],
                           [webscraper.chosenTeam.teamName, webscraper.chosenTeam.robotName, webscraper.chosenTeam.teamActivity, webscraper.chosenTeam.gradeLevel, webscraper.chosenTeam.teamLocation, webscraper.chosenTeam.teamRegion]]
        
        infoTable = CTkTable.CTkTable(self, row=2, column=6, values = infoTableValues)
        infoTable.place(relx=0.05, rely=0.18)

        skillsTableLabel = customtkinter.CTkLabel(self, text="Current Season Skills Information", fg_color="transparent", font=customtkinter.CTkFont("Roboto", 16, 'bold'))
        skillsTableLabel.place(relx=0.05, rely=0.25)
        skillsTableValues = [["Global Rank" , f"Region ({webscraper.chosenTeam.teamRegion}) Rank", "Best Combined Score", "Autonomous Score of Best Run", "Driver Score of Best Run"],
                             [webscraper.chosenTeam.globalSkillsRank, webscraper.chosenTeam.regionSkillsRank, webscraper.chosenTeam.skillsBestScore, webscraper.chosenTeam.skillsBestProgScore, webscraper.chosenTeam.skillsBestUserScore]]
        skillsTable = CTkTable.CTkTable(self, row=2, column=5, values = skillsTableValues)
        skillsTable.place(relx=0.05, rely=0.28)
        seasonValues = None
        if(division == "V5RC"):
            seasonValues = self.V5RCSeasonsList
        elif(division == "VIQRC"):
            seasonValues = self.VIQRCSeasonsList
        seasonDropdownLabel = customtkinter.CTkLabel(self, text="Season Information", font=customtkinter.CTkFont("Roboto", 16, 'bold'))
        seasonDropdownLabel.place(relx=0.05, rely=0.35)
        seasonDropdown = customtkinter.CTkOptionMenu(self, values=seasonValues, command=self.getSeasonInfo)
        seasonDropdown.place(relx=0.05, rely=0.38)
        noteLabel = customtkinter.CTkLabel(self, text="Season Notes", font=customtkinter.CTkFont("Roboto", 16, 'bold'))
        noteLabel.place(relx=0.05, rely=0.6)
        createNoteButton = customtkinter.CTkButton(self, text="Create New Note", command=lambda:NoteCreationFrame(self, seasonDropdown.get()))
        createNoteButton.place(relx=0.05, rely=0.63)
        self.getSeasonInfo(seasonDropdown.get())

    def getSeasonInfo(self, season):
        removeFrames(["NoteViewFrame", "DetailedNoteViewFrame"])
        note.txtFilesToNotes(season, webscraper.chosenTeam.teamNumber)
        NoteViewFrame(self, season)
        if (self.rankingTable):
            self.rankingTable.destroy()
            self.rankingTable = None

        if (self.secondRankingTable):
            self.secondRankingTable.destroy()
            self.secondRankingTable = None

        if (self.thirdRankingTable):
            self.thirdRankingTable.destroy()
            self.thirdRankingTable = None

        if (self.noDataFoundLabel):
            self.noDataFoundLabel.destroy()
            self.noDataFoundLabel = None
        returnCode = webscraper.getSeasonStats(season)
   
        if(returnCode == 1):
            if(webscraper.chosenTeam.teamDivision == "V5RC"):
                keys = list(webscraper.chosenTeam.V5RCRankingData[season].keys()) 
                values = list(webscraper.chosenTeam.V5RCRankingData[season].values())
                rankingTableValues = [keys[:5], values[:5]]
                secondRankingTableValues = [keys[5:9], values[5:9]]
                thirdRankingTableValues = [keys[9:], values[9:]]
                self.rankingTable = CTkTable.CTkTable(self, row=2, column=5, values=rankingTableValues)
                self.rankingTable.place(relx=0.05, rely=0.41)   
                self.secondRankingTable = CTkTable.CTkTable(self, row=2, column=4, values=secondRankingTableValues)
                self.secondRankingTable.place(relx=0.05, rely=0.47) 
                self.thirdRankingTable = CTkTable.CTkTable(self, row=2, column=4, values=thirdRankingTableValues)
                self.thirdRankingTable.place(relx=0.05, rely=0.53) 
            elif(webscraper.chosenTeam.teamDivision == "VIQRC"):
                keys = list(webscraper.chosenTeam.VIQRCRankingData[season].keys()) 
                values = list(webscraper.chosenTeam.VIQRCRankingData[season].values())
                rankingTableValues = [keys, values]
                self.rankingTable = CTkTable.CTkTable(self, row=2, column=3, values=rankingTableValues)        
                self.rankingTable.place(relx=0.05, rely=0.41)   
        else:
            self.noDataFoundLabel = customtkinter.CTkLabel(self, text=f"Alert: No Data Found for {webscraper.chosenTeam.teamNumber} in the {season} season   ", text_color="orange", fg_color="transparent", font=customtkinter.CTkFont("Roboto", 16, slant='italic'))
            self.noDataFoundLabel.place(relx=0.05, rely=0.41)

        

class TeamInputFrame(customtkinter.CTkFrame):
    def __init__(self, container, division):
        super().__init__(container)
        self.place(relx=0.5, rely=0.5, anchor = customtkinter.CENTER, relwidth=0.6, relheight=0.6)
        self.container = container
        self.division = division
        activeFrameList.append(self)
        activeFrameDict.update({"TeamInputFrame" : self})
        teamLabel = customtkinter.CTkLabel(self, text=f"Input {division} Team", fg_color="transparent", font=customtkinter.CTkFont("Roboto", 32, 'bold'))
        teamLabel.place(anchor=customtkinter.CENTER, relx=0.5, rely=0.1)
        self.teamInput = customtkinter.CTkEntry(self, placeholder_text=f"Enter {division} Team Number Here")
        self.teamInput.place(anchor=customtkinter.CENTER, relx=0.5, rely=0.5, relwidth=0.8, relheight=0.1)
        self.loadingBar = customtkinter.CTkProgressBar(self, mode="indeterminate", indeterminate_speed=0.5)
        self.close_button = customtkinter.CTkButton(self, image = close_btn_img, fg_color="transparent", text=None, command=lambda:[self.place_forget(), activeFrameList.remove(self)])
        self.close_button.place(anchor=customtkinter.NE, relwidth=0.05, relheight=0.07, relx=1, rely=0)
        self.submitButton = customtkinter.CTkButton(self, text="Submit", font=customtkinter.CTkFont("Roboto", 32, 'bold'), command=lambda:self.launchTeamInfoFrame(self.teamInput.get()))
        self.submitButton.place(anchor=customtkinter.CENTER, relx=0.5, rely=0.7, relwidth=0.4, relheight=0.15)
        self.teamInput.bind("<Return>", lambda event:self.launchTeamInfoFrame(self.teamInput.get(), event))
    def launchTeamInfoFrame(self, team, event=None):
        self.loadingBar.place(anchor=customtkinter.CENTER, relx=0.5, rely=0.9, relwidth=0.8, relheight=0.02)
        self.loadingBar.start()  
        thread = threading.Thread(target=self.run_webscraper, args=(team,))
        thread.start()

    def run_webscraper(self, team, runningMultipleTimes=False, lastRun=True):
        team = team.upper().strip()
        errorWaitTime = 0
        errorMsg = None
        if(webscraper.searchTeam(self.division, team) == -1):
            otherDivision = None
            if(self.division == "V5RC"):
                otherDivision = "VIQRC"
            elif(self.division == "VIQRC"):
                otherDivision = "V5RC"
            errorMsg = customtkinter.CTkLabel(self, text=f"Error: {self.division} team {team} not found! Ensure {team} is a {self.division} team and not a {otherDivision} team. Exiting in 5 seconds.", text_color="red", fg_color="transparent", font=customtkinter.CTkFont("Roboto", 16, slant='italic'))
            self.loadingBar.stop()
            self.loadingBar.place_forget()
            errorMsg.place(anchor=customtkinter.CENTER, relx=0.5, rely = 0.90)
            self.container.after(5000, lambda:[self.place_forget(), activeFrameList.remove(self)])
            return
        if(webscraper.getSkillsScores() == -1):
            errorMsg = customtkinter.CTkLabel(self, text=f"Alert: {self.division} team {team} has not competed in skills this season! Skills table and event region will be blank. Continuing in 5 seconds.", text_color="orange", fg_color="transparent", font=customtkinter.CTkFont("Roboto", 16, slant='italic'))
            errorMsg.place(anchor=customtkinter.CENTER, relx=0.5, rely = 0.90)
            errorWaitTime = 5000

        if(lastRun):
            self.loadingBar.stop()
            self.loadingBar.place_forget()
        if(not runningMultipleTimes):
            if(errorMsg == None):   
                self.container.after(0, lambda: TeamInfoFrame(self.container, self.division, team))
            else:
                self.container.after(errorWaitTime, lambda:[errorMsg.place_forget(), TeamInfoFrame(self.container, self.division, team)])
        
class DivisionChooserFrame(customtkinter.CTkFrame):
    def __init__(self, container):
        super().__init__(container)
        self.place(relx=0.5, rely=0.5, anchor = customtkinter.CENTER, relwidth=0.6, relheight=0.6)
        self.container = container
        activeFrameList.append(self)
        activeFrameDict.update({"TeamChooserFrame" : self})
        divisionLabel = customtkinter.CTkLabel(self, text="Choose Division", fg_color="transparent", font=customtkinter.CTkFont("Roboto", 32, 'bold'))
        divisionLabel.place(anchor=customtkinter.CENTER, relx=0.5, rely=0.05)
        vrcButton = customtkinter.CTkButton(self, text="V5RC", font=customtkinter.CTkFont("Roboto", 32, 'bold'), fg_color="red", command=lambda:self.launchTeamInput("V5RC"))
        vrcButton.place(anchor=customtkinter.CENTER, relx=0.5, rely=0.4, relwidth=0.5, relheight=0.2)
        self.iqButton = customtkinter.CTkButton(self, text="VIQRC", font=customtkinter.CTkFont("Roboto", 32, 'bold'), command=lambda:self.launchTeamInput("VIQRC"))
        self.iqButton.place(anchor=customtkinter.CENTER, relx=0.5, rely=0.7, relwidth=0.5, relheight=0.2)
        self.close_button = customtkinter.CTkButton(self, image = close_btn_img, fg_color="transparent", text=None, command=lambda:[self.place_forget(), activeFrameList.remove(self)])
        self.close_button.place(anchor=customtkinter.NE, relwidth=0.05, relheight=0.07, relx=1, rely=0)

    def launchTeamInput(self, division):
        TeamInputFrame(self.container, division)

class ComparisonDivisonChooserFrame(DivisionChooserFrame):
    def __init__(self, container):
        super().__init__(container)
        self.iqButton.configure(state="disabled", text="VIQRC - Feature Coming Soon")

    def launchTeamInput(self, division):
        ComparisonTeamInputFrame(self.container, division)

class ComparisonTeamInputFrame(TeamInputFrame):
    def __init__(self, container, division):
        super().__init__(container, division)
        self.teamInput.configure(placeholder_text=f"Enter 1st {division} Team Number Here")
        self.teamInput.place(anchor=customtkinter.CENTER, relx=0.5, rely=0.3, relwidth=0.8, relheight=0.05)
        teamInput2 = customtkinter.CTkEntry(self, placeholder_text=f"Enter 2nd {division} Team Number Here")
        teamInput2.place(anchor=customtkinter.CENTER, relx=0.5, rely=0.4, relwidth=0.8, relheight=0.05)
        teamInput3 = customtkinter.CTkEntry(self, placeholder_text=f"Enter 3rd {division} Team Number Here")
        teamInput3.place(anchor=customtkinter.CENTER, relx=0.5, rely=0.5, relwidth=0.8, relheight=0.05)
        teamInput4 = customtkinter.CTkEntry(self, placeholder_text=f"Enter 4th {division} Team Number Here")
        teamInput4.place(anchor=customtkinter.CENTER, relx=0.5, rely=0.6, relwidth=0.8, relheight=0.05)
        self.submitButton.configure(command=lambda:self.launchTeamInfoFrame([self.teamInput.get(), teamInput2.get(), teamInput3.get(), teamInput4.get()]))
        self.submitButton.place(anchor=customtkinter.CENTER, relx=0.5, rely=0.75, relwidth=0.4, relheight=0.15)
        self.teamInput.unbind()
        
    def launchTeamInfoFrame(self, teams):
        self.loadingBar.place(anchor=customtkinter.CENTER, relx=0.5, rely=0.9, relwidth=0.8, relheight=0.02)
        self.loadingBar.start()  # Start loading animation

    # Run web scraping in a separate thread
        thread = threading.Thread(target=self.runWebscrapers, args=(teams,))
        thread.start()

    def runWebscrapers(self, teams):
        for team in teams:
            if(team == teams[len(teams) - 1]):
                self.run_webscraper(team, lastRun=True)
            else:
                self.run_webscraper(team)
    
    def run_webscraper(self, team, runningMultipleTimes=True, lastRun=False):
        super().run_webscraper(team, runningMultipleTimes, lastRun)
        if(runningMultipleTimes):
            pass
class App(customtkinter.CTk):
    def __init__(self):
        super().__init__()
        # self.geometry("%dx%d" % (self.winfo_screenwidth(), self.winfo_screenheight()))
        self.geometry(f"{self.winfo_screenwidth()}x{self.winfo_screenheight()}+0+0")
        self.title("Ernie's Scoutanator vDEV")
        # self.state('zoomed')
        titleFont = customtkinter.CTkFont("Roboto", 64, 'bold')
        startButton = customtkinter.CTkButton(self, text="Choose Team", command=self.launchDivisionChooser, font=customtkinter.CTkFont("Roboto", 32, 'bold'))
        startButton.place(anchor=customtkinter.CENTER, relx=0.5, rely=0.5, relwidth=0.4, relheight=0.1)
        teamComparisonButton = customtkinter.CTkButton(self, text="Compare Teams", command=lambda:ComparisonDivisonChooserFrame(self), font=customtkinter.CTkFont("Roboto", 32, 'bold'))
        teamComparisonButton.place(anchor=customtkinter.CENTER, relx=0.5, rely=0.7, relwidth=0.4, relheight=0.1)
        label = customtkinter.CTkLabel(self, text="Ernie's Scoutanator vDEV", fg_color="transparent", font = titleFont)
        label.place(relx = 0.5, rely = 0.05, anchor = customtkinter.N)
    def launchDivisionChooser(self):
        DivisionChooserFrame(self)
def findCorrectColorFromJSON(className:str, attrName:str):
    #Reads and initializes data in blue.json
    jsonFile = f"themes/{customtkinter.ThemeManager._currently_loaded_theme}.json"
    with open(jsonFile, "r") as json_file:
        themeData = json.load(json_file)

    #JSON data is in lst format: [colorForLightMode, colorForDarkMode]
    if(customtkinter.get_appearance_mode() == "Dark"): themeDataIDX = 1
    else: themeDataIDX = 0

    return themeData[className][attrName][themeDataIDX]
       
def removeFrames(frameKeyList):
    for frameKey in frameKeyList:
        if(frameKey is None):
            continue
        frame = activeFrameDict.get(frameKey)
        if(frame is not None):
            activeFrameDict.get(frameKey).place_forget()
app = App()
app.mainloop()