import os
import json
import requests

# Vercel वर environment variable म्हणून VIVEK_API सेव्ह करा
# तिथे key: VIVEK_API  आणि value: तुमचा खरा API-की
API_KEY = os.getenv("VIVEK_API", "your_custom_key_here")

def bol_ai_chat(user_message: str):
    url = "https://bol-ai.vercel.app/api/chat2"
    headers = {
        "Content-Type": "application/json",
        "x-my-client-key": API_KEY
    }
    payload = {
        "message": user_message,
        "system": "You are a helpful assistant"
    }

    response = requests.post(url, headers=headers, data=json.dumps(payload))

    if response.ok:
        data = response.json()
        print("AI:", data["choices"][0]["message"]["content"])
    else:
        print("Error:", response.json().get("error", "Unknown error"))

if __name__ == "__main__":
    bol_ai_chat("Hello, how can I use this API?")
