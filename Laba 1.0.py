import asyncio
import websockets

async def hello():
    uri = "wss://sprs.herokuapp.com/zeroth/mefisto"
    async with websockets.connect(uri) as websocket:
        greetings = "Let's start"
        await websocket.send(greetings)

        equation = await websocket.recv()
        print(f"< {equation}")

        tokens = equation.split()
        print(tokens)

        result = 0;

        if ( tokens[2] == "+" ): 
            result = int(tokens[1]) + int(tokens[3])
        if ( tokens[2] == "-" ):           
            result = int(tokens[1]) - int(tokens[3])
        if ( tokens[2] == "*" ):     
            result = int(tokens[1]) * int(tokens[3])

        print(result)
        await websocket.send(str(result))
        

asyncio.get_event_loop().run_until_complete(hello())
