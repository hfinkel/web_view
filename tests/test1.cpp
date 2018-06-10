#include <web_view.h>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
using namespace std::chrono_literals;

int main(int argc, char *argv[]) {
  std::vector<std::string> args(argv, argv + argc);

  std::mutex m;
  std::condition_variable cv;
  bool done = false;

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
    std::unique_lock<std::mutex> ul(m);
    done = true;
    ul.unlock();
    cv.notify_one(); });

  auto f = w.display_from_uri("wv://first.html");
  std::cout << "initial display complete: " << f.get() << "\n";

  std::unique_lock<std::mutex> ul(m);
  while (!cv.wait_for(ul, 2000ms, [&] { return done; })) {
    auto r = w.run_script("var node = document.createElement('li');"
                          "node.appendChild(document.createTextNode('Time has passed'));"
                          "document.getElementById('dl').appendChild(node); "
                          "var d = new Date(); d.getTime();");
    std::cout << "got from script: " << r.get() << "\n";
  }

  std::cout << "web_view closed\n";

  return 0;
}

