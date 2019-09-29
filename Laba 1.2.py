import asyncio
import websockets

# Turn to "True" if you want to see messages and turn to "False" if you don't
showMessage  = True

# Settings
width = 15
# Loss can be a number or "L1"
loss = "L1"
totalSteps = 1
repeats = 3

async def hello():
    
    uri = "wss://sprs.herokuapp.com/second/mefisto"
    async with websockets.connect(uri) as websocket:
        
        greetings = "Let's start with "+str(width)+" "+str(loss)+" "+str(totalSteps)+" "+str(repeats)
        await websocket.send(greetings)
        if showMessage:
            print(greetings)
            print("-----------------------------------------")

        # This variable will tell us which algorithm should be used
        algorithm = 0
        if (loss == "L1"):
            algorithm = 1

        kids = await websocket.recv()

        for i in range(0, totalSteps):
            
            await websocket.send("Ready")
            
            heatmap = await websocket.recv()
            splittedHeatmap = heatmap.split()
            if showMessage:
                print(heatmap)
                print("-----------------------------------------")

            # Algorithm with d
            if ( algorithm == 0 ):
                
                # We will add "0" in the end if heatmap just for some technical use
                splittedHeatmap.append("0")

                # Array of our sums ( they could be treated as probabilities if we normalized numbers in heatmap, but there is no actual need in doing this )
                sumMap = []

                for j in range(0, width):
                
                    sumMap.append(0)
            
                # First let's calculate first "loss + 1" values, because after them we will use "smart" algorithm of calculating
                for j in range(0, loss + 1):
                
                    sumMap[0] += int(splittedHeatmap[j + 2])
            
                for j in range(1, loss + 1):

                    sumMap[j] = sumMap[j - 1] + int(splittedHeatmap[min(j + loss, width) + 2])

                # Now we are taking prev value, decrease it by the [rightest] value it included and increase by the [leftest + 1]

                for j in range(loss + 1, width):

                    sumMap[j] = sumMap[j - 1] - int(splittedHeatmap[j - loss - 1 + 2]) + int(splittedHeatmap[min(j + loss, width) + 2])

                if showMessage:
                    print(sumMap)
                    print("-----------------------------------------")

                # Sorting and saving indexes
                sortedSumMap = sorted( ( (value, index) for index, value in enumerate(sumMap) ), reverse = True )

                # Making an answer
                answer = splittedHeatmap[1]+"\n"+str(sortedSumMap[0][1])
                for j in range(1, repeats):

                    answer += " "+str(sortedSumMap[0][1])
                
                if showMessage:
                    print(answer)
                    print("-----------------------------------------")

                await websocket.send(answer)

                solution = await websocket.recv()
                if showMessage:
                    print(solution)
                    print("-----------------------------------------")

            # Algorithm with L1
            else:

                sumMap = 0

                for j in range(0, width):

                    sumMap += int(splittedHeatmap[j + 2])

                if showMessage:
                    print(sumMap)
                    print("-----------------------------------------")

                index = 0
                newSum = 0.0

                while ( newSum < ( float(sumMap) / 2.0 ) ):
                    
                    newSum += float(splittedHeatmap[index + 2])
                    index += 1

                index -= 1

                # Making an answer
                answer = splittedHeatmap[1]+"\n"+str(index)
                for j in range(1, repeats):

                    answer += " "+str(index)

                if showMessage:
                    print(answer)
                    print("-----------------------------------------")

                await websocket.send(answer)

                solution = await websocket.recv()
                if showMessage:
                    print(solution)
                    print("-----------------------------------------")
                

        await websocket.send("Bye")

        results = await websocket.recv()
        print(results)


asyncio.get_event_loop().run_until_complete(hello())
