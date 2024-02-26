# multiAgents.py
# --------------
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


from util import manhattanDistance
from game import Directions
import random, util

from game import Agent

class ReflexAgent(Agent):
    """
    A reflex agent chooses an action at each choice point by examining
    its alternatives via a state evaluation function.

    The code below is provided as a guide.  You are welcome to change
    it in any way you see fit, so long as you don't touch our method
    headers.
    """


    def getAction(self, gameState):
        """
        You do not need to change this method, but you're welcome to.

        getAction chooses among the best options according to the evaluation function.

        Just like in the previous project, getAction takes a GameState and returns
        some Directions.X for some X in the set {NORTH, SOUTH, WEST, EAST, STOP}
        """
        # Collect legal moves and successor states
        legalMoves = gameState.getLegalActions()

        # Choose one of the best actions
        scores = [self.evaluationFunction(gameState, action) for action in legalMoves]
        bestScore = max(scores)
        bestIndices = [index for index in range(len(scores)) if scores[index] == bestScore]
        chosenIndex = random.choice(bestIndices) # Pick randomly among the best

        "Add more of your code here if you want to"

        return legalMoves[chosenIndex]

    def evaluationFunction(self, currentGameState, action):
        """
        Design a better evaluation function here.

        The evaluation function takes in the current and proposed successor
        GameStates (pacman.py) and returns a number, where higher numbers are better.

        The code below extracts some useful information from the state, like the
        remaining food (newFood) and Pacman position after moving (newPos).
        newScaredTimes holds the number of moves that each ghost will remain
        scared because of Pacman having eaten a power pellet.

        Print out these variables to see what you're getting, then combine them
        to create a masterful evaluation function.
        """
        # Useful information you can extract from a GameState (pacman.py)
        successorGameState = currentGameState.generatePacmanSuccessor(action)
        newPos = successorGameState.getPacmanPosition()
        newFood = successorGameState.getFood()
        newGhostStates = successorGameState.getGhostStates()
        newScaredTimes = [ghostState.scaredTimer for ghostState in newGhostStates]

        score = successorGameState.getScore() - currentGameState.getScore() # get difference between next state and current state  
        
        # calculate score using food information
        foodPosList = newFood.asList()
        foodScore = 0
        if len(foodPosList) != 0:
            foodDistance = min(manhattanDistance(foodPos, newPos) for foodPos in foodPosList) # manhattan distance to nearest food
            foodScore = 10 / foodDistance # take reciprocal of minimum distance so that smaller distance gets higher score
        
        # calculate score using ghost information
        ghostPosList = [ghostState.getPosition() for ghostState in newGhostStates]
        ghostDistance = min(manhattanDistance(ghostPos, newPos) for ghostPos in ghostPosList) # manhattan distacne to nearest ghost
        ghostScore = -10 / ghostDistance if ghostDistance > 0 else -1000 # smaller distance gets lower score
        if newScaredTimes[0] > 0: # if ghosts are scared, change the sign
            ghostScore *= -1

        score += foodScore
        score += ghostScore
        return score

def scoreEvaluationFunction(currentGameState):
    """
    This default evaluation function just returns the score of the state.
    The score is the same one displayed in the Pacman GUI.

    This evaluation function is meant for use with adversarial search agents
    (not reflex agents).
    """
    return currentGameState.getScore()

class MultiAgentSearchAgent(Agent):
    """
    This class provides some common elements to all of your
    multi-agent searchers.  Any methods defined here will be available
    to the MinimaxPacmanAgent, AlphaBetaPacmanAgent & ExpectimaxPacmanAgent.

    You *do not* need to make any changes here, but you can if you want to
    add functionality to all your adversarial search agents.  Please do not
    remove anything, however.

    Note: this is an abstract class: one that should not be instantiated.  It's
    only partially specified, and designed to be extended.  Agent (game.py)
    is another abstract class.
    """

    def __init__(self, evalFn = 'scoreEvaluationFunction', depth = '2'):
        self.index = 0 # Pacman is always agent index 0
        self.evaluationFunction = util.lookup(evalFn, globals())
        self.depth = int(depth)

