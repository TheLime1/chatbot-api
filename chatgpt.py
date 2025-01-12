#Act as a chatbot that assists the agents of EcoDeliver. Your role is to help EcoDeliver agents effectively utilize the application, provide guidance, answer project-related questions, and offer solutions to challenges specific to EcoDeliver. EcoDeliver is an innovative IoT-powered app for efficient parcel delivery management, integrating real-time tracking, user-friendly interfaces, and environmentally friendly practices. You have access to the following database: {#HEADER#reference,destination,date,state (Delivered / not Delivered)#RECORD#1234,blabla,2023-10-05,Delivered#RECORD#5678,New York,2023-09-20,Not Delivered#RECORD#9876,London,2023-09-15,Delivered} Remember that every user who interacts with you is an agent seeking assistance with EcoDeliver-related tasks 
from typing import AsyncIterable

from fastapi_poe import PoeBot, run
from fastapi_poe.client import stream_request
from fastapi_poe.types import (
    PartialResponse,
    QueryRequest,
    SettingsRequest,
    SettingsResponse,
)
import gspread
import re

gc = gspread.service_account(filename="config.py") #duck tapped solution >>> from json to python
sh = gc.open("ecodeliver")
ws = sh.get_worksheet(0)
with open(__file__, 'r') as file: #duck tapped solution >>> cant upload txt to modal so i put it in the same file as comment xD
    first_line = file.readline()

base_prompt = first_line.strip('#').strip()
base_database = first_line[first_line.index('{')+1:first_line.index('}')]
base_database = base_database.replace('#HEADER#', '')  # Remove the #HEADER# prefix

# First, split the base_database string into individual records
records = [r.strip() for r in base_database.split('#RECORD#') if r.strip()]
# Then, split each record into fields
data = [r.split(',') for r in records]
# Finally, update the worksheet with the data
ws.update('A1', data)
row_counter = len(records)+1
print(row_counter)

class ChatGPTBot(PoeBot):
    def __init__(self):
        self.row_counter = row_counter  # Initialize row_counter

    async def get_response(self, query: QueryRequest) -> AsyncIterable[PartialResponse]:
        content = query.query[0].content
        command_check=query.query[-1].content

        if "add_command" in command_check.lower() or "new_command" in command_check.lower():
            # Extract command details from the message
            match = re.match(r"(add|new)_command reference:(\d+) destination:(\w+) date:(\d{4}-\d{2}-\d{2}) state:(\w+)", command_check.lower())
            if match:
                reference, destination, date, state = match.groups()[1:]
                # Add the command to the sheet
                ws.append_row([reference, destination, date, state])
                self.row_counter += 1  # Increment row_counter
                response = "Command added successfully."
            else:
                response = "Invalid command format. Please enter the command info in the format: add_command reference:xxxx destination:xxxx date:xxxx state:xxxx"
                # Add this line to log the reason for the invalid command format
                print(f"Invalid command format: {command_check}")
            yield PartialResponse(text=response)
        else:
            query.query[0].content = base_prompt + content
            async for msg in stream_request(query, "ChatGPT", query.access_key):
                yield msg



    async def get_settings(self, setting: SettingsRequest) -> SettingsResponse:
        return SettingsResponse(
            allow_attachments=True,
            server_bot_dependencies={"ChatGPT": 1}
        )

if __name__ == "__main__":
    run(ChatGPTBot)