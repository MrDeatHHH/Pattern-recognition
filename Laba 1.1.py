import asyncio
import websockets

# Turn to "True" if you want to see messages and turn to "False" if you don't
showMessage  = True

# Settings
sclWidth = 5
sclHeight = 5
noise = 0.4
nSteps = 100
shuffleOn = False

async def hello():
    
    uri = "wss://sprs.herokuapp.com/first/mefisto"
    async with websockets.connect(uri) as websocket:
        
        greetings = "Let's start"
        await websocket.send(greetings)
        if showMessage:
            print(greetings)
            print("-----------------------------------------")

        consts = await websocket.recv()
        tokens = consts.split()
        if showMessage:
            print(tokens)
            print("-----------------------------------------")

        width = int(tokens[0])
        height = int(tokens[1])
        N = int(tokens[2])

        # Now we have real width and height of pictures
        width *= sclWidth
        height *= sclHeight

        # Sending settings
        settings = str(sclWidth)+" "+str(sclHeight)+" "+str(noise)+" "+str(nSteps)+" "+str("on" if shuffleOn else "off")
        if showMessage:
            print(settings)
            print("-----------------------------------------")
        await websocket.send(settings)

        # Recieving matrixes of numbers
        numbers = await websocket.recv()
        if showMessage:
            print(numbers)
            print("-----------------------------------------")
        matrixesOfNumbers = numbers.split()

        # We got all matrixes of numbers and now we are ready to start completing the task
        # THEY ARE NOT SCALED AS WRITTEN IN DOCUMENTATION ON GITHUB!!!
        # P.S. Now everything is fine, but I will leave this comment here >:)

        for i in range(0, nSteps):
            
            await websocket.send("Ready")
            
            matrix = await websocket.recv()
            splittedMatrix = matrix.split()
            if showMessage:
                print(matrix)
                print("-----------------------------------------")

            # Initializing array which will contain amount of "1" in noise matrix
            noiseCounter = []
            for j in range(0, N):
                
                noiseCounter.append(0)

            # Amount of elements in matrix
            lengthOfMatrix = width * height
            
            for j in range(0, N):
            
                # Starting position of j'th number in "matrixesOfNumbers" array
                startingPosition = (j + 1) + j * lengthOfMatrix

                # Counting amount of "1" in noise matrix
                for k in range(0, lengthOfMatrix):
                
                    noiseCounter[j] += 1 if ( bool(int(splittedMatrix[k + 1])) ^ bool(int(matrixesOfNumbers[startingPosition + k])) ) else 0
            
            # Finding the digit with the biggest probability
            indexOfMaxProb = 0
            # Number with biggest probability has the lowest amount of "1" in noise matrix if noise probabilty is less then 0.5
            # This comes from the fact that prob = p^(#"1") * (1-p)^(#'0')
            # Thus if p < 1 - p ( or p < 0.5 ) then the lower #"1" is the lower probability is
            if ( noise > 0.5 ):
                
                for j in range(0, N):

                    noiseCounter[j] *= -1

            if showMessage:
                print(noiseCounter)
                print("-----------------------------------------")

            for j in range(1, N):
                
                if ( noiseCounter[j] < noiseCounter[indexOfMaxProb] ):

                    indexOfMaxProb = j

            answer = splittedMatrix[0]+" "+matrixesOfNumbers[indexOfMaxProb * ( 1 + lengthOfMatrix )]
            if showMessage:
                print(answer)
            await websocket.send(answer)

            realAnswer = await websocket.recv()
            if showMessage:
                print(realAnswer)
                print("-----------------------------------------")

        await websocket.send("Bye")

        results = await websocket.recv()
        print(results)


asyncio.get_event_loop().run_until_complete(hello())
