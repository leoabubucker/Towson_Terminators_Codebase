import random
import os
import datetime
class Note:
    openNotes = []
    def __init__(self, noteTitle, noteContent, noteDate, noteTeam, noteSeason):
        self.noteTitle = noteTitle
        self.noteContent = noteContent
        self.noteDate = noteDate
        self.noteTeam = noteTeam
        self.noteSeason = noteSeason
        self.noteRandomNum = random.randint(0, 100)

    def saveToTxtFile(self):
        fileName = "Notes/" + self.noteSeason + "_" + self.noteTeam + "_" + self.noteDate + "_" + str(self.noteRandomNum) + ".txt"
        with(open(fileName, 'w+') as file):
            file.write(self.noteDate + "\n")
            file.write("___TITLE___\n")
            file.write(self.noteTitle)
            file.write("___CONTENT___\n")
            file.write(self.noteContent)
            file.write("___END___")

def txtFilesToNotes(season, team):
    unorderedNotes = []
    Note.openNotes = []
    txtFilesToOpen = [file for file in os.listdir("Notes/") if file.endswith(".txt") and season in file and team in file]
    for file in txtFilesToOpen:
        with(open("Notes/" + file, 'r') as f):
            content = f.readlines()
            print(content)
            noteDate = content[0].removesuffix("\n")
            print(noteDate)
            contentFlag = None
            endFlag = None
            for i, line in enumerate(content):
                if(line == "___CONTENT___\n"):
                    contentFlag = i
                elif(line == "___END___"):
                    endFlag = i
            
            noteTitleList = [line for line in content[2:contentFlag]]
            noteTitle = ""
            for line in noteTitleList:
                noteTitle += line
            noteContentList = [line for line in content[contentFlag+1 : endFlag]]
            noteContent = ""
            for line in noteContentList:
                noteContent += line
            unorderedNotes.append(Note(noteTitle, noteContent, noteDate, team, season))

    unorderedDates = [note.noteDate for note in unorderedNotes]
    orderedDates = sorted(unorderedDates, key=lambda x: datetime.datetime.fromisoformat(x), reverse=True)
    orderedNotes = []
    for date in orderedDates:
        for note in unorderedNotes:
            if(note.noteDate == date and not note in orderedNotes):
                orderedNotes.append(note)
    Note.openNotes = orderedNotes
