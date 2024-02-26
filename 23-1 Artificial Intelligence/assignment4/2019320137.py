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
               first = 'HybridAgent', second = 'HybridAgent'):
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

class HybridAgent(CaptureAgent):
  """
  A Dummy agent to serve as an example of the necessary agent structure.
  You should look at baselineTeam.py for more details about how to
  create an agent as this is the bare minimum.
  """

  def registerInitialState(self, gameState):
    self.start = gameState.getAgentPosition(self.index)
    CaptureAgent.registerInitialState(self, gameState)

    self.width = gameState.data.layout.width
    self.height = gameState.data.layout.height

    middleX = self.width // 2
    if self.red: middleX -= 1

    # boundary points in the middle of the board -> use when pacman come back to home side
    self.homePositions = [(middleX, i) for i in range(self.height) 
                          if not gameState.hasWall(middleX, i)]

    # choose markman based on its index
    if self.index == self.getTeam(gameState)[0]:
      self.markmanIndex = self.getOpponents(gameState)[0]
    else:
      self.markmanIndex = self.getOpponents(gameState)[-1]
    
    self.PASSIVE_THRESHOLD = 2 # strategy conversion threshold

  def chooseAction(self, gameState):
    actions = gameState.getLegalActions(self.index)
    if self.getScore(gameState) >= self.PASSIVE_THRESHOLD: # if score >= threshold, change strategy to passive 
      strategy = "passive"
    else: # default: balanced
      strategy = "balanced"
    
    # Picks among the actions with the highest Q(s,a).
    values = [self.evaluate(gameState, a, strategy) for a in actions]
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

  def evaluate(self, gameState, action, strategy="balanced"):
    """
    Computes a linear combination of features and feature weights
    """
    features = self.getFeatures(gameState, action, strategy)
    weights = self.getWeights(gameState, action, strategy)

    return features * weights
  
  def getFeatures(self, gameState, action, strategy):
    """
    Returns a counter of features for the state
    """
    if strategy == "passive": # if passive mode, all agents behave as passive agent
      features = self.getPassiveFeatures(gameState, action)
      features['distanceToCenter'] = 0 # disable feature 'distanceToCenter' in passive mode
      return features
    else: # if balanced mode, first agent becomes aggressive and second becomes passive agent
      if self.index == self.getTeam(gameState)[0]:
        return self.getAggressiveFeatures(gameState, action)
      else: 
        features = self.getPassiveFeatures(gameState, action)
        features['distanceToMarkman'] = 0 # disable feature 'distanceToMarkman' in balanced mode 
        return features
  
  def getPassiveFeatures(self, gameState, action):
    features = util.Counter()
    successor = self.getSuccessor(gameState, action)

    myState = successor.getAgentState(self.index)
    myPos = myState.getPosition()

    # Computes whether we're on defense (1) or offense (0)
    features['onDefense'] = 1
    if myState.isPacman: features['onDefense'] = 0

    # Computes distance to invaders we can see
    enemies = [successor.getAgentState(i) for i in self.getOpponents(successor)]
    invaders = [a for a in enemies if a.isPacman and a.getPosition() != None]
    markman = successor.getAgentState(self.markmanIndex)
    features['numInvaders'] = len(invaders)
    
    if len(invaders) > 0: # if there is an invader, chase it.
      distanceToInvaders = [self.getMazeDistance(myPos, a.getPosition()) for a in invaders]
      mindist = min(distanceToInvaders)

      # but if ghost is scared, keep a fixed distance(5) from invader
      if myState.scaredTimer > 0:
        features['distanceToInvaders'] = abs(mindist - 5)
      else:
        features['distanceToInvaders'] = mindist
    else: # if not, wander around center point(in balanced mode) or its markman (in passive mode)
      features['distanceToCenter'] = self.getMazeDistance(myPos, (self.width // 2, self.height // 2))

      features['distanceToMarkman'] = self.getMazeDistance(myPos, markman.getPosition())

    if action == Directions.STOP: features['stop'] = 1
    rev = Directions.REVERSE[gameState.getAgentState(self.index).configuration.direction]
    if action == rev: features['reverse'] = 1

    return features
  
  def getAggressiveFeatures(self, gameState, action):
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
        features['distanceToGhost'] = 1 / minGhostDistance * 10
      else:
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

  
  def getWeights(self, gameState, action, strategy):
      weight = dict()
      if strategy == "passive": # weights for passive mode
        weight = {
          'numInvaders': -1000, 
          'onDefense': 100, 
          'distanceToInvaders': -10,
          'distanceToMarkman': -10,
          'stop': -100, 
          'reverse': -2
          }
      else: # weights for balanced mode
        weight = {
        # for offensive agent
        'distanceToHome': -10,
        'foodCount': -100,
        'distanceToFood': -4,
        'distanceToGhost': -70,
        'score': 10000,
        # for defensive agent
        'numInvaders': -1000, 
        'onDefense': 100, 
        'distanceToInvaders': -10,
        'distanceToCenter': -2,
        'distanceToMarkman': -10,
        
        'stop': -100, 
        'reverse': -2
        }
      
      return weight