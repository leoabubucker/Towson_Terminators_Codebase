from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.support import expected_conditions
from selenium.webdriver.support.ui import WebDriverWait
from team import Team

global chosenTeam
def searchTeam(division, team):
    global chosenTeam
    urlStem = "https://www.robotevents.com/teams/"
    url = urlStem + division + "/" + team
    driver = webdriver.Chrome()
    driver.minimize_window()
    driver.get(url)
    try:
        infoTable = driver.find_element(By.ID, "info")
    except Exception:
        driver.close()
        return -1
    infoTableBody = infoTable.find_element(By.TAG_NAME, "tbody")
    infoTableBodyRows = infoTableBody.find_elements(By.TAG_NAME, "tr")
    teamNumber = team
    teamDivision = division
    teamName = None
    robotName = None
    teamActivity = None
    gradeLevel = None
    teamLocation = None   
    for data in infoTableBodyRows:
        dataHeader = data.find_element(By.TAG_NAME, "th").text
        teamInfo = data.find_element(By.TAG_NAME, "td").text
        if(dataHeader == "Team Name:"):
            teamName = teamInfo
        elif(dataHeader == "Robot Name:"):
            robotName = teamInfo
        elif(dataHeader == "Active:"):
            teamActivity = teamInfo    
        elif(dataHeader == "Grade Level:"):
            gradeLevel = teamInfo  
        elif(dataHeader == "Location:"):
            teamLocation = teamInfo    
    chosenTeam = Team(teamNumber, teamDivision, teamName, robotName, teamActivity, gradeLevel, teamLocation)
    driver.close()
    return 1
def getSkillsScores():
    global chosenTeam
    gradeLevel = "High+School"
    if(chosenTeam.teamDivision == "V5RC"):
        url = f"https://www.robotevents.com/robot-competitions/vex-robotics-competition/standings/skills?search={chosenTeam.teamNumber}&grade_level={gradeLevel}"
    elif(chosenTeam.teamDivision == "VIQRC"):
        gradeLevel = "Middle+School"
        url = f"https://www.robotevents.com/robot-competitions/vex-iq-competition/standings/skills?search={chosenTeam.teamNumber}&grade_level={gradeLevel}"

    driver = webdriver.Chrome()
    driver.minimize_window()
    driver.get(url)
    infoTable = None
    try:
        infoTable = WebDriverWait(driver, 10).until(expected_conditions.presence_of_element_located((By.TAG_NAME, "table")))
    except Exception:
        if(chosenTeam.teamDivision == "V5RC"):
            gradeLevel = "Middle+School"
            url = f"https://www.robotevents.com/robot-competitions/vex-robotics-competition/standings/skills?search={chosenTeam.teamNumber}&grade_level={gradeLevel}"
        elif(chosenTeam.teamDivision == "VIQRC"):
            gradeLevel = "Elementary"
            url = f"https://www.robotevents.com/robot-competitions/vex-iq-competition/standings/skills?search={chosenTeam.teamNumber}&grade_level={gradeLevel}"     

        driver.get(url)
        try:
            infoTable = WebDriverWait(driver, 10).until(expected_conditions.presence_of_element_located((By.TAG_NAME, "table")))  
        except Exception:
            driver.close()
            return -1

    infoTableRows = infoTable.find_element(By.TAG_NAME, "tbody").find_elements(By.TAG_NAME, "tr")
    chosenRow = None
    for row in infoTableRows:
        teamNumberRow = row.find_element(By.CLASS_NAME, "column-team-number")
        teamText = teamNumberRow.find_element(By.TAG_NAME, "a").text
        if(teamText == chosenTeam.teamNumber):
            chosenRow = row

    if(chosenRow != None):
        chosenTeam.addGlobalSkillsRank(int(chosenRow.find_element(By.CLASS_NAME, "column-rank").text))
        chosenTeam.addSkillsBestScore(int(chosenRow.find_element(By.CLASS_NAME, "column-score").text))
        chosenTeam.addSkillsBestProgScore(int(chosenRow.find_element(By.CLASS_NAME, "column-programming").text))
        chosenTeam.addSkillsBestUserScore(int(chosenRow.find_element(By.CLASS_NAME, "column-driver").text))  
        chosenTeam.addTeamRegion(chosenRow.find_element(By.CLASS_NAME, "column-event-region").text)
        urlFriendlyEventRegion = (chosenRow.find_element(By.CLASS_NAME, "column-event-region").text).replace(" ", "+")
        if(chosenTeam.teamDivision == "V5RC"):
            url = f"https://www.robotevents.com/robot-competitions/vex-robotics-competition/standings/skills?search={chosenTeam.teamNumber}&grade_level={gradeLevel}&event_region={urlFriendlyEventRegion}"
        elif(chosenTeam.teamDivision == "VIQRC"):
            url = f"https://www.robotevents.com/robot-competitions/vex-iq-competition/standings/skills?search={chosenTeam.teamNumber}&grade_level={gradeLevel}&event_region={urlFriendlyEventRegion}"
        
        driver.get(url)
        infoTable = WebDriverWait(driver, 5).until(expected_conditions.presence_of_element_located((By.TAG_NAME, "table")))
        infoTableRows = infoTable.find_element(By.TAG_NAME, "tbody").find_elements(By.TAG_NAME, "tr")
        chosenRow = None
        for row in infoTableRows:
            teamNumberRow = row.find_element(By.CLASS_NAME, "column-team-number")
            teamText = teamNumberRow.find_element(By.TAG_NAME, "a").text
            if(teamText == chosenTeam.teamNumber):
                chosenRow = row
        if(chosenRow != None):
            chosenTeam.addRegionSkillsRank(int(chosenRow.find_element(By.CLASS_NAME, "column-rank").text))     
        driver.close()

        if(chosenTeam.gradeLevel == None):
            chosenTeam.gradeLevel = gradeLevel.replace("+", " ")
        return 1

