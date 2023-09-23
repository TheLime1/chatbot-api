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
    async def get_response(self, query: QueryRequest) -> AsyncIterable[PartialResponse]:
        async for msg in stream_request(query, "ChatGPT", query.access_key):
            yield msg


    async def get_settings(self, setting: SettingsRequest) -> SettingsResponse:
        return SettingsResponse(
            allow_attachments=True,
            server_bot_dependencies={"ChatGPT": 1}
    )