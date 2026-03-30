import os
import json
import requests
from http.server import BaseHTTPRequestHandler

class handler(BaseHTTPRequestHandler):
    
    def do_OPTIONS(self):
        self.send_response(200, "ok")
        self.send_header('Access-Control-Allow-Origin', '*')
        self.send_header('Access-Control-Allow-Methods', 'POST, OPTIONS')
        self.send_header('Access-Control-Allow-Headers', 'Content-Type')
        self.end_headers()

    def do_POST(self):
        content_length = int(self.headers.get('Content-Length', 0))
        post_data = self.rfile.read(content_length)
        
        try:
            req_body = json.loads(post_data)
            user_message = req_body.get("message", "नमस्कार Vivek 7311")
        except:
            user_message = "नमस्कार Vivek 7311"

        api_key = os.environ.get("VIVEK_API")

        if not api_key:
            self.send_response(500)
            self.send_header('Content-type', 'application/json')
            self.send_header('Access-Control-Allow-Origin', '*')
            self.end_headers()
            self.wfile.write(json.dumps({"error": "VIVEK_API environment variable सापडली नाही!"}).encode('utf-8'))
            return

        url = "https://bol-ai.vercel.app/api/chat2"
        
        headers = {
            "Content-Type": "application/json",
            "x-my-client-key": api_key
        }
        
        payload = {
            "message": user_message + " (Vivek 7311)",
            "system": "तू Our Events platform साठी एक मदत करणारा assistant आहेस. कृपया नम्र आणि थोडक्यात उत्तर दे."
        }

        try:
            response = requests.post(url, headers=headers, json=payload)
            result = response.json()

            self.send_response(200)
            self.send_header('Content-type', 'application/json')
            self.send_header('Access-Control-Allow-Origin', '*')
            self.end_headers()

            if response.status_code == 200:
                ai_reply = result.get("choices", [{}])[0].get("message", {}).get("content", "AI कडून उत्तर मिळालं नाही")
                self.wfile.write(json.dumps({"reply": ai_reply + " - Vivek 7311"}).encode('utf-8'))
            else:
                error_msg = result.get("error", "Unknown API Error")
                self.wfile.write(json.dumps({"reply": f"Error: {error_msg}"}).encode('utf-8'))

        except Exception as e:
            self.send_response(500)
            self.send_header('Content-type', 'application/json')
            self.send_header('Access-Control-Allow-Origin', '*')
            self.end_headers()
            self.wfile.write(json.dumps({"reply": f"Server Error: {str(e)}"}).encode('utf-8'))