def getSeasonStats(season):
    global chosenTeam
    url = f"https://www.robotevents.com/teams/{chosenTeam.teamDivision}/{chosenTeam.teamNumber}"
    driver = webdriver.Chrome()
    driver.minimize_window()
    driver.get(url)
    navCol = WebDriverWait(driver, 10).until(expected_conditions.presence_of_element_located((By.CLASS_NAME, "col-sm-3")))  
    navBar = navCol.find_element(By.TAG_NAME, "nav")
    navItems = navBar.find_elements(By.TAG_NAME, "a")
    infoTab = None
    rankingsTab = None
    matchResultsTab = None
    awardsTab = None
    for item in navItems:
        if(item.text == "Info"):
            infoTab = item
        elif(item.text == "Rankings"):
            rankingsTab = item
        elif(item.text == "Match Results"):
            matchResultsTab = item
        elif(item.text == "Awards"):
            awardsTab = item
    
    # Rankings Tab
    rankingsTab.click()
    waitingValue = WebDriverWait(driver, 10).until(expected_conditions.presence_of_element_located((By.CLASS_NAME, "card")))
    seasonCards = driver.find_elements(By.CLASS_NAME, "card")
    seasonCardHeaders = driver.find_elements(By.CLASS_NAME, "card-header")
    cleanedSeason = season.split(" (")[0]
    chosenSeasonCard = None
    for i, header in enumerate(seasonCardHeaders):
        if(header.text.find(cleanedSeason) != -1):
            chosenSeasonCard = seasonCards[i]
            break
    if(chosenSeasonCard == None):
        driver.close()
        return -1
    
    seasonTableBody = chosenSeasonCard.find_element(By.TAG_NAME, "tbody")
    seasonCompetitions = seasonTableBody.find_elements(By.TAG_NAME, "tr")
    chosenTeam.updateRankingValue(season, "Competitions Attended", len(seasonCompetitions))
    if(chosenTeam.teamDivision == "V5RC"):
        numWins = 0
        numLosses = 0
        numTies = 0
        totalWinPoints = 0
        totalAutonomousPoints = 0
        totalSchedulePoints = 0
        matchesPlayed = 0
        highestLevelReached = "County"
        for competition in seasonCompetitions:
            competitionData = competition.find_elements(By.TAG_NAME, "td")
            if(competition == seasonCompetitions[0]):
                tournamentName = competitionData[0].find_element(By.TAG_NAME, "a").text
                if(tournamentName.find("World") != -1):
                    highestLevelReached = "World"
                elif(tournamentName.find("State") != -1):
                    highestLevelReached = "State"

            numWins += int(competitionData[2].text)
            numLosses += int(competitionData[3].text)
            numTies += int(competitionData[4].text)
            points = competitionData[5].text.split(" / ")
            totalWinPoints += int(points[0])
            totalAutonomousPoints += int(points[1])
            totalSchedulePoints += int(points[2])
        matchesPlayed = numWins + numLosses + numTies
        chosenTeam.updateRankingValue(season, "Matches Played", matchesPlayed)
        chosenTeam.updateRankingValue(season, "Wins", numWins)
        chosenTeam.updateRankingValue(season, "Losses", numLosses)
        chosenTeam.updateRankingValue(season, "Ties", numTies)
        chosenTeam.updateRankingValue(season, "Total Win Points", totalWinPoints)
        chosenTeam.updateRankingValue(season, "Total Autonomous Points", totalAutonomousPoints)
        chosenTeam.updateRankingValue(season, "Total Schedule Points", totalSchedulePoints)
        chosenTeam.updateRankingValue(season, "Highest Level Reached", highestLevelReached)
        chosenTeam.updateRankingValue(season, "Win Rate", round((float(numWins) / float(matchesPlayed)) * 100, 3))
        chosenTeam.updateRankingValue(season, "Average Win Points", round((float(totalWinPoints) / float(matchesPlayed)), 3))
        chosenTeam.updateRankingValue(season, "Average Autonomous Points", round((float(totalAutonomousPoints) / float(matchesPlayed)), 3))
        chosenTeam.updateRankingValue(season, "Average Schedule Points", round((float(totalSchedulePoints) / float(matchesPlayed)), 3))
        driver.close()
        return 1
    elif(chosenTeam.teamDivision == "VIQRC"):
        totalAvgPoints = 0
        avgRanking = 0
        chosenTeam.updateRankingValue(season, "Average Ranking", "Data Coming Soon")
        chosenTeam.updateRankingValue(season, "Average Points", "Data Coming Soon")
        driver.close()
        return 1
        




