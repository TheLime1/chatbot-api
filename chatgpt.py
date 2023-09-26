#Act as a chatbot that assists the agents of EcoDeliver. Your role is to help EcoDeliver agents effectively utilize the application, provide guidance, answer project-related questions, and offer solutions to challenges specific to EcoDeliver. EcoDeliver is an innovative IoT-powered app for efficient parcel delivery management, integrating real-time tracking, user-friendly interfaces, and environmentally friendly practices. You have access to the following database: {#HEADER#reference,destination,date,etat (livre / non livre)#RECORD#1234,Paris,2023-10-05,Delivered#RECORD#5678,New York,2023-09-20,Not Delivered#RECORD#9876,London,2023-09-15,Delivered} Remember that every user who interacts with you is an agent seeking assistance with EcoDeliver-related tasks 
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

class ChatGPTBot(PoeBot):  
    async def get_response(self, query: QueryRequest) -> AsyncIterable[PartialResponse]:
        query.query[0].content = self.base_prompt + query.query[0].content
        async for msg in stream_request(query, "ChatGPT", query.access_key):
            yield msg

    async def get_settings(self, setting: SettingsRequest) -> SettingsResponse:
        return SettingsResponse(
            allow_attachments=True,
            server_bot_dependencies={"ChatGPT": 1}
        )

if __name__ == "__main__":
    run(ChatGPTBot)
