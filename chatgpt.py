#only speak french
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
