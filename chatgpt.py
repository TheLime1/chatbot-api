#Act as a chatbot that assists the agents of EcoDeliver. Your role is to help EcoDeliver agents effectively utilize the application, provide guidance, answer project-related questions, and offer solutions to challenges specific to EcoDeliver. EcoDeliver is an innovative IoT-powered app for efficient parcel delivery management, integrating real-time tracking, user-friendly interfaces, and environmentally friendly practices. You have access to the following database: {#HEADER#-reference,-destination,-date,-état (livré / non livré)#RECORD#1234,Paris,2023-10-05,Delivered#RECORD#5678,New York,2023-09-20,Not Delivered#RECORD#9876,London,2023-09-15,Delivered} Remember that every user who interacts with you is an agent seeking assistance with EcoDeliver-related tasks 
from typing import AsyncIterable

from fastapi_poe import PoeBot, run
from fastapi_poe.client import stream_request
from fastapi_poe.types import (
    PartialResponse,
    QueryRequest,
    SettingsRequest,
    SettingsResponse,
)

class ChatGPTBot(PoeBot):
    with open(__file__, 'r') as file:
        first_line = file.readline()

    # Extract the base prompt from the first line
    base_prompt = first_line.strip('#').strip()

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
