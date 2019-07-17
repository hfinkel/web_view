/*
 *                 Copyright (C) 2018, UChicago Argonne, LLC
 *                            All Rights Reserved
 *
 *                          Software Name: web_view
 *                      By: Argonne National Laboratory
 *                            OPEN SOURCE LICENSE
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *   3. Neither the name of the copyright holder nor the names of its
 *      contributors may be used to endorse or promote products derived from
 *      this software without specific prior written permission.
 *
 * *****************************************************************************
 *                                 DISCLAIMER
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 * *****************************************************************************
 */

#include <web_view.h>
#include <vector>
#include <string>
#include <chrono>
using namespace std::chrono_literals;

int main(int argc, char *argv[]) {
  std::vector<std::string> args(argv, argv + argc);

  wv::web_view w("web_view test app");
  w.set_uri_scheme_handler("wv", [&](const std::string &uri, std::ostream &os) {
    std::cout << "request: " << uri << "\n";
    os << "<html><head><title>" << uri << "</title></head><body><p>" << uri << "</p><table>";
    for (auto &a : args)
      os << "<tr><td>" << a << "</td></tr>" << "\n"; // we need some kind of "to_html" utility function.
    os << "</table>";
    os << "<p><a href=\"" << uri << "/more.html" << "\">more</a></p>";
    os << "<ul id='dl'></ul>";
    os << "</body></html>";

    return true;
  });
  w.set_close_handler([&]() {
    std::cout << "close handler called\n";
  });

  auto f = w.display_from_uri("wv://first.html");
  std::cout << "initial display complete: " << f.get() << "\n";

  while (!w.wait_for_close(2000ms)) {
    auto r = w.run_script("var node = document.createElement('li');"
                          "node.appendChild(document.createTextNode('Time has passed'));"
                          "document.getElementById('dl').appendChild(node); "
                          "var d = new Date(); d.getTime();");
    std::cout << "got from script: " << r.get() << "\n";
  }

  std::cout << "web_view closed\n";

  return 0;
}

