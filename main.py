from fastapi_poe import make_app
from modal import Image, Stub, asgi_app

from chatgpt import ChatGPTBot
from echobot import EchoBot
from no_base import nobase

# Echo bot is a very simple bot that just echoes back the user's last message.
#bot = EchoBot()

# A bot that uses Poe's ChatGPT bot
# Good simple example of using another bot using Poe's third party bot API.
# For more details, see: https://developer.poe.com/server-bots/accessing-other-bots-on-poe
#bot = ChatGPTBot()

bot=nobase()

# The following is setup code that is required to host with modal.com
image = Image.debian_slim().pip_install_from_requirements("requirements.txt")
# Rename "poe-server-bot-quick-start" to your preferred app name.
stub = Stub("chatbot-api-service")


@stub.function(image=image)
@asgi_app()
def fastapi_app():
    # Optionally, provide your Poe access key here:
    # 1. You can go to https://poe.com/create_bot?server=1 to generate an access key.
    # 2. We strongly recommend using a key for a production bot to prevent abuse,
    # but the starter example disables the key check for convenience.
    # 3. You can also store your access key on modal.com and retrieve it in this function
    # by following the instructions at: https://modal.com/docs/guide/secrets
    POE_ACCESS_KEY = "C6C6kZ45ZHxGBk2LWZ8WdhR9A63N802n"
    app = make_app(bot, access_key=POE_ACCESS_KEY)
    #app = make_app(bot, allow_without_key=True)
    return app
