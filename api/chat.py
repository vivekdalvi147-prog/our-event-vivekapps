import os
import json
import requests
from http.server import BaseHTTPRequestHandler

class handler(BaseHTTPRequestHandler):
    
    # CORS (Cross-Origin) को हैंडल करने के लिए OPTIONS मेथड
    def do_OPTIONS(self):
        self.send_response(200, "ok")
        self.send_header('Access-Control-Allow-Origin', '*')
        self.send_header('Access-Control-Allow-Methods', 'POST, OPTIONS')
        self.send_header('Access-Control-Allow-Headers', 'Content-Type')
        self.end_headers()

    def do_POST(self):
        # 1. Frontend से आया हुआ मैसेज पढ़ें
        content_length = int(self.headers.get('Content-Length', 0))
        post_data = self.rfile.read(content_length)
        
        try:
            req_body = json.loads(post_data)
            user_message = req_body.get("message", "Hello")
        except:
            user_message = "Hello"

        # 2. Vercel Environment Variables से अपनी API Key निकालें
        api_key = os.environ.get("VIVEK_API")

        # अगर API Key नहीं मिलती है तो Error भेजें
        if not api_key:
            self.send_response(500)
            self.send_header('Content-type', 'application/json')
            self.send_header('Access-Control-Allow-Origin', '*')
            self.end_headers()
            self.wfile.write(json.dumps({"error": "VIVEK_API environment variable is missing!"}).encode('utf-8'))
            return

        # 3. Bol-AI API को रिक्वेस्ट भेजें
        url = "https://bol-ai.vercel.app/api/chat2"
        
        headers = {
            "Content-Type": "application/json",
            "x-my-client-key": api_key  # यहाँ आपकी Vercel वाली API Key जा रही है
        }
        
        payload = {
            "message": user_message,
            "system": "You are a helpful assistant for Our Events platform. Answer politely and concisely."
        }

        try:
            # Bol-AI को POST request करें
            response = requests.post(url, headers=headers, json=payload)
            result = response.json()

            self.send_response(200)
            self.send_header('Content-type', 'application/json')
            self.send_header('Access-Control-Allow-Origin', '*')
            self.end_headers()

            # 4. Bol-AI का जवाब Frontend को वापस भेजें
            if response.status_code == 200:
                ai_reply = result.get("choices", [{}])[0].get("message", {}).get("content", "No response from AI")
                # Frontend में हमने data.reply का इस्तेमाल किया है, इसलिए 'reply' की में भेज रहे हैं
                self.wfile.write(json.dumps({"reply": ai_reply}).encode('utf-8'))
            else:
                error_msg = result.get("error", "Unknown API Error")
                self.wfile.write(json.dumps({"reply": f"Error: {error_msg}"}).encode('utf-8'))

        except Exception as e:
            # अगर कोई सर्वर या नेटवर्क एरर आता है
            self.send_response(500)
            self.send_header('Content-type', 'application/json')
            self.send_header('Access-Control-Allow-Origin', '*')
            self.end_headers()
            self.wfile.write(json.dumps({"reply": f"Server Error: {str(e)}"}).encode('utf-8'))
