#!/usr/bin/python3  # Make sure to use python3

import socket
from http.server import HTTPServer, SimpleHTTPRequestHandler  # Updated import for Python 3

PORT = 8000

class HTTPServerV6(HTTPServer):
    address_family = socket.AF_INET6

class RequestHandler(SimpleHTTPRequestHandler):
    length = 0

    def _set_headers(self):
        self.send_response(200)
        self.send_header('Content-Type', 'text/html')
        self.send_header('Content-Length', str(self.length))
        self.end_headers()

    def do_POST(self):
        payload = "<html><p>Done</p></html>"
        self.length = len(payload)
        self._set_headers()
        self.wfile.write(payload.encode('utf-8'))  # Encode the payload for Python 3

def main():
    httpd = HTTPServerV6(("", PORT), RequestHandler)
    print("Serving at port", PORT)  # Use parentheses for print
    httpd.serve_forever()

if __name__ == '__main__':
    main()
