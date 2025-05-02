#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
# SPDX-FileCopyrightText: 2025 Eli2

from http.server import BaseHTTPRequestHandler, HTTPServer

class Handler(BaseHTTPRequestHandler):
    def do_GET(self):
        filesToServe = [
            ("/",                "web/index.html",            "text/html"),
            ("/index.html",      "web/index.html",            "text/html"),
            ("/MyHumanoid.js",   "build-web/MyHumanoid.js",   "text/javascript"),
            ("/MyHumanoid.wasm", "build-web/MyHumanoid.wasm", "application/wasm"),
            ("/MyHumanoid.data", "build-web/MyHumanoid.data", "application/zip"),
        ]
        for f in filesToServe:
            if f[0] == self.path:
                self.send_response(200)
                self.send_header("Content-type", f[2])
                self.end_headers()
                content = open(f[1], "rb").read()
                self.wfile.write(content)

if __name__ == "__main__":
    host = "localhost"
    port = 8080
    server = HTTPServer((host, port), Handler)
    print("Server started http://%s:%s" % (host, port))

    try:
        server.serve_forever()
    except KeyboardInterrupt:
        pass

    server.server_close()
    print("Server stopped")
