class Team:
    teams = []
    def __init__(self, teamNumber = None, teamDivision = None, teamName = None, robotName = None, teamActivity = None, gradeLevel = None, teamLocation = None):
        self.teamNumber = teamNumber
        self.teamDivision = teamDivision
        self.teamName = teamName
        self.robotName = robotName
        self.teamActivity = teamActivity
        self.gradeLevel = gradeLevel
        self.teamLocation = teamLocation     
        self.globalSkillsRank = None
        self.regionSkillsRank = None
        self.skillsBestScore = None
        self.skillsBestProgScore = None
        self.skillsBestUserScore = None
        self.teamRegion = None
        self.V5RCRankingData = {
        "High Stakes (2024-2025)" : {
            "Competitions Attended" : None,
            "Matches Played" : None,
            "Wins" : None,
            "Losses" : None,
            "Ties" : None,
            "Total Win Points" : None,
            "Total Autonomous Points" : None,
            "Total Schedule Points" : None,
            "Highest Level Reached" : None,
            "Win Rate" : None,
            "Average Win Points" : None,
            "Average Autonomous Points" : None,
            "Average Schedule Points" : None,
        },
        "Over Under (2023-2024)" : {
            "Competitions Attended" : None,
            "Matches Played" : None,
            "Wins" : None,
            "Losses" : None,
            "Ties" : None,
            "Total Win Points" : None,
            "Total Autonomous Points" : None,
            "Total Schedule Points" : None,
            "Highest Level Reached" : None,
            "Win Rate" : None,
            "Average Win Points" : None,
            "Average Autonomous Points" : None,
            "Average Schedule Points" : None,
        },
        "Spin Up (2022-2023)" : {
            "Competitions Attended" : None,
            "Matches Played" : None,
            "Wins" : None,
            "Losses" : None,
            "Ties" : None,
            "Total Win Points" : None,
            "Total Autonomous Points" : None,
            "Total Schedule Points" : None,
            "Highest Level Reached" : None,
            "Win Rate" : None,
            "Average Win Points" : None,
            "Average Autonomous Points" : None,
            "Average Schedule Points" : None,
        },
        "Tipping Point (2021-2022)" : {
            "Competitions Attended" : None,
            "Matches Played" : None,
            "Wins" : None,
            "Losses" : None,
            "Ties" : None,
            "Total Win Points" : None,
            "Total Autonomous Points" : None,
            "Total Schedule Points" : None,
            "Highest Level Reached" : None,
            "Win Rate" : None,
            "Average Win Points" : None,
            "Average Autonomous Points" : None,
            "Average Schedule Points" : None,
        },
        "Change Up (2020-2021)" : {
            "Competitions Attended" : None,
            "Matches Played" : None,
            "Wins" : None,
            "Losses" : None,
            "Ties" : None,
            "Total Win Points" : None,
            "Total Autonomous Points" : None,
            "Total Schedule Points" : None,
            "Highest Level Reached" : None,
            "Win Rate" : None,
            "Average Win Points" : None,
            "Average Autonomous Points" : None,
            "Average Schedule Points" : None,
        },
        "Tower Takeover (2019-2020)" : {
            "Competitions Attended" : None,
            "Matches Played" : None,
            "Wins" : None,
            "Losses" : None,
            "Ties" : None,
            "Total Win Points" : None,
            "Total Autonomous Points" : None,
            "Total Schedule Points" : None,
            "Highest Level Reached" : None,
            "Win Rate" : None,
            "Average Win Points" : None,
            "Average Autonomous Points" : None,
            "Average Schedule Points" : None,
        },
        "Turning Point (2018-2019)" : {
            "Competitions Attended" : None,
            "Matches Played" : None,
            "Wins" : None,
            "Losses" : None,
            "Ties" : None,
            "Total Win Points" : None,
            "Total Autonomous Points" : None,
            "Total Schedule Points" : None,
            "Highest Level Reached" : None,
            "Win Rate" : None,
            "Average Win Points" : None,
            "Average Autonomous Points" : None,
            "Average Schedule Points" : None,
        },
        "In the Zone (2017-2018)" : {
            "Competitions Attended" : None,
            "Matches Played" : None,
            "Wins" : None,
            "Losses" : None,
            "Ties" : None,
            "Total Win Points" : None,
            "Total Autonomous Points" : None,
            "Total Schedule Points" : None,
            "Highest Level Reached" : None,
            "Win Rate" : None,
            "Average Win Points" : None,
            "Average Autonomous Points" : None,
            "Average Schedule Points" : None,
        },
        "Starstruck (2016-2017)" : {
            "Competitions Attended" : None,
            "Matches Played" : None,
            "Wins" : None,
            "Losses" : None,
            "Ties" : None,
            "Total Win Points" : None,
            "Total Autonomous Points" : None,
            "Total Schedule Points" : None,
            "Highest Level Reached" : None,
            "Win Rate" : None,
            "Average Win Points" : None,
            "Average Autonomous Points" : None,
            "Average Schedule Points" : None,
        },
        "Nothing But Net (2015-2016)" : {
            "Competitions Attended" : None,
            "Matches Played" : None,
            "Wins" : None,
            "Losses" : None,
            "Ties" : None,
            "Total Win Points" : None,
            "Total Autonomous Points" : None,
            "Total Schedule Points" : None,
            "Highest Level Reached" : None,
            "Win Rate" : None,
            "Average Win Points" : None,
            "Average Autonomous Points" : None,
            "Average Schedule Points" : None,
        },
        "Skyrise (2014-2015)" : {
            "Competitions Attended" : None,
            "Matches Played" : None,
            "Wins" : None,
            "Losses" : None,
            "Ties" : None,
            "Total Win Points" : None,
            "Total Autonomous Points" : None,
            "Total Schedule Points" : None,
            "Highest Level Reached" : None,
            "Win Rate" : None,
            "Average Win Points" : None,
            "Average Autonomous Points" : None,
            "Average Schedule Points" : None,
        },
        "Toss Up (2013-2014)" : {
            "Competitions Attended" : None,
            "Matches Played" : None,
            "Wins" : None,
            "Losses" : None,
            "Ties" : None,
            "Total Win Points" : None,
            "Total Autonomous Points" : None,
            "Total Schedule Points" : None,
            "Highest Level Reached" : None,
            "Win Rate" : None,
            "Average Win Points" : None,
            "Average Autonomous Points" : None,
            "Average Schedule Points" : None,
        },
        "Sack Attack (2012-2013)" : {
            "Competitions Attended" : None,
            "Matches Played" : None,
            "Wins" : None,
            "Losses" : None,
            "Ties" : None,
            "Total Win Points" : None,
            "Total Autonomous Points" : None,
            "Total Schedule Points" : None,
            "Highest Level Reached" : None,
            "Win Rate" : None,
            "Average Win Points" : None,
            "Average Autonomous Points" : None,
            "Average Schedule Points" : None,
        },
        "Gateway (2011-2012)" : {
            "Competitions Attended" : None,
            "Matches Played" : None,
            "Wins" : None,
            "Losses" : None,
            "Ties" : None,
            "Total Win Points" : None,
            "Total Autonomous Points" : None,
            "Total Schedule Points" : None,
            "Highest Level Reached" : None,
            "Win Rate" : None,
            "Average Win Points" : None,
            "Average Autonomous Points" : None,
            "Average Schedule Points" : None,
        },
        "Round Up (2010-2011)" : {
            "Competitions Attended" : None,
            "Matches Played" : None,
            "Wins" : None,
            "Losses" : None,
            "Ties" : None,
            "Total Win Points" : None,
            "Total Autonomous Points" : None,
            "Total Schedule Points" : None,
            "Highest Level Reached" : None,
            "Win Rate" : None,
            "Average Win Points" : None,
            "Average Autonomous Points" : None,
            "Average Schedule Points" : None,
        },
        "Clean Sweep (2009-2010)" : {
            "Competitions Attended" : None,
            "Matches Played" : None,
            "Wins" : None,
            "Losses" : None,
            "Ties" : None,
            "Total Win Points" : None,
            "Total Autonomous Points" : None,
            "Total Schedule Points" : None,
            "Highest Level Reached" : None,
            "Win Rate" : None,
            "Average Win Points" : None,
            "Average Autonomous Points" : None,
            "Average Schedule Points" : None,
        },
        "Elevation (2008-2009)" : {
            "Competitions Attended" : None,
            "Matches Played" : None,
            "Wins" : None,
            "Losses" : None,
            "Ties" : None,
            "Total Win Points" : None,
            "Total Autonomous Points" : None,
            "Total Schedule Points" : None,
            "Highest Level Reached" : None,
            "Win Rate" : None,
            "Average Win Points" : None,
            "Average Autonomous Points" : None,
            "Average Schedule Points" : None,
        },
        "Bridge Battle (2007-2008)" : {
            "Competitions Attended" : None,
            "Matches Played" : None,
            "Wins" : None,
            "Losses" : None,
            "Ties" : None,
            "Total Win Points" : None,
            "Total Autonomous Points" : None,
            "Total Schedule Points" : None,
            "Highest Level Reached" : None,
            "Win Rate" : None,
            "Average Win Points" : None,
            "Average Autonomous Points" : None,
            "Average Schedule Points" : None,
        },
        }
        self.VIQRCRankingData = {
        "Rapid Relay (2024-2025)" : {
            "Competitions Attended" : None,
            "Average Ranking" : None,
            "Average Points" : None,
        },
        "Full Volume (2023-2024)" : {
            "Competitions Attended" : None,
            "Competitions Attended" : None,
            "Average Ranking" : None,
            "Average Points" : None,
        },
        "Slapshot (2022-2023)" : {
            "Competitions Attended" : None,
            "Average Ranking" : None,
            "Average Points" : None,
        },
        "Pitching In (2021-2022)" : {
            "Competitions Attended" : None,
            "Average Ranking" : None,
            "Average Points" : None,
        },
        "Rise Above (2020-2021)" : {
            "Competitions Attended" : None,
            "Average Ranking" : None,
            "Average Points" : None,
        },
        "Squared Away (2019-2020)" : {
            "Competitions Attended" : None,
            "Average Ranking" : None,
            "Average Points" : None,
        },
        "Next Level (2018-2019)" : {
            "Competitions Attended" : None,
            "Average Ranking" : None,
            "Average Points" : None,
        },
        "Ringmaster (2017-2018)" : {
            "Competitions Attended" : None,
            "Average Ranking" : None,
            "Average Points" : None,
        },
        "Crossover (2016-2017)" : {
            "Competitions Attended" : None,
            "Average Ranking" : None,
            "Average Points" : None,
        },
        "Bank Shot (2015-2016)" : {
            "Competitions Attended" : None,
            "Average Ranking" : None,
            "Average Points" : None,
        },
        "Highrise (2014-2015)" : {
            "Competitions Attended" : None,
            "Average Ranking" : None,
            "Average Points" : None,
        },
        "Add It Up (2013-2014)" : {
            "Competitions Attended" : None,
            "Average Ranking" : None,
            "Average Points" : None,
        },
        "Rings-n-Things (2012-2013)" : {
            "Competitions Attended" : None,
            "Average Ranking" : None,
            "Average Points" : None,
        },
    }
    
    def addGlobalSkillsRank(self, score):
        self.globalSkillsRank = score

    def addRegionSkillsRank(self, score):
        self.regionSkillsRank = score

    def addSkillsBestScore(self, score):
        self.skillsBestScore = score

    def addSkillsBestProgScore(self, score):
        self.skillsBestProgScore = score

    def addSkillsBestUserScore(self, score):
        self.skillsBestUserScore = score

    def addTeamRegion(self, region):
        self.teamRegion = region

    def updateRankingValue(self, season, key, value):
        for seasonKey in list(self.V5RCRankingData.keys()):
            if(season == seasonKey):
                self.V5RCRankingData[season][key] = value
                return
        for seasonKey in list(self.VIQRCRankingData.keys()):
            if(season == seasonKey):
                self.VIQRCRankingData[season][key] = value
                return        


    def __str__(self):
        return f"{self.teamName} ({self.teamNumber} - {self.teamDivision}):\nRobot Name: {self.robotName}\nTeam Name: {self.teamName}\nCurrently Active: {self.teamActivity}\nGrade Level: {self.gradeLevel}\nLocation: {self.teamLocation}\nEvent Region: {self.teamRegion}\nGlobal Skills: {self.globalSkillsRank}\nRegion Skills: {self.regionSkillsRank}\nBest Combined Skills Score: {self.skillsBestScore}\nProg Skills Score of Best Run: {self.skillsBestProgScore}\nDriver Skills Score of Best Run: {self.skillsBestUserScore}"