class MinimaxAgent(MultiAgentSearchAgent):
    """
    Your minimax agent (question 2)
    """

    def getAction(self, gameState):
        """
        Returns the minimax action from the current gameState using self.depth
        and self.evaluationFunction.

        Here are some method calls that might be useful when implementing minimax.

        gameState.getLegalActions(agentIndex):
        Returns a list of legal actions for an agent
        agentIndex=0 means Pacman, ghosts are >= 1

        gameState.generateSuccessor(agentIndex, action):
        Returns the successor game state after an agent takes an action

        gameState.getNumAgents():
        Returns the total number of agents in the game

        gameState.isWin():
        Returns whether or not the game state is a winning state

        gameState.isLose():
        Returns whether or not the game state is a losing state
        """
        bestAction = self.minimax(agentIndex=0, gameState=gameState, depth=0)[1] # get best action by minimax
        return bestAction
    
    def minimax(self, agentIndex, gameState, depth):
        if gameState.isWin() or gameState.isLose() or depth == self.depth:  
            return self.evaluationFunction(gameState), "" # if game ends or depth limit reached, return evaluation value 
        
        bestAction = Directions.STOP
        if agentIndex == 0: # pacman
            actions = gameState.getLegalActions(agentIndex)
            nextAgentIndex, nextDepth = agentIndex + 1, depth

            bestValue = -10 ** 6
            for action in actions: # for each legal actions under current state,
                successorGameState = gameState.generateSuccessor(agentIndex, action) #

                value, _ = self.minimax(nextAgentIndex, successorGameState, nextDepth) # get minimax value for next state

                if value > bestValue: # for pacman, get maximum value and corresponding action
                    bestValue = value
                    bestAction = action 
        else: # ghost
            actions = gameState.getLegalActions(agentIndex)
            if agentIndex < gameState.getNumAgents() - 1:
                nextAgentIndex, nextDepth = agentIndex + 1, depth
            else:
                nextAgentIndex, nextDepth = 0, depth + 1 # increment depth only after all agents move
            
            bestValue = 10 ** 6
            for action in actions:
                successorGameState = gameState.generateSuccessor(agentIndex, action)
                
                value, _ = self.minimax(nextAgentIndex, successorGameState, nextDepth)
                if value < bestValue: # for ghosts, get minimum value and action
                    bestValue = value
                    bestAction = action

        return bestValue, bestAction # return best value and action

class AlphaBetaAgent(MultiAgentSearchAgent):
    """
    Your minimax agent with alpha-beta pruning (question 3)
    """

    def getAction(self, gameState):
        """
        Returns the minimax action using self.depth and self.evaluationFunction
        """
        bestAction = self.alphebeta(agentIndex=0, gameState=gameState, depth=0)[1] # get best action by minimax with alph-beta pruning
        return bestAction

    def alphebeta(self, agentIndex, gameState, depth, alpha=-10**6, beta=10**6):
        if gameState.isWin() or gameState.isLose() or depth == self.depth:  
            return self.evaluationFunction(gameState), "" # if game ends or depth limit reached, return evaluation value 
        
        bestAction = Directions.STOP
        if agentIndex == 0: # pacman
            actions = gameState.getLegalActions(agentIndex)
            nextAgentIndex, nextDepth = agentIndex + 1, depth

            bestValue = -10 ** 6
            for action in actions: # for each legal actions under current state,
                successorGameState = gameState.generateSuccessor(agentIndex, action) #

                value, _ = self.alphebeta(nextAgentIndex, successorGameState, nextDepth, alpha, beta) # get minimax value for next state
                if value > bestValue: # for pacman, get maximum value and corresponding action
                    bestValue = value
                    bestAction = action 
                    
                    if bestValue > beta: # check if current maximum value exceeds beta
                        return bestValue, bestAction # if so then return immediately
                    alpha = max(alpha, bestValue) # update alpha value

        else: # ghost
            actions = gameState.getLegalActions(agentIndex)
            if agentIndex < gameState.getNumAgents() - 1:
                nextAgentIndex, nextDepth = agentIndex + 1, depth
            else:
                nextAgentIndex, nextDepth = 0, depth + 1 # increment depth only after all agents move
            
            bestValue = 10 ** 6
            for action in actions:
                successorGameState = gameState.generateSuccessor(agentIndex, action)
                
                value, _ = self.alphebeta(nextAgentIndex, successorGameState, nextDepth, alpha, beta)
                if value < bestValue: # for ghosts, get minimum value and action
                    bestValue = value
                    bestAction = action
                
                    if bestValue < alpha: # check if current minimum value is less than alpha
                        return bestValue, bestAction # if so then return immediately
                    beta = min(beta, bestValue) # update beta value

        return bestValue, bestAction # return best value and action


class ExpectimaxAgent(MultiAgentSearchAgent):
    """
      Your expectimax agent (question 4)
    """

    def getAction(self, gameState):
        """
        Returns the expectimax action using self.depth and self.evaluationFunction

        All ghosts should be modeled as choosing uniformly at random from their
        legal moves.
        """
        "*** YOUR CODE HERE ***"
        util.raiseNotDefined()

def betterEvaluationFunction(currentGameState):
    """
    Your extreme ghost-hunting, pellet-nabbing, food-gobbling, unstoppable
    evaluation function (question 5).

    DESCRIPTION: <write something here so we know what you did>
    """
    "*** YOUR CODE HERE ***"
    util.raiseNotDefined()

# Abbreviation
better = betterEvaluationFunction
