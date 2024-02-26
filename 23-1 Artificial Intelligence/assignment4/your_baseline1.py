# myTeam.py
# ---------
# Licensing Information:  You are free to use or extend these projects for
# educational purposes provided that (1) you do not distribute or publish
# solutions, (2) you retain this notice, and (3) you provide clear
# attribution to UC Berkeley, including a link to http://ai.berkeley.edu.
# 
# Attribution Information: The Pacman AI projects were developed at UC Berkeley.
# The core projects and autograders were primarily created by John DeNero
# (denero@cs.berkeley.edu) and Dan Klein (klein@cs.berkeley.edu).
# Student side autograding was added by Brad Miller, Nick Hay, and
# Pieter Abbeel (pabbeel@cs.berkeley.edu).


from captureAgents import CaptureAgent
import random, time, util
from game import Directions
import game
from util import nearestPoint

#################
# Team creation #
#################

def createTeam(firstIndex, secondIndex, isRed,
               first = 'AggressiveAgent', second = 'AggressiveAgent'):
  """
  This function should return a list of two agents that will form the
  team, initialized using firstIndex and secondIndex as their agent
  index numbers.  isRed is True if the red team is being created, and
  will be False if the blue team is being created.

  As a potentially helpful development aid, this function can take
  additional string-valued keyword arguments ("first" and "second" are
  such arguments in the case of this function), which will come from
  the --redOpts and --blueOpts command-line arguments to capture.py.
  For the nightly contest, however, your team will be created without
  any extra arguments, so you should make sure that the default
  behavior is what you want for the nightly contest.
  """

  # The following line is an example only; feel free to change it.
  return [eval(first)(firstIndex), eval(second)(secondIndex)]

##########
# Agents #
##########

class AggressiveAgent(CaptureAgent):
  """
  A Dummy agent to serve as an example of the necessary agent structure.
  You should look at baselineTeam.py for more details about how to
  create an agent as this is the bare minimum.
  """

  def registerInitialState(self, gameState):
    self.start = gameState.getAgentPosition(self.index)
    CaptureAgent.registerInitialState(self, gameState)
    
    middleX = gameState.data.layout.width // 2
    if self.red: middleX -= 1

    # boundary points in the middle of the board -> use when pacman come back to home side
    self.homePositions = [(middleX, y) for y in range(gameState.data.layout.height) 
                          if not gameState.hasWall(middleX, y)]


  def chooseAction(self, gameState):
    # Picks among the actions with the highest Q(s,a).
    actions = gameState.getLegalActions(self.index)
    values = [self.evaluate(gameState, a) for a in actions]

    maxValue = max(values)
    bestActions = [a for a, v in zip(actions, values) if v == maxValue]
    
    return random.choice(bestActions)
  
  def getSuccessor(self, gameState, action):
    """
    Finds the next successor which is a grid position (location tuple).
    """
    successor = gameState.generateSuccessor(self.index, action)
    pos = successor.getAgentState(self.index).getPosition()
    if pos != nearestPoint(pos):
      # Only half a grid position was covered
      return successor.generateSuccessor(self.index, action)
    else:
      return successor

  def evaluate(self, gameState, action):
    """
    Computes a linear combination of features and feature weights
    """
    features = self.getFeatures(gameState, action)
    weights = self.getWeights(gameState, action)

    return features * weights
  
  def getFeatures(self, gameState, action):
    """
    Returns a counter of features for the state
    """
    features = util.Counter()

    successor = self.getSuccessor(gameState, action)
    myState = successor.getAgentState(self.index)
    myPos = myState.getPosition()

    enemies = [successor.getAgentState(i) for i in self.getOpponents(successor)]
    ghosts = [a for a in enemies if not a.isPacman and a.scaredTimer == 0 and a.getPosition() != None]
    
    # if there is a ghost, use reciprocal of minimum distance to ghost as a feature in order for pacman to avoid ghost
    minGhostDistance = 9999
    if len(ghosts) > 0: 
      minGhostDistance = min([self.getMazeDistance(myPos, ghost.getPosition()) for ghost in ghosts])

    # if pacman carries food, return to home side
    if myState.numCarrying > 0:
      distanceToHome = min(self.getMazeDistance(myPos, homePos) for homePos in self.homePositions)
      features['distanceToHome'] = distanceToHome

      # when returning, have more weights on avoiding ghosts than when searching
      features['distanceToGhost'] = 1/minGhostDistance * 10
    else: # if pacman search for food, keep distant from its teammate to prevent searching for same food
      if myState.isPacman:
        teammate = [successor.getAgentState(i) for i in self.getTeam(successor) if i != self.index][0]
        teammatePos = teammate.getPosition()
        features['distanceToTeammate'] = self.getMazeDistance(myPos, teammatePos)

      # use number of food and minimum distance to food as a feature
      foodList = self.getFood(successor).asList()
      features['foodCount'] = len(foodList)

      if len(foodList) > 0:
        minFoodDistance = min([self.getMazeDistance(myPos, food) for food in foodList])
        features['distanceToFood'] = minFoodDistance
      
      features['distanceToGhost'] = 1/minGhostDistance
    
    features['score'] = self.getScore(successor)
    if action == Directions.STOP: # keep pacman moving
      features['stop'] = 1
    
    return features

  def getWeights(self, gameState, action):
      return {
        'distanceToHome': -10,
        'foodCount': -100,
        'distanceToFood': -4, # short distance -> high q-value
        'distanceToGhost': -70, # long distance -> high q-value
        'distanceToTeammate': 2, # long distance -> higher q-value
        'stop': -100,
        'score': 10000
        }